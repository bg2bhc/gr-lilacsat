#ifndef __DSLWP_TM_HEADER_H__
#define __DSLWP_TM_HEADER_H__
/*
typedef struct __attribute__((__packed__))
{
	unsigned int version_number			: 2;
	unsigned int spacecraft_id			: 10;
	unsigned int virtual_channel_id			: 3;
	unsigned int ab_flag				: 1;
	unsigned int master_frame_count			: 8;
	unsigned int virtual_channel_frame_count	: 8;
	unsigned int first_header_pointer		: 8;
} tm_header_t;
*/

typedef struct __attribute__((__packed__))
{
	unsigned int first_header_pointer		: 8;
	unsigned int virtual_channel_frame_count	: 8;
	unsigned int master_frame_count			: 8;
	unsigned int ab_flag				: 1;
	unsigned int virtual_channel_id			: 3;
	unsigned int spacecraft_id			: 10;
	unsigned int version_number			: 2;
} tm_header_t;


#define LEN_TM_HEADER 	sizeof(tm_header_t)

#endif


