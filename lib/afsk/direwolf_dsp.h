
/* dsp.h */

// TODO:  put prefixes on these names.

#ifndef __DIREWOLF_DSP_H__
#define __DIREWOLF_DSP_H__

typedef enum bp_window_e { BP_WINDOW_TRUNCATED,
				BP_WINDOW_COSINE,
				BP_WINDOW_HAMMING,
				BP_WINDOW_BLACKMAN,
				BP_WINDOW_FLATTOP } bp_window_t;

float window (bp_window_t type, int size, int j);

int gen_lowpass (float fc, float *lp_filter, int filter_size, bp_window_t wtype, float lp_delay_fract);

void gen_bandpass (float f1, float f2, float *bp_filter, int filter_size, bp_window_t wtype);

void gen_ms (int fc, int samples_per_sec, float *sin_table, float *cos_table, int filter_size, int wtype);

#define MAX_FILTER_SIZE 320		/* 304 is needed for profile C, 300 baud & 44100. */


#endif
