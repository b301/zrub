#include "pkt.h"


static inline bool echat_encrypt_data(echat_epkt *pkt, u8 *data, u32 data_length, u8* key)
{
    /* generate random nonce */
    u8 nonce[ECHAT_PKT_NONCE_LEN];
    randombytes(nonce, ECHAT_PKT_NONCE_LEN);

    /* print key for debug */
    {
        #ifdef ZRUBLIB_DEBUG
        char hex_key[128];
        sodium_bin2hex(hex_key, 128, key, 32);
        ZRUB_LOG_DEBUG("key: %s\n", hex_key);

        #endif
    }

    if (crypto_secretbox_easy(pkt->data, data, data_length, nonce, key) != 0)
    {
        ZRUB_LOG_ERROR("failed to encrypt");
        return false;
    }

    ZRUB_LOG_DEBUG("successfully encrypted data of length %lu\n", data_length);

    #ifdef ECHAT_EPKT_DEBUG
    {
        /* print nonce */
        printf("nonce: ");
        for (u32 i = 0; i < ECHAT_PKT_NONCE_LEN; i++)
        {
            printf("%02x ", nonce[i]);
        }

        printf("\n");

        /* print plain data */     
        printf("decrypted: ");   
        for (u32 i = 0; i < data_length; i++)
        {
            printf("%02x ", data[i]);
        }

        printf("\n");
        
        /* print encrypted data */
        printf("encrypted: ");
        for (u32 i = 0; i < data_length; i++)
        {
            printf("%02x ", pkt->data[i]);
        }

        printf("\n");
    }
    #endif

    memcpy(pkt->nonce, nonce, ECHAT_PKT_NONCE_LEN);

    return true;
}

/**
 * @brief this function encrypts data onto an echat encrypted packet struct
 * 
 * @param pkt           echat encrypted packet to be set
 * @param data          data to be encrypted
 * @param data_length   data length to be encrypted
 * @param key           256-bit key to encrypt the data with
 * 
 * @note data_length must be less or equal 1000(DATABUFFER) - 16(MACBYTES) = 984!!
 * @returns a complete encrypted packet to transmit
 */
bool echat_encrypt_packet(echat_epkt *pkt, u8 *data, u32 data_length, u8 *key)
{
    if (data_length > ECHAT_PKT_DATA_MAX)
    {
        ZRUB_LOG_ERROR("data length must be less or equal to 984, current: %lu\n", data_length);
        return false;
    }

    if (!echat_encrypt_data(pkt, data, data_length, key))
    {
        ZRUB_LOG_ERROR("failed to encrypt data\n");
        return false;
    }

    /* the size of the actual data (max 984 bytes) */
    pkt->data_length = data_length;

    return true;
}


static inline bool echat_decrypt_data(echat_epkt *pkt, u8 *key)
{
    // MACBYTES appended into the encrypted bytes
    u8 temp[ECHAT_PKT_DATA_MAX + crypto_box_MACBYTES];

    for (u32 i = 0; i < ECHAT_PKT_DATA_MAX; i++)
    {
        temp[i] = 0;
    }

    /* print key for debug */
    {
        #ifdef ZRUBLIB_DEBUG
        char hex_key[128];
        sodium_bin2hex(hex_key, 128, key, 32);
        ZRUB_LOG_DEBUG("key: %s\n", hex_key);
        
        #endif
    }
    
    #ifdef ECHAT_EPKT_DEBUG
    {
        /* print nonce */
        printf("nonce: ");
        for (u32 i = 0; i < ECHAT_PKT_NONCE_LEN; i++)
        {
            printf("%02x ", pkt->nonce[i]);
        }
    
        printf("\n");
    
        /* print encrypted data */
        printf("encrypted: ");
        for (u32 i = 0; i < pkt->data_length; i++)
        {
            printf("%02x ", pkt->data[i]);
        }
    
        printf("\n");
    }
    #endif

    /* pkt->data_length is at most (984) */
    if (crypto_secretbox_open_easy(temp, pkt->data, pkt->data_length + crypto_box_MACBYTES, pkt->nonce, key) != 0)
    {
        ZRUB_LOG_ERROR("failed to decrypt\n");
        return false;
    }

    #ifdef ECHAT_EPKT_DEBUG
    {
        /* print decrpyted data */
        printf("decrypted: ");
        for (u32 i = 0; i < pkt->data_length; i++)
        {
            printf("%02x ", temp[i]);
        }

        printf("\n");
    }
    #endif

    // ZRUB_LOG_DEBUGk("successfully decrypted data of length: %lu\n", pkt->data_length);
    // ZRUB_LOG_DEBUG("decrypted data is `%s`\n", (char*)temp);

    memcpy(pkt->data, temp, pkt->data_length);

    return true;
}

bool echat_decrypt_packet(echat_epkt *pkt, u8 *key)
{
    if (!echat_decrypt_data(pkt, key))
    {
        ZRUB_LOG_ERROR("failed to decrypt data!\n");
        return false;
    }

    return true;
}
