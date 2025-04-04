/*
 * This file has been modified from its original release by Barry Duncan to comment out header includes for
 * 'config.h' and 'libavutil/mem.h', which are both part of FFmpeg but are not present in this project. This
 * file has also been modified to add an include for 'FFmpeg_includes.h' which includes the functionality from
 * the removed header files this file is otherwise identical to the FFmpeg release.
 */

/*
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 * Copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_FFT_H
#define AVCODEC_FFT_H

#ifndef CONFIG_FFT_FLOAT
	#define CONFIG_FFT_FLOAT 1
#endif

#include <stdint.h>
//#include "config.h"
//#include "libavutil/mem.h"
#include "FFmpeg_includes.h"

#if CONFIG_FFT_FLOAT

#include "avfft.h"

#define FFT_NAME(x) x

typedef float FFTDouble;

#else

#define FFT_NAME(x) x ## _fixed

typedef int16_t FFTSample;
typedef int     FFTDouble;

typedef struct FFTComplex
{
	int16_t re, im;
} FFTComplex;

typedef struct FFTContext FFTContext;

#endif /* CONFIG_FFT_FLOAT */

typedef struct FFTDComplex
{
	FFTDouble re, im;
} FFTDComplex;

/* FFT computation */

struct FFTContext
{
	int nbits;
	int inverse;
	uint16_t* revtab;
	FFTComplex* tmp_buf;
	int mdct_size; /* size of MDCT (i.e. number of input data * 2) */
	int mdct_bits; /* n = 2^nbits */
	/* pre/post rotation tables */
	FFTSample* tcos;
	FFTSample* tsin;
	/**
	 * Do the permutation needed BEFORE calling fft_calc().
	 */
	void ( *fft_permute )( struct FFTContext* s, FFTComplex* z );
	/**
	 * Do a complex FFT with the parameters defined in ff_fft_init(). The
	 * input data must be permuted before. No 1.0/sqrt(n) normalization is done.
	 */
	void ( *fft_calc )( struct FFTContext* s, FFTComplex* z );
	void ( *imdct_calc )( struct FFTContext* s, FFTSample* output, const FFTSample* input );
	void ( *imdct_half )( struct FFTContext* s, FFTSample* output, const FFTSample* input );
	void ( *mdct_calc )( struct FFTContext* s, FFTSample* output, const FFTSample* input );
	void ( *mdct_calcw )( struct FFTContext* s, FFTDouble* output, const FFTSample* input );
	int fft_permutation;
#define FF_FFT_PERM_DEFAULT   0
#define FF_FFT_PERM_SWAP_LSBS 1
#define FF_FFT_PERM_AVX       2
	int mdct_permutation;
#define FF_MDCT_PERM_NONE       0
#define FF_MDCT_PERM_INTERLEAVE 1
};

#if CONFIG_HARDCODED_TABLES
	#define COSTABLE_CONST const
#else
	#define COSTABLE_CONST
#endif

#define COSTABLE(size) \
    COSTABLE_CONST DECLARE_ALIGNED(32, FFTSample, FFT_NAME(ff_cos_##size))[size/2]

extern COSTABLE( 16 );
extern COSTABLE( 32 );
extern COSTABLE( 64 );
extern COSTABLE( 128 );
extern COSTABLE( 256 );
extern COSTABLE( 512 );
extern COSTABLE( 1024 );
extern COSTABLE( 2048 );
extern COSTABLE( 4096 );
extern COSTABLE( 8192 );
extern COSTABLE( 16384 );
extern COSTABLE( 32768 );
extern COSTABLE( 65536 );
extern COSTABLE_CONST FFTSample* const FFT_NAME( ff_cos_tabs )[17];

#define ff_init_ff_cos_tabs FFT_NAME(ff_init_ff_cos_tabs)

/**
 * Initialize the cosine table in ff_cos_tabs[index]
 * @param index index in ff_cos_tabs array of the table to initialize
 */
void ff_init_ff_cos_tabs( int index );

#define ff_fft_init FFT_NAME(ff_fft_init)
#define ff_fft_end  FFT_NAME(ff_fft_end)

/**
 * Set up a complex FFT.
 * @param nbits           log2 of the length of the input array
 * @param inverse         if 0 perform the forward transform, if 1 perform the inverse
 */
int ff_fft_init( FFTContext* s, int nbits, int inverse );

#if CONFIG_FFT_FLOAT
	void ff_fft_init_altivec( FFTContext* s );
	void ff_fft_init_mmx( FFTContext* s );
	void ff_fft_init_arm( FFTContext* s );
#else
	void ff_fft_fixed_init_arm( FFTContext* s );
#endif

void ff_fft_end( FFTContext* s );

#define ff_mdct_init FFT_NAME(ff_mdct_init)
#define ff_mdct_end  FFT_NAME(ff_mdct_end)

int ff_mdct_init( FFTContext* s, int nbits, int inverse, double scale );
void ff_mdct_end( FFTContext* s );

#endif /* AVCODEC_FFT_H */
