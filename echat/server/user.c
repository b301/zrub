#include "user.h"


/**
 * @brief create user
 * 
 * @param user          user to set data to
 * @param name          name of user
 * @param passwd        plaintext passwd
 * @returns true if created user
 */
bool echat_user_create(struct echat_user *user, char *name, char *passwd)
{
    u32 name_len = strlen(name);
    u32 passwd_len = strlen(passwd);

    if (name_len > ECHAT_USER_MAXNAME || name_len < ECHAT_USER_MINNAME)
    {
        ZRUB_LOG_ERROR("name length must be in range [4, 32]\n");
        return false;
    }

    if (passwd_len > ECHAT_USER_MAXPASSWD || passwd_len < ECHAT_USER_MINPASSWD)
    {
        ZRUB_LOG_ERROR("password length must be in range [8, 32]\n");
        return false;
    }

    if (crypto_pwhash_str(user->passwd_hash, passwd, strlen(passwd), crypto_pwhash_OPSLIMIT_SENSITIVE, crypto_pwhash_MEMLIMIT_SENSITIVE) != 0)
    {
        ZRUB_LOG_ERROR("out of memory\n");
        return false;
    }

    // set username (snprintf null termiantes)
    snprintf(user->name, 33, "%s", name);
    user->role = ECHAT_USER_ROLE_MEMBER;

    // generate uuid4
    uuid4_t id;
    uuid4_state_t state;

    uuid4_seed(&state);
    uuid4_gen(&state, &id);

    uuid4_to_s(id, user->uuid4, UUID4_STR_BUFFER_SIZE);

    ZRUB_LOG_DEBUG("created user %s with passwd %s\n", name, passwd);
    return true;
}

/**
 * @brief verify user passwd
 * 
 * @param user          user to check passwd against
 * @param passwd        plaintext passwd to check
 */
bool echat_user_verify_passwd(const struct echat_user user, char *passwd)
{
    if (crypto_pwhash_str_verify(user.passwd_hash, passwd, strlen(passwd)) != 0)
    {
        ZRUB_LOG_DEBUG("incorrect passwd! try again\n");
        return false;
    }

    ZRUB_LOG_DEBUG("correct password, welcome %s\n", user.name);
    return true;
}

/**
 * @brief set user node
 */
void echat_user_node_set(struct echat_user_node *ptr, struct echat_user *user)
{
    ptr->next = NULL;
    ptr->user = user;
}

/**
 * @brief appends set user to echat user node
 *
 * @param head          head of list
 * @param user          set user to append
 * @return true if appended
 */
bool echat_user_list_append(struct echat_user_node *head, struct echat_user *user)
{
    struct echat_user_node *p = head;
    u32 counter = 0;

    while (p->next != NULL)
    {
        counter++;
        p = p->next;
    }

    ZRUB_LOG_INFO("counter is %lu\n", counter);

    struct echat_user_node *n = (struct echat_user_node*)malloc(sizeof(struct echat_user_node));
    if (n == NULL)
    {
        ZRUB_LOG_ERROR("failed to allocate memory fo echat_user_node*\n");
        return false;
    }

    memset(n, 0, sizeof(struct echat_user_node));

    echat_user_node_set(n, user);
    p->next = n;

    ZRUB_LOG_DEBUG("set user\n");

    return true;
}
