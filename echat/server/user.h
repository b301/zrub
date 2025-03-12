#ifndef ECHAT_USER_H
#define ECHAT_USER_H


#include "server_headers.h"

#include "proto/secure.pb.h"

#include <zrub/3p/uuid/uuid4.h>
#include <sodium.h>


#define ECHAT_USER_MINNAME      4
#define ECHAT_USER_MAXNAME      32

#define ECHAT_USER_MINPASSWD    8
#define ECHAT_USER_MAXPASSWD    32

enum echat_g_user_role {
    ADMIN = 0,
    MEMBER = 1,
};

#define ECHAT_USER_ROLE_ADMIN        (enum echat_g_user_role)ADMIN
#define ECHAT_USER_ROLE_MEMBER       (enum echat_g_user_role)MEMBER


struct echat_user {
    char name[ECHAT_USER_MAXNAME];
    char passwd_hash[crypto_pwhash_STRBYTES];
    char uuid4[UUID4_STR_BUFFER_SIZE];
    enum echat_g_user_role role;
};

struct echat_user_node {
    struct echat_user *user;
    struct echat_user_node *next;
};


bool echat_user_create(struct echat_user *user, char *name, char *passwd);
bool echat_user_verify_passwd(struct echat_user user, char *passwd);

void echat_user_node_set(struct echat_user_node *ptr, struct echat_user *user);
bool echat_user_list_append(struct echat_user_node *head, struct echat_user *user);


#endif // ECHAT_USER_H
