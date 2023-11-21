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
* File Name    : r_dsp_filters.h
* Description  : PUBLIC header file for filter kernels of DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.07.20  5.00
******************************************************************************/ 

#ifndef R_DSP_FILTERS_H
#define R_DSP_FILTERS_H


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
/*---------------------------*/
/* Generic FIR filter        */
/*---------------------------*/
typedef struct 
{ 
    uint32_t taps;     // number of filter taps 
    void *   coefs;    // pointer to filter coefficients 
    void *   state;    // pointer to filter state data, including the filter痴 delay line  
                       // and any other implementation-dependent state 
    scale_t  scale;    // scaling parameter
    uint32_t options;  // options that specify rounding, saturation, or other behaviors 
} r_dsp_firfilter_t; 

/*---------------------------*/
/* IIR Biquad filter         */
/*---------------------------*/
typedef enum { 
    R_DSP_BIQUAD_FORM_DEFAULT  = 0,
    R_DSP_BIQUAD_FORM_I = 1,
    R_DSP_BIQUAD_FORM_II
  // other forms can be added here in the future. 
}r_dsp_iirbiquad_form_t; 

typedef struct 
{ 
    uint32_t stages;     // number of biquad stages 
    void *   coefs;      // pointer to filter coefficients 
    void *   state;      // pointer to filter痴 internal state (delay line) 
    scale_t  scale;      // scaling parameter
    uint32_t qint;       // number of integer bit (0 or 1) for coefficients 
    uint32_t options;    // options that specify rounding, saturation, or other behaviors 
    uint32_t form;  //which biquad form to use 
} r_dsp_iirbiquad_t; 

/*---------------------------*/
/* Single-pole IIR filter    */
/*---------------------------*/
typedef struct 
{ 
    void     *coefs;   // pointer to filter coefficient
    void     *state;   // pointer to feedback state 
    scale_t  scale;    // scaling parameter
    uint32_t  options; // options that specify rounding, saturation, or other behaviors 
} r_dsp_iirsinglepole_t; 

/****************************************************************************** 
Filter API Function prototypes
******************************************************************************/ 
/*---------------------------*/
/* Generic FIR filter        */
/*---------------------------*/
/* Real input: f32; Real output: f32 */ 
r_dsp_status_t R_DSP_FIR_Init_f32f32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_f32f32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i32; Real output: i32 */ 
r_dsp_status_t R_DSP_FIR_Init_i32i32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_i32i32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i32 */ 
r_dsp_status_t R_DSP_FIR_Init_i16i32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_i16i32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i16 */ 
r_dsp_status_t R_DSP_FIR_Init_i16i16 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_i16i16 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: cf32; Complex output: cf32 */ 
r_dsp_status_t R_DSP_FIR_Init_cf32cf32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_cf32cf32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci32; Complex output: ci32 */ 
r_dsp_status_t R_DSP_FIR_Init_ci32ci32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_ci32ci32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci16; Complex output: ci32 */ 
r_dsp_status_t R_DSP_FIR_Init_ci16ci32 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_ci16ci32 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci16; Complex output: ci16 */ 
r_dsp_status_t R_DSP_FIR_Init_ci16ci16 (
    r_dsp_firfilter_t * handle
);

r_dsp_status_t R_DSP_FIR_ci16ci16 (
    const r_dsp_firfilter_t * handle, 
    const vector_t * input, 
    vector_t * output
);


/*---------------------------*/
/* IIR Biquad filter         */
/*---------------------------*/
/* Real input: f32; Real output: f32 */ 
int32_t R_DSP_IIRBiquad_StateSize_f32f32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_f32f32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_f32f32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i32; Real output: i32 */ 
int32_t R_DSP_IIRBiquad_StateSize_i32i32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_i32i32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i32 */ 
int32_t R_DSP_IIRBiquad_StateSize_i16i32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_i16i32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i16 */ 
int32_t R_DSP_IIRBiquad_StateSize_i16i16 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_i16i16 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: cf32; Complex output: cf32 */ 
int32_t R_DSP_IIRBiquad_StateSize_cf32cf32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_cf32cf32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_cf32cf32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci32; Complex output: ci32 */ 
int32_t R_DSP_IIRBiquad_StateSize_ci32ci32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_ci32ci32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci16; Complex output: ci32 */ 
int32_t R_DSP_IIRBiquad_StateSize_ci16ci32 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_ci16ci32 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Complex input: ci16; Complex output: ci16 */ 
int32_t R_DSP_IIRBiquad_StateSize_ci16ci16 (
    const r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_Init_ci16ci16 (
    r_dsp_iirbiquad_t * handle
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16 (
    const r_dsp_iirbiquad_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/*---------------------------*/
/* Single-pole IIR filter    */
/*---------------------------*/
/* Real input: f32; Real output: f32 */ 
r_dsp_status_t R_DSP_IIRSinglePole_f32f32 (
    const r_dsp_iirsinglepole_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i32; Real output: i32 */ 
r_dsp_status_t R_DSP_IIRSinglePole_i32i32 (
    const r_dsp_iirsinglepole_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i32 */ 
r_dsp_status_t R_DSP_IIRSinglePole_i16i32 (
    const r_dsp_iirsinglepole_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/* Real input: i16; Real output: i16 */ 
r_dsp_status_t R_DSP_IIRSinglePole_i16i16 (
    const r_dsp_iirsinglepole_t * handle, 
    const vector_t * input, 
    vector_t * output
);

/******************************************************************************
Filter Internal Function prototypes
******************************************************************************/
/*---------------------------*/
/* Generic FIR filter        */
/*---------------------------*/
r_dsp_status_t R_DSP_FIR_f32f32_asm(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i16_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i16_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i16_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i16_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_cf32cf32_asm(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#if RX_DSP_CPU == RX_DSP_CPU_V1

r_dsp_status_t R_DSP_FIR_i32i32_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_nt(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_n2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_st(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_s2(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#else /* RX_DSP_CPU != RX_DSP_CPU_V1 */

r_dsp_status_t R_DSP_FIR_i32i32_asm_ntu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_ntd(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_ntn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_n2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_n2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_n2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_stu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_std(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_stn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_s2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_s2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i32i32_asm_s2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_ntu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_ntd(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_ntn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_i16i32_asm_n2u        R_DSP_FIR_i16i32_asm_ntu

r_dsp_status_t R_DSP_FIR_i16i32_asm_n2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_i16i32_asm_n2n        R_DSP_FIR_i16i32_asm_ntn

r_dsp_status_t R_DSP_FIR_i16i32_asm_stu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_std(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_i16i32_asm_stn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_i16i32_asm_s2u        R_DSP_FIR_i16i32_asm_stu

r_dsp_status_t R_DSP_FIR_i16i32_asm_s2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_i16i32_asm_s2n        R_DSP_FIR_i16i32_asm_stn

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_ntu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_ntd(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_ntn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_n2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_n2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_n2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_stu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_std(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_stn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_s2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_s2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci32ci32_asm_s2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_ntu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_ntd(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_ntn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_n2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_n2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_n2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_stu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_std(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_stn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_s2u(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_s2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci16_asm_s2n(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_ntu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_ntd(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_ntn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_ci16ci32_asm_n2u      R_DSP_FIR_ci16ci32_asm_ntu

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_n2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_ci16ci32_asm_n2n      R_DSP_FIR_ci16ci32_asm_ntn

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_stu(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_std(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_stn(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_ci16ci32_asm_s2u      R_DSP_FIR_ci16ci32_asm_stu

r_dsp_status_t R_DSP_FIR_ci16ci32_asm_s2d(
    const r_dsp_firfilter_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_FIR_ci16ci32_asm_s2n      R_DSP_FIR_ci16ci32_asm_stn

#endif /* RX_DSP_CPU */

/*---------------------------*/
/* IIR Biquad filter         */
/*---------------------------*/
#if RX_DSP_CPU == RX_DSP_CPU_V1

r_dsp_status_t R_DSP_IIRBiquad_f32f32_asm(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);


r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_cf32cf32_asm(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s2(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#else /* RX_DSP_CPU != RX_DSP_CPU_V1 */

r_dsp_status_t R_DSP_IIRBiquad_f32f32_asm(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i32i32_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_nt0u R_DSP_IIRBiquad_i16i16_asm_nt0d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_nt1u R_DSP_IIRBiquad_i16i16_asm_nt1d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_n20u R_DSP_IIRBiquad_i16i16_asm_n20d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_n21u R_DSP_IIRBiquad_i16i16_asm_n21d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_st0u R_DSP_IIRBiquad_i16i16_asm_st0d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_st1u R_DSP_IIRBiquad_i16i16_asm_st1d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_s20u R_DSP_IIRBiquad_i16i16_asm_s20d

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i16_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_i16i16_asm_s21u R_DSP_IIRBiquad_i16i16_asm_s21d

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_i16i32_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_cf32cf32_asm(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci32ci32_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_nt0u R_DSP_IIRBiquad_ci16ci16_asm_nt0d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_nt1u R_DSP_IIRBiquad_ci16ci16_asm_nt1d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_n20u R_DSP_IIRBiquad_ci16ci16_asm_n20d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_n21u R_DSP_IIRBiquad_ci16ci16_asm_n21d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_st0u R_DSP_IIRBiquad_ci16ci16_asm_st0d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_st1u R_DSP_IIRBiquad_ci16ci16_asm_st1d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_s20u R_DSP_IIRBiquad_ci16ci16_asm_s20d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci16_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

#define R_DSP_IIRBiquad_ci16ci16_asm_s21u R_DSP_IIRBiquad_ci16ci16_asm_s21d

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_nt1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_n21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st0n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st0u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st0d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st1n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st1u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_st1d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s20n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s20u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s20d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s21n(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s21u(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRBiquad_ci16ci32_asm_s21d(
    const r_dsp_iirbiquad_t * handle,
    const vector_t * input,
    vector_t * output
);
#endif /* RX_DSP_CPU */

/*---------------------------*/
/* Single-pole IIR filter    */
/*---------------------------*/
r_dsp_status_t R_DSP_IIRSinglePole_f32f32_asm(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i32i32_asm_nt(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i32i32_asm_n2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i32i32_asm_st(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i32i32_asm_s2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i16_asm_nt(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i16_asm_n2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i16_asm_st(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i16_asm_s2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i32_asm_nt(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i32_asm_n2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i32_asm_st(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

r_dsp_status_t R_DSP_IIRSinglePole_i16i32_asm_s2(
    const r_dsp_iirsinglepole_t * handle,
    const vector_t * input,
    vector_t * output
);

#endif
/* End of file */
