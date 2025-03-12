#define CONN_BACKLOG         8
#define MAX_EPOLL_EVENTS     16

#include "server/server_headers.h"
#include "server/utils.h"

#include "server/user.h"
#include "server/pkt.h"

#include "proto/user.pb.h"


u8 generic_key[32] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
};


void check_packet(u8 *buf, u32 bufsize);

volatile sig_atomic_t stop_server = 0;

struct echat_user_node *head = NULL;

/**
 * @brief assign global variable `stop_server` to 1 on SIGINT or SIGTERM to stop the server!
 */
void handle_shutdown(i32 signum)
{
    (void)(signum);
    stop_server = 1;
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
bool handle_connections(i32 server_sockfd, i32 epoll_fd, u32 max_events)
{
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    // char client_ip_addr_str[INET6_ADDRSTRLEN];

    i32 num_events;
    struct epoll_event events[max_events];

    // loop vars
    i32 cfd;        // current fd
    i32 nfd;        // new fd
    struct epoll_event e;

    u32 bufsize = 1024;
    u8 buf[bufsize];

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
        for (i32 i = 0; i < num_events; i++)
        {
            cfd = events[i].data.fd;
            if (cfd == server_sockfd)
            {
                // accept new connections
                nfd = accept(cfd, (struct sockaddr*)&client_addr, &client_addr_size);

                if (nfd == -1)
                {
                    ZRUB_LOG_ERROR("failed accepting client: %s\n", strerror(errno));
                    continue;
                }

                // set non-blocking mode on socket
                i32 flags = fcntl(nfd, F_GETFL, 0);
                fcntl(nfd, F_SETFL, flags | O_NONBLOCK);

                // add client sock to epoll
                e.events = EPOLLIN | EPOLLET;
                e.data.fd = nfd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, nfd, &e) == -1)
                {
                    ZRUB_LOG_ERROR("error manipulating epoll instance ADD: %s\n", strerror(errno));
                    close(nfd);
                }

                if (client_addr_as_str((char*)buf, bufsize, client_addr))
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
                // handle client
                while (1)
                {
                    i32 bytes_read = read(cfd, buf, bufsize);
    
                    
                    if (bytes_read > 0)
                    {
                        ZRUB_LOG_INFO("received %d bytes\n", bytes_read);
                        // echo to client
                        check_packet(buf, bytes_read);
                        
                        // generate true response
                        user_registration_response resp = user_registration_response_init_zero;
                        resp.status = true;

                        u8 outbuf[1024];
                        echat_epkt pkt = echat_epkt_init_zero;

                        // serialize response
                        {
                            pb_ostream_t ostream = pb_ostream_from_buffer(outbuf, 1024);

                            pb_encode(&ostream, user_registration_response_fields, &resp);
                            echat_encrypt_packet(&pkt, outbuf, ostream.bytes_written, generic_key);
                        }

                        // serialize packet and send over socket
                        {
                            pb_ostream_t ostream = pb_ostream_from_buffer(outbuf, 1024);

                            pb_encode(&ostream, echat_epkt_fields, &pkt);
                            send(cfd, outbuf, ostream.bytes_written, 0);
                            ZRUB_LOG_INFO("sent %d to client %d\n", ostream.bytes_written, cfd);
                        }

                        // send(cfd, buf, bytes_read, 0);
                    }
 
                    else if (bytes_read == 0)
                    {
                        ZRUB_LOG_INFO("client disconnected: %d\n", cfd);
      
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cfd, &e) == -1)
                        {
                            ZRUB_LOG_ERROR("error manipulating epoll instance DEL: %s\n", strerror(errno));
                            return false;
                        }

                        ZRUB_LOG_INFO("deleted %d from epoll\n", cfd);

                        close(cfd);
                        break;
                    }
    
                    else if (bytes_read < 0)
                    {
                        // no more data available
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            ZRUB_LOG_DEBUG("finished reading %d\n", cfd);
                        }
                        
                        else
                        {
                            ZRUB_LOG_ERROR("error reading from client %d: %s\n", cfd, strerror(errno));
                            
                            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cfd, &e) == -1)
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
            }
        }
    }

    return true;
}

void check_packet(u8 *buf, u32 bufsize)
{
    echat_epkt pkt;
    pb_istream_t istream = pb_istream_from_buffer(buf, bufsize);

    if (!pb_decode(&istream, echat_epkt_fields, &pkt))
    {
        ZRUB_LOG_ERROR("failed to decode packet\n");
        return;
    }

    ZRUB_LOG_INFO("[[info about packet]]\n");

    switch (pkt.action)
    {
        case echat_action_USER_REGISTRATION_REQUEST:
            ZRUB_LOG_INFO("registration request\n");
            break;

        case echat_action_USER_REGISTRATION_RESPONSE:
            ZRUB_LOG_INFO("registratino response\n");
            break;
        
        default:
            ZRUB_LOG_INFO("dunno\n");
            break;
    }
}

i32 main(int argc, char **argv)
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

    /* SERVER INITIALIZATION STARTING*/
    i32 server_sockfd;

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

    i32 epoll_fd;
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
    }

    /* CLEANUP */
    close(epoll_fd);
    close(server_sockfd);

    return 0;
}
