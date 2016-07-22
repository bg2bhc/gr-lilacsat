#ifndef __CC_H__
#define __CC_H__

#include <stm32f4xx.h>
#include "viterbi27.h"
#include "randomizer.h"
#include "rs.h"
#include "tab.h"
#include "fifobuf.h"

#include "FreeRTOS.h"
#include "timers.h"

#define	CONFIG_CCSDS_TX_BUFLEN	1024
#define CONFIG_TIMEOUT_TX_WRITE	1000
#define CONFIG_LEN_PREAMBLE			400

#define FLAG_TLM_PRI_L_TX				0x01
#define FLAG_TLM_PRI_H_TX				0x02
#define FLAG_CODEC2_TX					0x04


typedef struct Cc
{
	FIFOBuffer tx_fifo;
	uint8_t tx_buf[CONFIG_CCSDS_TX_BUFLEN];
	uint8_t flag_ctcss;
	uint8_t codec2_bits[7];
	uint8_t i_byte_in_frame;
	uint16_t n_preamble;
	uint8_t tx_bit_state;
	uint8_t encstate;
} Cc;

void cc_init(Cc *cc);
void cc_tx_proc(Cc *cc, unsigned char *symbols, unsigned int nbits);
void cc_send(Cc *cc, uint8_t *message, int length, int pri);

#endif

