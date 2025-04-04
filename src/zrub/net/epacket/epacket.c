#include "zrub/net/epacket/epacket.h"


static bool zrub_epacket_encrypt_data(struct zrub_epacket *pkt, uint8_t *data, int32_t data_length, uint8_t* key)
{
    /* generate random nonce */
    zrub_randombytes(pkt->nonce, ZRUB_PKT_NONCE_LEN);

    if (crypto_secretbox_detached(pkt->data, pkt->macbytes, data, data_length, pkt->nonce, key) != 0)
    {
        ZRUB_LOG_ERROR("failed to encrypt");
        return false;
    }

    ZRUB_LOG_DEBUG("successfully encrypted data of length %lu\n", data_length);
    return true;
}

bool zrub_epacket_encrypt(struct zrub_epacket *pkt, uint8_t *data, int32_t data_length, uint8_t *key)
{
    if (data_length > ZRUB_PKT_DATA_MAX)
    {
        ZRUB_LOG_ERROR("data length must be less or equal to %d, current: %lu\n", ZRUB_PKT_DATA_MAX, data_length);
        return false;
    }

    if (!zrub_epacket_encrypt_data(pkt, data, data_length, key))
    {
        ZRUB_LOG_ERROR("failed to encrypt data\n");
        return false;
    }

    /* the size of the actual data (max 975 bytes) */
    pkt->data_length = data_length;

    return true;
}

static bool zrub_epacket_decrypt_data(struct zrub_epacket *pkt, uint8_t *key)
{
    uint8_t temp[ZRUB_PKT_DATA_MAX] = { 0 };

    if (crypto_secretbox_open_detached(temp, pkt->data, pkt->macbytes, pkt->data_length, pkt->nonce, key) != 0)
    {
        ZRUB_LOG_ERROR("failed to decrypt\n");
        return false;
    }

    ZRUB_LOG_DEBUG("successfully decrypted data of length: %lu\n", pkt->data_length);
    memcpy(pkt->data, temp, pkt->data_length);

    return true;
}

bool zrub_epacket_decrypt(struct zrub_epacket *pkt, uint8_t *key)
{
    if (!zrub_epacket_decrypt_data(pkt, key))
    {
        ZRUB_LOG_ERROR("failed to decrypt data!\n");
        return false;
    }

    return true;
}

uint8_t zrub_epacket_send(struct zrub_epacket *pkt, int sockfd)
{
    int32_t message_size = ZRUB_PKT_NONCE_LEN + ZRUB_PKT_MACBYTES_LEN + pkt->data_length;
    uint8_t buf[4];
    uint32_t offset = 0;
    int32_t rc = 0;

    zrub_serialize_unsigned_int32(buf, 4, message_size, &offset);

    ZRUB_LOG_DEBUG("%10s: ", "message");
    zrub_bytes_print(buf, 4);

    rc = send(sockfd, buf, 4, 0);
    if (rc != 4)
    {
        ZRUB_LOG_ERROR("failed to send message-size, %d\n", rc);
        return ZRUB_PKT_FAILED_SIZE;
    }

    rc = send(sockfd, pkt->nonce, ZRUB_PKT_NONCE_LEN, 0);
    if (rc != ZRUB_PKT_NONCE_LEN)
    {
        ZRUB_LOG_ERROR("failed to send nonce, %d\n", rc);
        return ZRUB_PKT_FAILED_NONCE;
    }

    ZRUB_LOG_DEBUG("%10s: ", "nonce");
    zrub_bytes_print(pkt->nonce, ZRUB_PKT_NONCE_LEN);

    rc = send(sockfd, pkt->macbytes, ZRUB_PKT_MACBYTES_LEN, 0);
    if (rc != ZRUB_PKT_MACBYTES_LEN)
    {
        ZRUB_LOG_ERROR("failed to send macbytes, %d\n", rc);
        return ZRUB_PKT_FAILED_MACBYTES;
    }

    ZRUB_LOG_DEBUG("%10s: ", "macbytes");
    zrub_bytes_print(pkt->macbytes, ZRUB_PKT_MACBYTES_LEN);

    if (send(sockfd, pkt->data, pkt->data_length, 0) != pkt->data_length)
    {
        ZRUB_LOG_ERROR("failed to send the encrypted data\n");
        return ZRUB_PKT_FAILED_DATA;
    }

    return ZRUB_PKT_SUCCESS;
}

uint8_t zrub_epacket_recv(struct zrub_epacket *pkt, int32_t sockfd)
{
    uint32_t message_size;
    uint8_t buf[4];
    uint32_t offset = 0;
    int32_t rc = 0;

    rc = recv(sockfd, buf, 4, 0);
    if (rc == 0)
    {
        ZRUB_LOG_INFO("client %d closed connection\n", sockfd);

        pkt->data_length = 0;

        return ZRUB_PKT_CLIENT_TERM;
    }
    if (rc == -1)
    {
        ZRUB_LOG_ERROR("failed to receive message size, %d\n", rc);
        return ZRUB_PKT_FAILED_SIZE;
    }

    zrub_deserialize_unsigned_int32(buf, 4, &message_size, &offset);
    ZRUB_LOG_DEBUG("%10s: ", "message");
    zrub_bytes_print(buf, 4);

    if (message_size > ZRUB_PKT_DATA_MAX)
    {
        return ZRUB_PKT_MSG_TOO_LARGE;
    }

    rc = recv(sockfd, pkt->nonce, ZRUB_PKT_NONCE_LEN, 0);
    if (rc != ZRUB_PKT_NONCE_LEN)
    {
        ZRUB_LOG_ERROR("failed receiving nonce, %d\n", rc);
        return ZRUB_PKT_FAILED_NONCE;
    }
    message_size -= rc;

    ZRUB_LOG_DEBUG("%10s: ", "nonce");
    zrub_bytes_print(pkt->nonce, ZRUB_PKT_NONCE_LEN);

    rc = recv(sockfd, pkt->macbytes, ZRUB_PKT_MACBYTES_LEN, 0);
    if (rc != ZRUB_PKT_MACBYTES_LEN)
    {
        ZRUB_LOG_ERROR("failed receiving macbytes, %d\n", rc);
        return ZRUB_PKT_FAILED_MACBYTES;
    }
    message_size -= rc;

    ZRUB_LOG_DEBUG("%10s: ", "macbytes");
    zrub_bytes_print(pkt->macbytes, ZRUB_PKT_MACBYTES_LEN);

    int32_t bytes_read = recv(sockfd, pkt->data, message_size, 0);
    if ((uint32_t)bytes_read != message_size)
    {
        ZRUB_LOG_DEBUG("received %d expected %d\n", bytes_read, message_size);
        return ZRUB_PKT_FAILED_DATA;
    }

    ZRUB_LOG_DEBUG("received data %d bytes\n", bytes_read);
    pkt->data_length = bytes_read;

    return ZRUB_PKT_SUCCESS;
}

uint8_t zrub_epacket_async_send(struct zrub_epacket *pkt, int32_t sockfd, struct zrub_epacket_state *state)
{
    ZRUB_NOT_IMPLEMENTED(0);
    ZRUB_UNUSED(pkt);
    ZRUB_UNUSED(sockfd);
    ZRUB_UNUSED(state);
}

uint8_t zrub_epacket_async_recv(struct zrub_epacket *pkt, int32_t sockfd, struct zrub_epacket_state *state)
{
    // ZRUB_NOT_IMPLEMENTED(0);
    // ZRUB_UNUSED(pkt);
    // ZRUB_UNUSED(sockfd);
    // ZRUB_UNUSED(state);

    uint8_t data[ sizeof(struct zrub_epacket) ] = { 0 };
    int32_t rc = recv(sockfd, data, sizeof(struct zrub_epacket), MSG_DONTWAIT);

    if (rc < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return ZRUB_PKT_NO_DATA_AVAILABLE;
        }

        return ZRUB_PKT_FAILED_RECV;
    }

    if (rc == 0)
    {
        return ZRUB_PKT_CLIENT_TERM;
    }

    uint32_t idx = 0;
    for (; idx < (uint32_t)rc; idx++)
    {
        // TODO - OPTIMIZE THIS!
        if (state->offset < 4)
        {
            ZRUB_LOG_DEBUG("setting %02x as %u\n", data[idx], 8 * (3 - state->offset));
            state->msg_size |= data[idx] << (8 * (3 - state->offset));
            state->offset++;

            if (state->offset == 4)
            {
                ZRUB_LOG_DEBUG("setting data_length as %u\n", state->msg_size);
                pkt->data_length = state->msg_size - 4 - ZRUB_PKT_NONCE_LEN - ZRUB_PKT_MACBYTES_LEN;
            }

            continue;
        }

        // reaches this only after state->msg_size is set!
        if (state->msg_size > ZRUB_PKT_DATA_MAX)
        {
            return ZRUB_PKT_MSG_TOO_LARGE;
        }

        // TODO - OPTIMIZE THIS!
        if (state->offset < 4 + ZRUB_PKT_NONCE_LEN)
        {
            ZRUB_LOG_DEBUG("(%u) setting %02x as nonce %u\n",
                1 + state->offset, data[idx], state->offset - 4);

            pkt->nonce[state->offset - 4] = data[idx];

            state->offset++;
            continue;
        }

        // TODO - OPTIMIZE THIS!
        if (state->offset < 4 + ZRUB_PKT_NONCE_LEN + ZRUB_PKT_MACBYTES_LEN)
        {
            ZRUB_LOG_DEBUG("(%u) setting %02x as macbytes %u\n",
                1 + state->offset, data[idx], state->offset - 4 - ZRUB_PKT_NONCE_LEN);

            pkt->macbytes[state->offset - 4 - ZRUB_PKT_NONCE_LEN] = data[idx];

            state->offset++;
            continue;
        }

        // TODO - OPTIMIZE THIS!
        if (state->offset < 4 + ZRUB_PKT_NONCE_LEN + ZRUB_PKT_MACBYTES_LEN + ZRUB_PKT_DATA_MAX)
        {
            ZRUB_LOG_DEBUG("(%u) setting %02x as data %u\n",
                1 + state->offset, data[idx], state->offset - 4 - ZRUB_PKT_NONCE_LEN - ZRUB_PKT_MACBYTES_LEN);

            pkt->data[state->offset - 4 - ZRUB_PKT_NONCE_LEN - ZRUB_PKT_MACBYTES_LEN] = data[idx];

            state->offset++;
            continue;
        }
    }

    ZRUB_DVAR_BYTES(pkt->nonce, ZRUB_PKT_NONCE_LEN);
    ZRUB_DVAR_BYTES(pkt->macbytes, ZRUB_PKT_MACBYTES_LEN);
    ZRUB_DVAR_BYTES(pkt->data, pkt->data_length);

    if (state->offset != state->msg_size)
    {
        return ZRUB_PKT_AWAITING_DATA;
    }

    return ZRUB_PKT_SUCCESS;
}
