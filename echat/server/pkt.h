#ifndef ECHAT_PKT_H
#define ECHAT_PKT_H

#include "server_headers.h"
#include "proto/secure.pb.h"

/**
 * @file epkt.h
 * @author Oded Zrubavel
 * 
 * @details eChat Packet
 */

// MAKE SURE THIS IS ONPAR WITH 
 #define ECHAT_PKT_NONCE_LEN     24
#define ECHAT_PKT_DATA_MAX      975

bool echat_encrypt_packet(echat_epkt *pkt, u8 *data, u32 data_length, u8 *key);
bool echat_decrypt_packet(echat_epkt *pkt, u8 *key);


#endif // ECHAT_PKT_H
