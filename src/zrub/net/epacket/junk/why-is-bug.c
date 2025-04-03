#define ZRUBLIB
#define ZRUBLIB_DEBUG
#include <zrub/zrub.h>


struct client {
    uint32_t            recv_size;
    uint32_t            msg_size;
    int32_t             cfd;
    struct zrub_epacket pkt;
    struct client *next;
};

struct client *create_state(int32_t cfd)
{
    ZRUB_LOG_DEBUG("Creating %d on %p\n", cfd);
    struct client *s = (struct client*)ZRUB_MALLOC(sizeof(struct client));   

    s->cfd = cfd;
    s->msg_size = 0;
    s->recv_size = 0;

    memset(s->pkt.data, 0, ZRUB_PKT_DATA_MAX);
    memset(s->pkt.nonce, 0, ZRUB_PKT_NONCE_LEN);
    memset(s->pkt.macbytes, 0, ZRUB_PKT_MACBYTES_LEN);
    s->pkt.data_length = 0;

    s->next = NULL;
}

struct client *get_client_state(struct client **head, int32_t cfd)
{
    if (head == NULL)
    {
        return NULL;
    }

    if ((*head) == NULL)
    {
        (*head) = create_state(cfd);
        ZRUB_LOG_DEBUG("Creating %d on %p\n", cfd, (*head));
        return (*head);
    }

    while ((*head)->next == NULL)
    {
        if ((*head)->cfd == cfd)
        {
            ZRUB_LOG_DEBUG("retrieving %d on %p\n", cfd, (*head));
            return (*head);
        }

        (*head) = (*head)->next;
    }

    if ((*head)->cfd == cfd)
    {
        return (*head);
    }

    struct client *new_state = create_state(cfd);

    (*head)->next = new_state;
    return new_state;
}


int main()
{
    struct client *head = NULL;
    struct client *state = get_client_state(&head, 3);

    ZRUB_DVAR_INT(state->cfd);
    ZRUB_DVAR_INT(state->msg_size);
    ZRUB_DVAR_UINT(state->recv_size);
    ZRUB_DVAR_UINT(state->msg_size);
    ZRUB_DVAR_INT(state->pkt.data_length);
    ZRUB_DVAR_BYTES(state->pkt.data, state->pkt.data_length);

    state = get_client_state(&head, 3);
    state = get_client_state(&head, 3);
    return 0;
}
