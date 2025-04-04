// TODO - Implement this in `epacket.c`
#define ZRUBLIB
#define ZRUBLIB_DEBUG
#include <zrub/zrub.h>


uint8_t gkey[32] = { 0 };

uint8_t async_recv(struct zrub_epacket *pkt, uint8_t *data, int32_t data_recv, struct zrub_epacket_state *state)
{
    ZRUB_DVAR_BYTES(data, data_recv);

    uint32_t idx = 0;
    for (; idx < (uint32_t)data_recv; idx++)
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
                pkt->data_length = state->msg_size;
            }

            continue;
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

    return (uint8_t)0;
}

int main()
{
    struct zrub_epacket spkt = { 0 };
    struct zrub_epacket rpkt = { 0 };

    // sender
    ZRUB_LOG_DEBUG("=========== SENDER_BODY =============\n");
    {
        char sdata[] = "hello, world!";
    
        uint8_t serbuf[512];
        struct zrub_serializer ser = {
            .buf = serbuf,
            .bufsize = sizeof(serbuf),
            .offset = 0
        };
    
        zrub_serialize_string(ser.buf, ser.bufsize, sdata, zrub_str_len(sdata), &ser.offset);
        ZRUB_DVAR_BYTES(ser.buf, ser.offset);
    
        zrub_epacket_encrypt(&spkt, ser.buf, ser.offset, gkey);
        ZRUB_DVAR_BYTES(spkt.data, spkt.data_length);
    }

    // simulate receiving one byte at a time
    ZRUB_LOG_DEBUG("========== SIMLUATE_BODY ============\n");    
    {
        struct zrub_epacket_state state = { 0 };

        // send partition of msg_size
        uint8_t msg_size_buf[4] = { 0 };
        uint32_t __offset = 0;

        ZRUB_DVAR_BYTES(msg_size_buf, 4);
        zrub_serialize_unsigned_int32(msg_size_buf, 4, spkt.data_length, &__offset);
        ZRUB_DVAR_BYTES(msg_size_buf, 4);

        // == SEND MSG_SIZE ==
        // 2 bytes
        async_recv(&rpkt, msg_size_buf, 2, &state);
        ZRUB_DVAR_UINT(state.msg_size);

        // 2 bytes
        async_recv(&rpkt, msg_size_buf + 2, 2, &state);
        ZRUB_DVAR_UINT(state.msg_size);

        // == SEND NONCE ==
        ZRUB_DVAR_BYTES(rpkt.nonce, 24);
        ZRUB_DVAR_BYTES(spkt.nonce, 24);

        // 6 bytes
        async_recv(&rpkt, spkt.nonce, 6, &state);
        ZRUB_DVAR_BYTES(rpkt.nonce, 24);
        ZRUB_DVAR_BYTES(spkt.nonce, 24);

        // 18 bytes
        async_recv(&rpkt, spkt.nonce + 6, 18, &state);
        ZRUB_DVAR_BYTES(rpkt.nonce, 24);
        ZRUB_DVAR_BYTES(spkt.nonce, 24);

        // == SEND MACBYTES ==
        ZRUB_DVAR_BYTES(rpkt.macbytes, 24);
        ZRUB_DVAR_BYTES(spkt.macbytes, 24);

        async_recv(&rpkt, spkt.macbytes, 8, &state);
        ZRUB_DVAR_BYTES(rpkt.macbytes, 24);
        ZRUB_DVAR_BYTES(spkt.macbytes, 24);

        async_recv(&rpkt, spkt.macbytes + 8, 5, &state);

        ZRUB_DVAR_BYTES(rpkt.macbytes, 24);
        ZRUB_DVAR_BYTES(spkt.macbytes, 24);
        async_recv(&rpkt, spkt.macbytes + 13, 3, &state);

        ZRUB_DVAR_BYTES(rpkt.macbytes, 24);
        ZRUB_DVAR_BYTES(spkt.macbytes, 24);

        // send all data
        ZRUB_DVAR_BYTES(spkt.data, spkt.data_length);
        async_recv(&rpkt, spkt.data, spkt.data_length, &state);
        ZRUB_DVAR_BYTES(rpkt.data, rpkt.data_length);
    }

    // receiver
    ZRUB_LOG_DEBUG("========== RECEIVER_BODY ============\n");
    {    
        ZRUB_DVAR_BYTES(rpkt.data, rpkt.data_length);

        zrub_epacket_decrypt(&rpkt, gkey);
        ZRUB_DVAR_BYTES(rpkt.data, rpkt.data_length);
    
        struct zrub_serializer des = {
            .buf = rpkt.data,
            .bufsize = rpkt.data_length,
            .offset = 0
        };
        
        char rdata[64] = { 0 };
    
        zrub_deserialize_string(des.buf, des.bufsize, rdata, sizeof(rdata) - 1, &des.offset);
        ZRUB_LOG_DEBUG("Received: %s\n", rdata);
    }

    return 0;
}
