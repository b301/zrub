#define ZRUBLIB
#include "zrub/zrub.h"

#include "zrub/3p/uuid/uuid4.h"


struct User {
    char username[255];
    char password[255];
    char uuid[40];
};


bool create_user(struct User *u, char *username, char *password)
{
    snprintf(u->username, 255, "%s", username);
    snprintf(u->password, 255, "%s", password);

    uuid4_state_t state;
    uuid4_t out;

    uuid4_seed(&state);

    uuid4_gen(&state, &out);
    uuid4_to_s(out, u->uuid, 40);
}


int main()
{
    struct User admin;
    struct User admin2;

    if (!create_user(&admin, "kildoes", "o518349276Z!"))
    {
        ZRUB_LOG_ERROR("failed to create user\n");
        return 1;
    }

    if (!create_user(&admin2, "kildoes", "o518349276Z!"))
    {
        ZRUB_LOG_ERROR("failed to create user\n");
        return 1;
    }

    ZRUB_LOG_INFO("%s user: %s with pass: %s\n", 
        admin.uuid, admin.username, admin.password);
    ZRUB_LOG_INFO("%s user: %s with pass: %s\n", 
        admin2.uuid, admin2.username, admin2.password);

    return 0;
}
