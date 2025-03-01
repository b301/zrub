#define ZRUBLIB
#include <zrub/zrub.h>

#define __USE_XOPEN2K       1

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define CONN_BACKLOG         8


i32 main()
{
    const char *port = "1002";
    char addr_str[INET_ADDRSTRLEN];
    void *addr;

    struct addrinfo hints;
    struct addrinfo *res;

    i32 sockfd;
    i32 rc;

    memset(&hints, 0, sizeof(hints));           // empty hints
    hints.ai_family = AF_UNSPEC;                // sets ipv4 family
    hints.ai_socktype = SOCK_STREAM;            // sets tcp protocol
    hints.ai_flags = AI_PASSIVE;                // fills ip address automatically

    // fill res data
    rc = getaddrinfo(NULL, port, &hints, &res);
    if (rc != 0)
    {
        ZRUB_LOG_ERROR("failed to get address info: %s\n", gai_strerror(rc));
        return 1;    
    }     

    // get a socket by syscall
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1)
    {
        ZRUB_LOG_ERROR("failed to create a socket\n");
        return 1;
    }

    addr = &(((struct sockaddr_in *)res->ai_addr)->sin_addr);
    inet_ntop(res->ai_family, addr, addr_str, sizeof(addr_str));

    // bind the socket to the port 
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {

        ZRUB_LOG_ERROR("failed to bind %s on port %s\n", addr_str ,port);
        return 1;
    }

    ZRUB_LOG_INFO("successfully binded %s on port %s\n", addr_str, port);

    if (listen(sockfd, CONN_BACKLOG) == -1)
    {
        ZRUB_LOG_ERROR("failed to listen\n");
        return 1;
    }

    close(sockfd);
    freeaddrinfo(res);

    return 0;
}
