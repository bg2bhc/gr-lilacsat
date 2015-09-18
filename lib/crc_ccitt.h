// This file is modified from BeRTOS.

#ifndef __CRC_CCITT_H__
#define __CRC_CCITT_H__

#include "bertos.h"


/* CRC table */
extern const uint16_t crc_ccitt_tab[256];



/** CRC-CCITT init value */
#define CRC_CCITT_INIT_VAL ((uint16_t)0xFFFF)

inline uint16_t updcrc_ccitt(uint8_t c, uint16_t oldcrc);

#endif
