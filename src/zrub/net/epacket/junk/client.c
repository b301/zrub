#define ZRUBLIB
#define ZRUBLIB_DEBUG
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

uint8_t generic_key[32] = {
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


int32_t main(int32_t argc, char **argv)
{
    if (argc < 3)
    {
        ZRUB_LOG_INFO("usage: <addr> <port>\n");
        return false;
    }

    char *srv_host = argv[1];
    char *srv_port = argv[2];

    int32_t sockfd;                     // the client connection sockfd
    // int32_t c;                          // number of bytes

    // char buf[1024];                 // msg buffer

    struct addrinfo hints;                  // ??
    struct addrinfo *srv_info = NULL;;      // server info (gets a linked list of addrinfo)
    struct addrinfo *p = NULL;             // linked list ptr

    int32_t rv;                         // return value
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

    uint8_t buf[128];
    char name[] = "oded";
    char passwd[] = "sigmaballs";

    // serialize pkt
    {
        uint8_t sbuf[512] = { 0 };
        struct zrub_serializer serializer = {
            .buf = sbuf,
            .bufsize = 512,
            .offset = 0,
        };

        // 4
        ZRUB_DVAR_INT(serializer.offset);
        zrub_serialize_unsigned_int32(
            serializer.buf,
            serializer.bufsize,
            0xffffffff,
            &serializer.offset
        );
        ZRUB_DVAR_INT(serializer.offset);

        // 4 + 4
        zrub_serialize_string(
            serializer.buf,
            serializer.bufsize,
            name,
            zrub_str_len(name),
            &serializer.offset
        );
        ZRUB_DVAR_INT(serializer.offset);

        // 4 + 10
        zrub_serialize_string(
            serializer.buf,
            serializer.bufsize,
            passwd,
            zrub_str_len(passwd),
            &serializer.offset
        );
        ZRUB_DVAR_INT(serializer.offset);

        ZRUB_LOG_DEBUG("serialized: ");
        zrub_bytes_print(serializer.buf, serializer.offset);

        // send packet
        struct zrub_epacket pkt = { 0 };
        zrub_epacket_encrypt(&pkt, serializer.buf, serializer.offset, generic_key);

        ZRUB_LOG_DEBUG("encrypted: ");
        zrub_bytes_print(pkt.data, pkt.data_length);

        uint8_t buf[4];
        uint32_t offset = 0;

        int32_t message_size = 4 + ZRUB_PKT_NONCE_LEN + ZRUB_PKT_MACBYTES_LEN + pkt.data_length;
        ZRUB_DVAR_INT(message_size);
        zrub_serialize_unsigned_int32(buf, 4, message_size, &offset);

        ZRUB_DVAR_BYTES(pkt.nonce, ZRUB_PKT_NONCE_LEN);
        ZRUB_DVAR_BYTES(pkt.macbytes, ZRUB_PKT_MACBYTES_LEN);

        send(sockfd, buf, 4, 0);

        zrub_time_sleep(1000);
        send(sockfd, pkt.nonce, ZRUB_PKT_NONCE_LEN, 0);

        zrub_time_sleep(1000);
        send(sockfd, pkt.macbytes, ZRUB_PKT_MACBYTES_LEN, 0);

        uint32_t count = 0;
        
        struct zrub_bytes_iter iteration = {
            .data = pkt.data,
            .ptr = 0,
            .usage = pkt.data_length
        };

        ZRUB_DVAR_BYTES(iteration.data, iteration.usage);

        uint8_t buffer[512];
        uint32_t buflen = 0;
        uint32_t blocksize = 8;

        if (zrub_bytes_iter_begin(&iteration, buffer, &buflen, blocksize)) {
            do {
                count += send(sockfd, buffer, buflen, 0);

                ZRUB_DVAR_BYTES(buffer, buflen);
                zrub_time_sleep(1000);
            } while (!zrub_bytes_iter_end(iteration) && zrub_bytes_iter_next(&iteration, buffer, &buflen, blocksize));
        }

        ZRUB_DVAR_UINT(count);
        ZRUB_DVAR_UINT(pkt.data_length);
        if (count == pkt.data_length)
        {
            printf("all is well\n");
        }
    }

    close(sockfd);
    return 0;
}
