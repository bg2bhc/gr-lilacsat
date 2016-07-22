/* Copyright 1994 Phil Karn, KA9Q
 * May be used under the terms of the GNU Public License
 */

#ifndef __VITERBI27_H__
#define __VITERBI27_H__

/* The two generator polynomials for the NASA Standard K=7 rate 1/2 code. */
#define	POLYA	0x6d
#define	POLYB	0x4f


/* This parameter sizes the path memory in bits, which is organized as a
 * circular buffer through which we periodically "trace back" to
 * produce the decoded data. PATHMEM must be greater than
 * MERGEDIST+TRACECHUNK, and for efficiency it should also be a power of 2.
 * Don't make it *too* large, or it will spill out of the CPU's on-chip cache
 * and decrease performance. Each bit of path memory costs 8 bytes for the
 * K=7 code.
 */
#define PATHMEM		128

/* In theory, a Viterbi decoder is true maximum likelihood only if
 * the path memory is as long as the entire message and a single traceback
 * is made from the terminal state (usually zero) after the entire message
 * is received.
 *
 * In practice, performance is essentially optimum as long as decoding
 * decisions are deferred by at least 4-5 constraint lengths (28-35 bits
 * for K=7) from the most recently received symbols. MERGEDIST sets this
 * parameter. We give ourselves some margin here in case the code is
 * punctured (which slows merging) and also to let us start each traceback
 * from an arbitrary current state instead of taking the time to find the
 * path with the highest current metric.
 */
#define	MERGEDIST	64	/* Distance to trace back before decoding */

/* Since each traceback is costly (thanks to the overhead of having to
 * go back MERGEDIST bits before we produce our first decoded bit) we'd like
 * to decode as many bits as possible per traceback at the expense of
 * increased decoding delay. TRACECHUNK sets how many bits to
 * decode on each traceback. Since output is produced in 8-bit bytes,
 * TRACECHUNK MUST be a multiple of 8.
 */
#define	TRACECHUNK	8	/* How many bits to decode on each traceback */

/* The path metrics need to be periodicially adjusted downward
 * to prevent an integer overflow that could cause the signed comparisons
 * in the butterfly macros to fail.
 *
 * It's possible to code the comparisons to work in modulo fashion, e.g.,
 * as 'if((a-b) > 0)' rather than 'if(a >b)'. A good optimizer would generate
 * code like 'cmp a,b;js foo' for this, but GCC doesn't.
 *
 * This constant should be larger than the maximum path metric spread.
 * Experimentally this seems to be 2040, which is probably related to the
 * free distance of the code (10) and the symbol metric scale (0-255).
 */
#define	RENORMALIZE	10000

#if (TRACECHUNK + MERGEDIST > PATHMEM)
#error "TRACECHUNK + MERGEDIST > PATHMEM"
#endif

#if ((TRACECHUNK % 8) != 0)
#error "TRACECHUNK not multiple of 8"
#endif


typedef struct v27
{
    long cmetric[64];
    long nmetric[64];
    unsigned long paths[2*PATHMEM];
    unsigned int pi;
    unsigned long dec;
    int mets[4];
} v27;

void encode27(unsigned char *encstate, unsigned char *symbols, unsigned char *data, unsigned int nbytes);
void vitfilt27_init(v27 *vi);
void vitfilt27_decode(v27 *vi, unsigned char *syms, unsigned char *data, unsigned int nbits);

#endif

