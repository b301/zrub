#ifndef ECHAT_CLIENT_USER_H
#define ECHAT_CLIENT_USER_H

#include "client/client_headers.h"

#include "proto/user.pb.h"

#define ECHAT_USER_MINNAME      4
#define ECHAT_USER_MAXNAME      32

#define ECHAT_USER_MINPASSWD    8
#define ECHAT_USER_MAXPASSWD    32

#define ECHAT_ACTION_REGISTRATION_REQUEST       (echat_action)echat_action_USER_REGISTRATION_REQUEST;
#define ECHAT_ACTION_REGISTRATION_RESPONSE      (echat_action)echat_action_USER_REGISTRATION_RESPONSE;

bool echat_register_user(i32 sockfd, u8 *key, char *user, char *passwd);


#endif // ECHAT_CLIENT_USER_H
