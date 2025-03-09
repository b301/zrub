#include "utils.h"


/**
 * @brief converts `struct sockaddr_storage` into a string
 * 
 * @param buf           buffer to write the string to
 * @param bufsize       size of the buffer
 * @param client_addr   client address data
 */
bool client_addr_as_str(char *buf, u32 bufsize, struct sockaddr_storage client_addr)
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
 * @param sockfd        address of i32
 * @param backlog       listener queue size
 */
bool initialize_server(const char *addr, const char *port, i32 *sockfd, u32 backlog)
{
    i32 rc;

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

    i32 opt = 1;

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
 * @param epoll_fd          address of i32
 */
bool initialize_epoll(i32 server_sockfd, i32 *epoll_fd)
{
    struct epoll_event event;

    i32 _epoll_fd = epoll_create1(0);
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
