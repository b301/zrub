#include "user.h"


bool echat_register_user(i32 sockfd, u8 *key, char *name, char *passwd)
{
    // serialize data
    u8 serialized_data[user_registration_request_size];
    echat_epkt opkt = echat_epkt_init_zero;
    opkt.action = ECHAT_ACTION_REGISTRATION_REQUEST;

    {
        pb_ostream_t ostream = pb_ostream_from_buffer(serialized_data, sizeof(serialized_data));
        user_registration_request req;
    
        snprintf(req.name, ECHAT_USER_MAXNAME, "%s", name);
        snprintf(req.passwd, ECHAT_USER_MAXPASSWD, "%s", passwd);

        if (!pb_encode(&ostream, user_registration_request_fields, &req))
        {
            ZRUB_LOG_ERROR("failed to encode request\n");
            return false;
        }

        // create encrypted packet
        if (!echat_encrypt_packet(&opkt, serialized_data, ostream.bytes_written, key))
        {
            ZRUB_LOG_ERROR("failed to encrpyt packet\n");
            return false;
        }
    }

    // serialize packet
    u8 serialized_packet[echat_epkt_size];

    {
        pb_ostream_t ostream = pb_ostream_from_buffer(serialized_packet, sizeof(serialized_packet));

        if (!pb_encode(&ostream, echat_epkt_fields, &opkt))
        {
            ZRUB_LOG_ERROR("failed to serialize packet: %s\n", PB_GET_ERROR(&ostream));
            return false;
        }

        // send data to server
        i32 bytes_sent = send(sockfd, serialized_packet, ostream.bytes_written, 0);

        if (bytes_sent == -1)
        {
            ZRUB_LOG_ERROR("failed sending packet to server\n");
            return false;
        }

        ZRUB_LOG_INFO("sent packet to server (size: %d)\n", bytes_sent);
    }

    // receive response from server
    u8 response_packet[echat_epkt_size];
    echat_epkt ipkt = echat_epkt_init_zero;
    user_registration_response resp = user_registration_response_init_zero;

    {
        i32 bytes_recv = recv(sockfd, response_packet, sizeof(response_packet), 0);

        if (bytes_recv == -1)
        {
            ZRUB_LOG_ERROR("failed recieving packet from server\n");
            return false;
        }

        ZRUB_LOG_INFO("received packet from server (size: %d)\n", bytes_recv);

        pb_istream_t istream = pb_istream_from_buffer(response_packet, bytes_recv);

        // deserialize packet
        if (!pb_decode(&istream, echat_epkt_fields, &ipkt))
        {
            ZRUB_LOG_ERROR("failed to deserialize packet: %s\n", PB_GET_ERROR(&istream));
            return false;
        }
    }

    {
        // decrypt packet
        if (!echat_decrypt_packet(&ipkt, key))
        {
            ZRUB_LOG_ERROR("failed decrypting packet from server\n");
            return false;
        }

        pb_istream_t istream = pb_istream_from_buffer(ipkt.data, ipkt.data_length);

        // deserialize response
        if (!pb_decode(&istream, user_registration_response_fields, &resp))
        {
            ZRUB_LOG_ERROR("failed to deserialize response: %s\n", PB_GET_ERROR(&istream));
            return false;
        }

        return resp.status;
    }


}
