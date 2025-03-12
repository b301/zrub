#define ZRUBLIB
#include "zrub/zrub.h"

#define __USE_XOPEN2K       1

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define CONN_BACKLOG         8

#include "client/user.h"


u8 generic_key[32] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
};


/**
 * @brief converts to  sockaddr_in for ip v4 or v6
 * 
 * @param sa        sockaddr struct
 */

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


i32 main(i32 argc, char **argv)
{
    if (argc < 3)
    {
        ZRUB_LOG_INFO("usage: <addr> <port>\n");
        return false;
    }

    char *srv_host = argv[1];
    char *srv_port = argv[2];

    i32 sockfd;                     // the client connection sockfd
    // i32 c;                          // number of bytes

    // char buf[1024];                 // msg buffer

    struct addrinfo hints;                  // ??
    struct addrinfo *srv_info = NULL;;      // server info (gets a linked list of addrinfo)
    struct addrinfo *p = NULL;             // linked list ptr

    i32 rv;                         // return value
    char srv_ip[INET6_ADDRSTRLEN];  // inet6 > inet(4) thus covers both cases

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;                // do not specify ip v4 or v6
    hints.ai_socktype = SOCK_STREAM;            // tcp

    rv = getaddrinfo(srv_host, srv_port, &hints, &srv_info);
    if (rv != 0)
    {
        ZRUB_LOG_ERROR("error by getaddrinfo(...), %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = srv_info; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
        {
            ZRUB_LOG_ERROR("failed to create socket\n");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            ZRUB_LOG_ERROR("failed to connect socket\n");
            close(sockfd);
            continue;
        }

        // exit if managed to create and connect or ran out of addrinfo structs
        break;
    }

    if (p == NULL)
    {
        ZRUB_LOG_ERROR("failed to connect to %s %s\n", srv_host, srv_info);
        return 1;
    }

    ZRUB_LOG_INFO("found a valid server address info\n");

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), srv_ip, sizeof(srv_ip));
    ZRUB_LOG_INFO("connecting to %s %s via ip address %s\n", srv_host, srv_port, srv_ip);

    freeaddrinfo(srv_info);
    srv_info = NULL;

    u8 buf[128];
    i32 c;

    // TODO: handle sudden server disconnect
    while (1)
    {
        if (echat_register_user(sockfd, generic_key, "oded", "theb3stk1ng"))
        {
            ZRUB_LOG_INFO("successfully registered\n");
        }
        else
        {
            ZRUB_LOG_INFO("false\n");
        }

        zrub_time_sleep(500);                           // sleep half a sec
        continue;

        fgets((char*)buf, 128, stdin);                   // buffer size - byte for null termination

        if (strncmp((char*)buf, "\n", 1) == 0)
        {
            ZRUB_LOG_DEBUG("you have entered a newline - not reason to do any task\n");
            continue;
        }

        if (strncmp((char*)buf, "end\n", 4) == 0)
        {
            break;
        }

        buf[strcspn((char*)buf, "\r\n")] = '\0';

        c = send(sockfd, buf, strlen((char*)buf), MSG_NOSIGNAL);
        if (c == -1)
        {
            if (errno == EPIPE || errno == ECONNRESET)
            {
                ZRUB_LOG_ERROR("server disconnected unexpectedly\n");
            }

            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                ZRUB_LOG_ERROR("send operation would block\n");
            }

            ZRUB_LOG_INFO("error on send: %s\n", strerror(errno));
            break;
        }

        ZRUB_LOG_INFO("sent %ld bytes\n", c);

        if (c <= 0)
        {
            ZRUB_LOG_ERROR("send(...) returned value: %ld\n", c);
            break;
        }

        c = recv(sockfd, buf, 127, MSG_NOSIGNAL);
        if (c == -1)
        {
            ZRUB_LOG_INFO("error on send: %s\n", strerror(errno));
            break;
        }

        buf[c] = '\0';
        ZRUB_LOG_INFO("received `%s`\n", buf);
    }

    close(sockfd);
    return 0;
}
