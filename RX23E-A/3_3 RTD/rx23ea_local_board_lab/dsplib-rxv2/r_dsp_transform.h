/******************************************************************************* 
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  
* intended for use with Renesas products. No other uses are authorized. This  
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS  
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the  
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.     
*******************************************************************************/ 

/******************************************************************************* 
* File Name    : r_dsp_transform.h
* Description  : PUBLIC header file for Fourier Transforms kernels of DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.06.04  5.00
******************************************************************************/ 

#ifndef R_DSP_TRANSFORM_H
#define R_DSP_TRANSFORM_H


/****************************************************************************** 
Includes
******************************************************************************/ 
#include "r_dsp_types.h"


/****************************************************************************** 
Macro definitions 
******************************************************************************/ 

/****************************************************************************** 
Typedef definitions 
******************************************************************************/ 

typedef struct 
{ 
    uint16_t n;	// number of points in the transform (e.g. 1024)
    uint16_t options; 
    void	* twiddles; // twiddle factors
    void	* bitrev; // bit-reverse LUT
    void	* work; // working area
    void	* window; // window coefficients
} r_dsp_fft_t; 


/****************************************************************************** 
Linear Transforms (Fourier Transforms) API Function prototypes
******************************************************************************/ 
/*---------------------------*/
/*  Forward Complex DFTs     */
/*---------------------------*/
r_dsp_status_t R_DSP_DFT_ci16ci16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_ci16ci32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_ci32ci32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_cf32cf32 (
    const vector_t * src, 
    vector_t * dst
);

/*---------------------------*/
/*  Inverse Complex DFTs     */
/*---------------------------*/
r_dsp_status_t R_DSP_IDFT_ci16ci16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_ci32ci16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_ci32ci32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_cf32cf32 (
    const vector_t * src, 
    vector_t * dst
);

/*-----------------------------------*/
/*  Forward DFT of Real-Valued Input */
/*-----------------------------------*/
r_dsp_status_t R_DSP_DFT_i16ci16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_i16ci32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_i32ci32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_DFT_f32cf32 (
    const vector_t * src, 
    vector_t * dst
);

/*---------------------------------------------------*/
/*  Inverse Complex-Conjugate-Symmetric Inverse DFTs */
/*---------------------------------------------------*/
r_dsp_status_t R_DSP_IDFT_CCS_ci16i16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_CCS_ci32i16 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_CCS_ci32i32 (
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IDFT_CCS_cf32f32 (
    const vector_t * src, 
    vector_t * dst
);


/*---------------------------------------------------*/
/*  Get FFT Working Buffer Sizes                     */
/*---------------------------------------------------*/

r_dsp_status_t R_DSP_FFT_BufSize_i16ci16 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_i16ci32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_i32ci32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_f32cf32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci16i16 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci32i16 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

#define R_DSP_FFT_BufSize_ci32i32(h,t,r,w) R_DSP_FFT_BufSize_i32ci32(h,t,r,w)


r_dsp_status_t R_DSP_FFT_BufSize_cf32f32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci16ci16 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci16ci32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci32ci16 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_ci32ci32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

r_dsp_status_t R_DSP_FFT_BufSize_cf32cf32 (
    r_dsp_fft_t * h, 
    size_t * numTwiddleBytes, 
    size_t * numBitRevBytes,
    size_t * numWorkBytes
);

/*---------------------------------------------------*/
/*  Initialize FFT Handle                            */
/*---------------------------------------------------*/

r_dsp_status_t R_DSP_FFT_Init_i16ci16 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_i16ci32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_i32ci32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_f32cf32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci16i16 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci32i16 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci32i32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_cf32f32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci16ci16 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci16ci32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci32ci16 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_ci32ci32 (
    r_dsp_fft_t * handle
);

r_dsp_status_t R_DSP_FFT_Init_cf32cf32 (
    r_dsp_fft_t * handle
);

/*---------------------------------------------------*/
/*  Forward Complex FFT                              */
/*---------------------------------------------------*/
r_dsp_status_t R_DSP_FFT_ci16ci16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_ci16ci32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_ci32ci32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_cf32cf32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

/*---------------------------------------------------*/
/*  Inverse Complex FFT                              */
/*---------------------------------------------------*/
r_dsp_status_t R_DSP_IFFT_ci16ci16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_ci32ci16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_ci32ci32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_cf32cf32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

/*---------------------------------------------------*/
/*  FFT of real-valued input                         */
/*---------------------------------------------------*/
r_dsp_status_t R_DSP_FFT_i16ci16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_i16ci32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_i32ci32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_FFT_f32cf32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

/*---------------------------------------------------*/
/*  Inverse Complex-Conjugate-Symmetric Inverse FFTs */
/*---------------------------------------------------*/
r_dsp_status_t R_DSP_IFFT_CCS_ci16i16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_CCS_ci32i16 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_CCS_ci32i32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

r_dsp_status_t R_DSP_IFFT_CCS_cf32f32 (
    r_dsp_fft_t * handle, 
    const vector_t * src, 
    vector_t * dst
);

/******************************************************************************
Linear Transforms (Fourier Transforms) Internal Function prototypes
******************************************************************************/
/*---------------------------------------------------*/
/*  FFT of real-valued input	                     */
/*---------------------------------------------------*/
r_dsp_status_t R_rfft_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);
r_dsp_status_t R_rfft_sc_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);
r_dsp_status_t R_rfft_x2_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);
r_dsp_status_t R_rfft_tw32_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);
r_dsp_status_t R_rfft_sc_tw32_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);
r_dsp_status_t R_rfft_x2_tw32_i16ci16(r_dsp_fft_t *handle, const int16_t *src, cplxi16_t *dst);

r_dsp_status_t R_rfft_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_sc_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_x2_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_tw32_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_sc_tw32_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_x2_tw32_i16ci32(r_dsp_fft_t *handle, const int16_t *src, cplxi32_t *dst);

r_dsp_status_t R_rfft_i32ci32(r_dsp_fft_t *handle, const int32_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_sc_i32ci32(r_dsp_fft_t *handle, const int32_t *src, cplxi32_t *dst);
r_dsp_status_t R_rfft_x2_i32ci32(r_dsp_fft_t *handle, const int32_t *src, cplxi32_t *dst);

r_dsp_status_t R_rfft_f32cf32(r_dsp_fft_t *handle, const float *src, cplxf32_t *dst);

/*  Inverse Complex-Conjugate-Symmetric Inverse FFTs */
r_dsp_status_t R_irfft_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);
r_dsp_status_t R_irfft_sc_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);
r_dsp_status_t R_irfft_x2_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);
r_dsp_status_t R_irfft_tw32_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);
r_dsp_status_t R_irfft_sc_tw32_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);
r_dsp_status_t R_irfft_x2_tw32_ci16i16(r_dsp_fft_t *handle, const cplxi16_t *src, int16_t *dst);

r_dsp_status_t R_irfft_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);
r_dsp_status_t R_irfft_sc_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);
r_dsp_status_t R_irfft_x2_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);
r_dsp_status_t R_irfft_tw32_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);
r_dsp_status_t R_irfft_sc_tw32_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);
r_dsp_status_t R_irfft_x2_tw32_ci32i16(r_dsp_fft_t *handle, const cplxi32_t *src, int16_t *dst);

r_dsp_status_t R_irfft_ci32i32(r_dsp_fft_t *handle, const cplxi32_t *src, int32_t *dst);
r_dsp_status_t R_irfft_sc_ci32i32(r_dsp_fft_t *handle, const cplxi32_t *src, int32_t *dst);
r_dsp_status_t R_irfft_x2_ci32i32(r_dsp_fft_t *handle, const cplxi32_t *src, int32_t *dst);

r_dsp_status_t R_irfft_cf32f32(r_dsp_fft_t *handle, const cplxf32_t *src, float *dst);

/*---------------------------------------------------*/
/*  Forward Complex FFT                              */
/*---------------------------------------------------*/
r_dsp_status_t R_cfft_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_cfft_sc_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_cfft_x2_ci16ci16(r_dsp_fft_t *hadnle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_cfft_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_cfft_sc_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_cfft_x2_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);

r_dsp_status_t R_cfft_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_sc_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_x2_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_tw32_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_sc_tw32_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_x2_tw32_ci16ci32(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi32_t *dst);

r_dsp_status_t R_cfft_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_sc_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);
r_dsp_status_t R_cfft_x2_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);

r_dsp_status_t R_cfft_cf32cf32(r_dsp_fft_t *handle, const cplxf32_t *src, cplxf32_t *dst);

r_dsp_status_t R_cfft16_cf32cf32(const cplxf32_t *src, cplxf32_t *dst);
r_dsp_status_t R_cfft32_cf32cf32(const cplxf32_t *src, cplxf32_t *dst);
r_dsp_status_t R_cfft64_cf32cf32(const cplxf32_t *src, cplxf32_t *dst);

/*---------------------------------------------------*/
/*  Inverse Complex FFT                              */
/*---------------------------------------------------*/
r_dsp_status_t R_icfft_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_sc_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_x2_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_sc_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_x2_tw32_ci16ci16(r_dsp_fft_t *handle, const cplxi16_t *src, cplxi16_t *dst);

r_dsp_status_t R_icfft_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_sc_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_x2_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_tw32_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_sc_tw32_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);
r_dsp_status_t R_icfft_x2_tw32_ci32ci16(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi16_t *dst);

r_dsp_status_t R_icfft_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);
r_dsp_status_t R_icfft_sc_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);
r_dsp_status_t R_icfft_x2_ci32ci32(r_dsp_fft_t *handle, const cplxi32_t *src, cplxi32_t *dst);

r_dsp_status_t R_icfft_cf32cf32(r_dsp_fft_t *handle, const cplxf32_t *src, cplxf32_t *dst);

#endif
/* End of file */


