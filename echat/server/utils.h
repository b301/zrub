#ifndef ECHAT_UTILS_H
#define ECHAT_UTILS_H

#include "server_headers.h"


bool initialize_server(const char *addr, const char *port, i32 *sockfd, u32 backlog);
bool initialize_epoll(i32 server_sockfd, i32 *epoll_fd);

bool client_addr_as_str(char *buf, u32 bufsize, struct sockaddr_storage client_addr);


#endif // ECHAT_UTILS_H
