#define CONN_BACKLOG 8
#define MAX_EPOLL_EVENTS 16

#define __USE_XOPEN2K       true

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <asm-generic/socket.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <signal.h>

/* LIBS */
#define ZRUBLIB
#define ZRUBLIB_DEBUG
#include "zrub/zrub.h"              // my custom lib

#define CONN_BACKLOG         8
#define MAX_EPOLL_EVENTS     16

#define ZRUBLIB
#include "zrub/zrub.h"


/**
 * @brief converts `struct sockaddr_storage` into a string
 * 
 * @param buf           buffer to write the string to
 * @param bufsize       size of the buffer
 * @param client_addr   client address data
 */
bool client_addr_as_str(char *buf, uint32_t bufsize, struct sockaddr_storage client_addr)
{
    // Determine the IP version and convert the client's IP address to a human-readable string
    if (client_addr.ss_family == AF_INET)
    {
        if (bufsize < INET_ADDRSTRLEN)
        {
            return false;
        }

        // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&client_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), buf, bufsize);

        return true;
    }

    if (client_addr.ss_family == AF_INET6)
    {
        if (bufsize < INET6_ADDRSTRLEN)
        {
            return false;
        }

        // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&client_addr;
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), buf, bufsize);

        return true;
    }

    return false;
}


/**
 * @brief initailizes a server socket
 * 
 * @param addr          address to use
 * @param port          the port to bind
 * @param sockfd        address of int32_t
 * @param backlog       listener queue size
 */
bool initialize_server(const char *addr, const char *port, int32_t *sockfd, uint32_t backlog)
{
    int32_t rc;

    ZRUB_LOG_INFO("server initialization starting...\n");
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));               // empty hints
    hints.ai_family = AF_UNSPEC;                    // sets unknown ipaddr
    hints.ai_socktype = SOCK_STREAM;                // sets tcp protocol
    hints.ai_flags = AI_PASSIVE;                    // select whatever ipaddr on the server

    rc = getaddrinfo(addr, port, &hints, &res);     // get address info and set assign the res variable to it
    if (rc != 0)
    {
        ZRUB_LOG_ERROR("failed to get address info with (%s, %s): %s\n", addr, port, gai_strerror(rc));
        return false;
    }

    // request socket creation from the system
    *sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);                   // socket creation syscall
    if (*sockfd == -1)
    {
        ZRUB_LOG_ERROR("failed to create a socket\n");
        return false;
    }

    int32_t opt = 1;

    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        ZRUB_LOG_ERROR("error setting sock opt `SO_REUSEADDR`: %s\n", strerror(errno));     // allows reuse of address
        return false;
    }

    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))                   // allows reuse of port
    {
        ZRUB_LOG_ERROR("error setting sock opt `SO_REUSEPORT`: %s\n", strerror(errno));
        return false;
    }

    char addr_str[INET6_ADDRSTRLEN];
    void *_addr = &(((struct sockaddr_in *)res->ai_addr)->sin_addr);            // point addr to sockaddr_in to sockaddr_in of the server res
    inet_ntop(res->ai_family, _addr, addr_str, sizeof(addr_str));               // convert network to presentation

    // attempt bind the socket to the port
    if (bind(*sockfd, res->ai_addr, res->ai_addrlen) == -1)                     // binding the socket to the port
    {
        ZRUB_LOG_ERROR("failed to bind %s on port %s\n", addr_str ,port);
        close(*sockfd);
        return false;
    }

    ZRUB_LOG_INFO("successfully binded %s on port %s\n", addr_str, port);

    if (listen(*sockfd, backlog) == -1)                                         // start listening on the socket
    {
        ZRUB_LOG_ERROR("failed to listen\n");
        close(*sockfd);
        return false;
    }

    freeaddrinfo(res);                                                          // free unneeded address info

    return true;
}


/**
 * @brief initializes an epoll socket
 * 
 * @param server_sockfd     a server socket
 * @param epoll_fd          address of int32_t
 */
bool initialize_epoll(int32_t server_sockfd, int32_t *epoll_fd)
{
    struct epoll_event event;

    int32_t _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1)
    {
        ZRUB_LOG_ERROR("failed creating epoll: %s\n", strerror(errno));
        *epoll_fd = -1;
        return false;
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = server_sockfd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, server_sockfd, &event) == -1)
    {
        ZRUB_LOG_ERROR("failed to manipulate epoll instance: %s\n", strerror(errno));
        *epoll_fd = -1;
        return false;
    }

    *epoll_fd = _epoll_fd;
    return true;
}


uint8_t generic_key[32] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};

void check_packet(uint8_t *buf, uint32_t bufsize);

volatile sig_atomic_t stop_server = 0;

struct client {
    uint32_t            recv_size;
    uint32_t            msg_size;
    int32_t             cfd;
    struct zrub_epacket pkt;
    struct zrub_epacket_async_state state;
    struct client *next;
};

struct client *create_state(int32_t cfd)
{
    ZRUB_LOG_DEBUG("Creating %d on %p\n", cfd);
    struct client *s = (struct client*)ZRUB_MALLOC(sizeof(struct client));   

    s->cfd = cfd;
    s->msg_size = 0;
    s->recv_size = 0;

    memset(s->pkt.data, 0, ZRUB_PKT_DATA_MAX);
    memset(s->pkt.nonce, 0, ZRUB_PKT_NONCE_LEN);
    memset(s->pkt.macbytes, 0, ZRUB_PKT_MACBYTES_LEN);
    s->pkt.data_length = 0;

    s->next = NULL;
}

struct client *get_client_state(struct client **head, int32_t cfd)
{
    if (head == NULL)
    {
        return NULL;
    }

    if ((*head) == NULL)
    {
        (*head) = create_state(cfd);
        ZRUB_LOG_DEBUG("Creating %d on %p\n", cfd, (*head));
        return (*head);
    }

    while ((*head)->next == NULL)
    {
        if ((*head)->cfd == cfd)
        {
            ZRUB_LOG_DEBUG("retrieving %d on %p\n", cfd, (*head));
            return (*head);
        }

        (*head) = (*head)->next;
    }

    if ((*head)->cfd == cfd)
    {
        return (*head);
    }

    struct client *new_state = create_state(cfd);

    (*head)->next = new_state;
    return new_state;
}

void delete_client_state(
    struct client **headptr, 
    int32_t cfd
) 
{
    if (headptr == NULL || *headptr == NULL) {
        return;
    }

    struct client *current = *headptr;
    struct client *prev = NULL;

    while (current != NULL && current->cfd != cfd) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return; // Not found
    }

    if (prev == NULL) {
        *headptr = current->next; // Remove head
    } else {
        prev->next = current->next;
    }

    free(current);
}

void cleanup_state_head(struct client **headptr)
{
    if (headptr == NULL)
    {
        return;
    }

    struct client *head = *headptr;
    struct client *torm = NULL;

    while (head != NULL)
    {
        torm = head;
        head = head->next;

        free(torm);
    }

    *headptr = NULL;
}

static struct client *state_head = NULL;

/**
 * @brief assign global variable `stop_server` to 1 on SIGINT or SIGTERM to stop the server!
 */
void handle_shutdown(int32_t signum)
{
    (void)(signum);
    stop_server = 1;
}

bool handle_client(int32_t efd, int32_t cfd, struct epoll_event *e)
{
    while (1)
    {
        struct client *state = get_client_state(&state_head, cfd);

        uint8_t rc = zrub_epacket_recv_nonblock(&state->pkt, cfd, &state->state);
        
        switch (rc)
        {
        case ZRUB_PKT_SUCCESS:
        {
            ZRUB_LOG_INFO("FINALLY GOT ALL THE DATA\n");
        }
        break;

        case ZRUB_PKT_CLIENT_TERM:
        {
        }
        break;

        case ZRUB_PKT_AWAITING_DATA:
        {
            ZRUB_LOG_INFO("WE ARE AWAITING DATA!!\n");
            return true;
        }
        break;

        default:
        {
            ZRUB_LOG_INFO("failure code %u\n", rc);
            return false;
        }
        break;
        }

        uint32_t action = 0;
        if (state->pkt.data_length > 0)
        {
            return true;
        }

        else if (state->pkt.data_length == 0)
        {
            ZRUB_LOG_INFO("client disconnected: %d\n", cfd);

            if (epoll_ctl(efd, EPOLL_CTL_DEL, cfd, e) == -1)
            {
                ZRUB_LOG_ERROR("error manipulating epoll instance DEL: %s\n", strerror(errno));
                return false;
            }

            ZRUB_LOG_INFO("deleted %d from epoll\n", cfd);

            close(cfd);
            delete_client_state(&state_head, cfd);

            break;
        }

        else if (state->pkt.data_length < 0)
        {
            // no more data available
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                ZRUB_LOG_DEBUG("finished reading %d\n", cfd);
            }

            else
            {
                ZRUB_LOG_ERROR("error reading from client %d: %s\n", cfd, strerror(errno));

                if (epoll_ctl(efd, EPOLL_CTL_DEL, cfd, e) == -1)
                {
                    ZRUB_LOG_ERROR("error manipulating epoll instance DEL: %s\n", strerror(errno));
                    return false;
                }

                ZRUB_LOG_INFO("deleted %d from epoll\n", cfd);
                close(cfd);
            }

            // break either way
            break;
        }

        // zrub_time_sleep(10);
    }

    return true;
}
/**
 * @brief function that handles connections for the server
 *
 * @details manages non-blocking sockets using the linux epoll API
 *
 * @param server_sockfd     a server socket fd
 * @param epoll_fd          an epoll fd
 * @param max_events        max events size
 */
bool handle_connections(int32_t server_sockfd, int32_t epoll_fd, uint32_t max_events)
{
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    // char client_ip_addr_str[INET6_ADDRSTRLEN];

    int32_t num_events;
    struct epoll_event events[max_events];

    // loop vars
    int32_t cfd; // current fd
    int32_t nfd; // new fd
    struct epoll_event e;

    uint32_t bufsize = 1024;
    uint8_t buf[bufsize];

    while (1)
    {
        num_events = epoll_wait(epoll_fd, events, max_events, -1);

        if (num_events == -1)
        {
            if (errno == EINTR)
            {
                ZRUB_LOG_INFO("shutting down gracefully\n");
                return true;
            }

            ZRUB_LOG_ERROR("failed getting events: %s\n", strerror(errno));

            return false;
        }

        ZRUB_LOG_INFO("received %d events\n", num_events);
        for (int32_t i = 0; i < num_events; i++)
        {
            cfd = events[i].data.fd;
            if (cfd == server_sockfd)
            {
                // accept new connections
                nfd = accept(cfd, (struct sockaddr *)&client_addr, &client_addr_size);

                if (nfd == -1)
                {
                    ZRUB_LOG_ERROR("failed accepting client: %s\n", strerror(errno));
                    continue;
                }

                // set non-blocking mode on socket
                int32_t flags = fcntl(nfd, F_GETFL, 0);
                fcntl(nfd, F_SETFL, flags | O_NONBLOCK);

                // add client sock to epoll
                e.events = EPOLLIN | EPOLLET;
                e.data.fd = nfd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, nfd, &e) == -1)
                {
                    ZRUB_LOG_ERROR("error manipulating epoll instance ADD: %s\n", strerror(errno));
                    close(nfd);
                }

                if (client_addr_as_str((char *)buf, bufsize, client_addr))
                {
                    ZRUB_LOG_INFO("connection from %s\n", buf);
                }
                else
                {
                    ZRUB_LOG_ERROR("failed to parse client addr: %s\n", strerror(errno));
                }

                ZRUB_LOG_INFO("added client socket: %d\n", nfd);
            }

            else
            {
                bool res = false;

                // handle client
                ZRUB_JUST_MEASURE_PERF(
                    res = handle_client(epoll_fd, cfd, &e));

                if (res == false)
                {
                    ZRUB_LOG_DEBUG("failed to handle client: %d\n", cfd);
                    // return false;
                }
            }
        }
    }

    return true;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        ZRUB_LOG_ERROR("usage: <addr> <port>\n");
        return 0;
    }

    /* HANDLE SHUTDOWN */
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);

    ZRUB_LOG_INFO("libsodium %s initialization starting...\n", sodium_version_string());
    if (sodium_init() != 0)
    {
        ZRUB_LOG_ERROR("failed to init libsodium %s\n", sodium_version_string());
        return 1;
    }

    ZRUB_LOG_INFO("libsodium initialization finished\n", sodium_version_string());

    /* SERVER INITIALIZATION STARTING */
    int32_t server_sockfd;

    const char *addr = argv[1];
    const char *port = argv[2];
    if (!initialize_server(addr, port, &server_sockfd, CONN_BACKLOG))
    {
        ZRUB_LOG_ERROR("failed to initialize server\n");
        return 1;
    }

    ZRUB_LOG_INFO("server initialization finished\n");
    /* SERVER INITIALIZATION FINISHED */

    /* EPOL INITIALIZATION STARTING */
    ZRUB_LOG_INFO("epoll initialization starting...\n");

    int32_t epoll_fd;
    if (!initialize_epoll(server_sockfd, &epoll_fd))
    {
        ZRUB_LOG_ERROR("failed to initialize epoll\n");
        return 1;
    }

    ZRUB_LOG_INFO("epoll initialization finished\n");
    /* EPOLL INITIALIZATION FINISHED */

    while (!stop_server)
    {
        if (handle_connections(server_sockfd, epoll_fd, MAX_EPOLL_EVENTS))
        {
            ZRUB_LOG_INFO("ended gracefully\n");
        }
        else
        {
            ZRUB_LOG_ERROR("ended abruptly\n");
        }

        cleanup_state_head(&state_head);
    }

    /* CLEANUP */
    close(epoll_fd);
    close(server_sockfd);

    return 0;
}
