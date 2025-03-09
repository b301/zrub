#ifndef ECHAT_SERVER_HEADERS_H
#define ECHAT_SERVER_HEADERS_H

/* SERVER */
#define __USE_XOPEN2K       1

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
#include <zrub/zrub.h>              // my custom lib

#include <sodium.h>                 // crypto lib

#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>


#define CONN_BACKLOG         8
#define MAX_EPOLL_EVENTS     16


#endif // ECHAT_SERVER_HEADERS_H
