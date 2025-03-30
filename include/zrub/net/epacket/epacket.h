#ifndef __ZRUB_EPACKET_H__
#define __ZRUB_EPACKET_H__

#include "zrub/global.h"
#include "zrub/core/bytes.h"
#include "zrub/serializer/interface.h"

#include <sys/socket.h>
#include <sodium.h>


#define ZRUB_PKT_NONCE_LEN     crypto_box_NONCEBYTES
#define ZRUB_PKT_MACBYTES_LEN  crypto_box_MACBYTES
#define ZRUB_PKT_DATA_MAX      ((int32_t)(1024 - sizeof(int32_t) - ZRUB_PKT_NONCE_LEN - ZRUB_PKT_MACBYTES_LEN))

#define ZRUB_PKT_SUCCESS           0
#define ZRUB_PKT_FAILED_MSG        1
#define ZRUB_PKT_FAILED_NONCE      2
#define ZRUB_PKT_FAILED_MACBYTES   3
#define ZRUB_PKT_FAILED_SIZE       4
#define ZRUB_PKT_FAILED_DATA       5

// size of struct is 1024
struct zrub_epacket
{
    uint8_t             data[ZRUB_PKT_DATA_MAX];
    int32_t             data_length;                        // 4 bytes
    uint8_t             nonce[ZRUB_PKT_NONCE_LEN];          // 24 bytes
    uint8_t             macbytes[ZRUB_PKT_MACBYTES_LEN];    // 16 bytes
};

/**
 * @brief this function encrypts data onto an encrypted packet struct
 *
 * @param pkt           encrypted packet to be set
 * @param data          data to be encrypted
 * @param data_length   data length to be encrypted
 * @param key           256-bit key to encrypt the data with
 *
 * @returns a complete encrypted packet to transmit
 */
bool zrub_epacket_encrypt(struct zrub_epacket *pkt, uint8_t *data, int32_t data_length, uint8_t *key);

/**
 * @brief this function decrypts data of a encrypted packet struct
 *
 * @param pkt           encrypted packet to be set
 * @param key           256-bit key to encrypt the data with
 *
 * @returns a complete decrypted packet to transmit
 */
bool zrub_epacket_decrypt(struct zrub_epacket *pkt, uint8_t *key);

/*
(int32_t) 4 bytes                   size of message to be sent
(uint8 *) ZRUB_PKT_NONCE_LEN        nonce used for the encryption/decryption
(uint8 *) .data_length              at most ZRUB_PKT_DATA_MAX bytes
*/
uint8_t zrub_epacket_send(struct zrub_epacket *pkt, int sockfd);
uint8_t zrub_epacket_recv(struct zrub_epacket *pkt, int sockfd);


#endif // __ZRUB_EPACKET_H__
