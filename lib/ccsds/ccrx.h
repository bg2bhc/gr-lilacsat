#ifndef __CCRX_H__
#define __CCRX_H__

typedef void (*sync_hook_t)(uint8_t *, uint16_t, int16_t byte_corr, void *obj_ptr);

#define LENTH_BUF_SYNC_OUT 1024

typedef struct CcRx
{
	uint8_t	d_rx_bit_state;
	uint8_t	d_n_out;
    uint8_t	d_mask_bit_out;
    uint8_t	d_buf_sync_out[LENTH_BUF_SYNC_OUT];
} CcRx;






#endif



