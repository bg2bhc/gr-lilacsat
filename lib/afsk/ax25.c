// This file is modified from BeRTOS.

#include "ax25.h"
#include "crc_ccitt.h"
#include "bertos.h"


//#define printf(...) ((void)0)

#include <string.h> //memset, memcmp
#include <ctype.h>  //isalnum, toupper
#include <stdio.h>  //isalnum, toupper

#define DECODE_CALL(buf, addr) \
{ \
	unsigned i; \
	for (i = 0; i < sizeof((addr)); i++) \
	{ \
		char c = (*(buf)++ >> 1); \
		(addr)[i] = (c == ' ') ? '\x0' : c; \
	} \
}


static void ax25_decode(AX25Ctx *ctx)
{
	AX25Msg msg;
	uint8_t *buf = ctx->buf;
	
	DECODE_CALL(buf, msg.dst.call);
	msg.dst.ssid = (*buf++ >> 1) & 0x0F;

	DECODE_CALL(buf, msg.src.call);
	msg.src.ssid = (*buf >> 1) & 0x0F;

//	LOG_INFO("SRC[%.6s-%d], DST[%.6s-%d]\n", msg.src.call, msg.src.ssid, msg.dst.call, msg.dst.ssid);

	/* Repeater addresses */
	#if CONFIG_AX25_RPT_LST
		for (msg.rpt_cnt = 0; !(*buf++ & 0x01) && (msg.rpt_cnt < countof(msg.rpt_lst)); msg.rpt_cnt++)
		{
			DECODE_CALL(buf, msg.rpt_lst[msg.rpt_cnt].call);
			msg.rpt_lst[msg.rpt_cnt].ssid = (*buf >> 1) & 0x0F;
//			LOG_INFO("RPT%d[%.6s-%d]\n", msg.rpt_cnt, msg.rpt_lst[msg.rpt_cnt].call, msg.rpt_lst[msg.rpt_cnt].ssid);
		}
	#else
		while (!(*buf++ & 0x01))
		{
			char rpt[6];
			uint8_t ssid;
			DECODE_CALL(buf, rpt);
			ssid = (*buf >> 1) & 0x0F;
			LOG_INFO("RPT[%.6s-%d]\n", rpt, ssid);
		}
	#endif

	msg.ctrl = *buf++;
	if (msg.ctrl != AX25_CTRL_UI)
	{
//		LOG_WARN("Only UI frames are handled, got [%02X]\n", msg.ctrl);
		return;
	}

	msg.pid = *buf++;
	if (msg.pid != AX25_PID_NOLAYER3)
	{
//		LOG_WARN("Only frames without layer3 protocol are handled, got [%02X]\n", msg.pid);
		return;
	}

	msg.len = ctx->frm_len - 2 - (buf - ctx->buf);
	msg.info = buf;
//	LOG_INFO("DATA: %.*s\n", msg.len, msg.info);
	
	if (ctx->hook)
    {
        int j;

        fprintf(stdout, "%.6s", msg.src.call);
        fprintf(stdout, "-%d>", msg.src.ssid);

        fprintf(stdout, "%.6s", msg.dst.call);
        fprintf(stdout, "-%d", msg.dst.ssid);

        for(j=0; j<msg.rpt_cnt; j++)
        {
            fprintf(stdout, ","); 
            fprintf(stdout, "%.6s", msg.rpt_lst[j].call);
            fprintf(stdout, "-%d", msg.rpt_lst[j].ssid);
        }
   
        fprintf(stdout, ":");  
        fprintf(stdout, "\n");

		ctx->hook(ctx->obj_ptr, &msg);
    }
}


/**
 * Check if there are any AX25 messages to be processed.
 * This function read available characters from the medium and search for
 * any AX25 messages.
 * If a message is found it is decoded and the linked callback executed.
 * This function may be blocking if there are no available chars and the KFile
 * used in \a ctx to access the medium is configured in blocking mode.
 *
 * \param ctx AX25 context to operate on.
 */
void ax25_poll(AX25Ctx *ctx)
{
	int c;
	
	while ((c = kfile_getc(ctx->ch)) != EOF)
	{
		if (!ctx->escape && c == HDLC_FLAG)
		{
			if (ctx->frm_len >= AX25_MIN_FRAME_LEN)
			{
				if (ctx->crc_in == AX25_CRC_CORRECT)
				{
//					LOG_INFO("Frame found!\n");
					ax25_decode(ctx);
				}
				else
				{
//					LOG_INFO("CRC error, computed [%04X]\n", ctx->crc_in);
				}
			}
			ctx->sync = true;
			ctx->crc_in = CRC_CCITT_INIT_VAL;
			ctx->frm_len = 0;
			continue;
		}

		if (!ctx->escape && c == HDLC_RESET)
		{
//			LOG_INFO("HDLC reset\n");
			ctx->sync = false;
			continue;
		}

		if (!ctx->escape && c == AX25_ESC)
		{
			ctx->escape = true;
			continue;
		}

		if (ctx->sync)
		{
			if (ctx->frm_len < CONFIG_AX25_FRAME_BUF_LEN)
			{
				ctx->buf[ctx->frm_len++] = c;
				ctx->crc_in = updcrc_ccitt(c, ctx->crc_in);
			}
			else
			{
//				LOG_INFO("Buffer overrun");
				ctx->sync = false;
			}
		}
		ctx->escape = false;
	}

	if (kfile_error(ctx->ch))
	{
//		LOG_ERR("Channel error [%04x]\n", kfile_error(ctx->ch));
		kfile_clearerr(ctx->ch);
	}
}


void ax25_putchar(AX25Ctx *ctx, uint8_t c)
{
	if (c == HDLC_FLAG || c == HDLC_RESET
		|| c == AX25_ESC)
		kfile_putc(AX25_ESC, ctx->ch);
	ctx->crc_out = updcrc_ccitt(c, ctx->crc_out);
	kfile_putc(c, ctx->ch);
}

static void ax25_sendCall(AX25Ctx *ctx, const AX25Call *addr, bool last)
{
	uint32_t i;
	uint32_t len;
	uint8_t ssid;

//	if(sizeof(addr->call) > strlen(addr->call)) len = strlen(addr->call);
//	else len = sizeof(addr->call);
		len = MIN(sizeof(addr->call), strlen(addr->call));

	for (i = 0; i < len; i++)
	{
		uint8_t c = addr->call[i];
//		ASSERT(isalnum(c) || c == ' ');
		c = toupper(c);
		ax25_putchar(ctx, c << 1);
	}

	/* Fill with spaces the rest of the CALL if it's shorter */
	if (len < sizeof(addr->call))
		for (i = 0; i < sizeof(addr->call) - len; i++)
			ax25_putchar(ctx, ' ' << 1);

	/* The bit0 of last call SSID should be set to 1 */
	ssid = addr->ssid << 1 | (last ? 0x01 : 0);
	ax25_putchar(ctx, ssid);
}

/**
 * Send an AX25 frame on the channel through a specific path.
 * \param ctx AX25 context to operate on.
 * \param path An array of callsigns used as path, \see AX25_PATH for
 *        an handy way to create a path.
 * \param path_len callsigns path lenght.
 * \param _buf payload buffer.
 * \param len length of the payload.
 */
void ax25_sendVia(AX25Ctx *ctx, const AX25Call *path, uint32_t path_len, const void *_buf, uint32_t len)
{
	size_t i;
	uint8_t crcl;
	uint8_t crch;

	const uint8_t *buf = (const uint8_t *)_buf;
//	ASSERT(path);
//	ASSERT(path_len >= 2);

	ctx->crc_out = CRC_CCITT_INIT_VAL;
	kfile_putc(HDLC_FLAG, ctx->ch);


	/* Send call */
	for (i = 0; i < path_len; i++)
		ax25_sendCall(ctx, &path[i], (i == path_len - 1));

	ax25_putchar(ctx, AX25_CTRL_UI);
	ax25_putchar(ctx, AX25_PID_NOLAYER3);

	while (len--)
		ax25_putchar(ctx, *buf++);

	/*
	 * According to AX25 protocol,
	 * CRC is sent in reverse order!
	 */
	crcl = (ctx->crc_out & 0xff) ^ 0xff;
	crch = (ctx->crc_out >> 8) ^ 0xff;
	ax25_putchar(ctx, crcl);
	ax25_putchar(ctx, crch);

//	ASSERT(ctx->crc_out == AX25_CRC_CORRECT);

	kfile_putc(HDLC_FLAG, ctx->ch);
}

static void print_call(KFile *ch, const AX25Call *call)
{
	printf("%.6s", call->call);
	if (call->ssid)
	{
		printf("-%d", call->ssid);
	}
}

/**
 * Print a AX25 message in TNC-2 packet monitor format.
 * \param ch a kfile channel where the message will be printed.
 * \param msg the message to be printed.
 */
void ax25_print(KFile *ch, const AX25Msg *msg)
{
	int i;

	print_call(ch, &msg->src);
	kfile_putc('>', ch);
	print_call(ch, &msg->dst);

	#if CONFIG_AX25_RPT_LST
	for (i = 0; i < msg->rpt_cnt; i++)
	{
		kfile_putc(',', ch);
		print_call(ch, &msg->rpt_lst[i]);
		// TODO: add * to the trasmitting digi
	}
	#endif

	printf(":%.*s\n", msg->len, msg->info);
}


/**
 * Init the AX25 protocol decoder.
 *
 * \param ctx AX25 context to init.
 * \param channel Used to gain access to the physical medium
 * \param hook Callback function called when a message is received
 */
void ax25_init(AX25Ctx *ctx, KFile *channel, void *obj_ptr, ax25_callback_t hook)
{
//	ASSERT(ctx);
//	ASSERT(channel);

	memset(ctx, 0, sizeof(*ctx));
	ctx->ch = channel;
	ctx->hook = hook;
	ctx->crc_in = ctx->crc_out = CRC_CCITT_INIT_VAL;
	ctx->obj_ptr = obj_ptr;
}



