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
* File Name    : r_dsp_complex.h
* Description  : PUBLIC header file for Complex arithmetic kernels of DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.06.14  5.00
******************************************************************************/ 

#ifndef R_DSP_COMPLEX_H
#define R_DSP_COMPLEX_H


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


/****************************************************************************** 
Complex API Function prototypes
******************************************************************************/ 
/*---------------------------*/
/*  Complex Magnitude        */
/*---------------------------*/
/*  Scalar Magnitude         */
int16_t R_DSP_CplxMag_ci16i16 (
    cplxi16_t x
);

int32_t R_DSP_CplxMag_ci16i32 (
    cplxi16_t x
);

int32_t R_DSP_CplxMag_ci32i32 (
    cplxi32_t x
);

float R_DSP_CplxMag_cf32f32 (
    cplxf32_t x
);

int16_t R_DSP_CplxMag_Fast_ci16i16 (
    cplxi16_t x
);

int32_t R_DSP_CplxMag_Fast_ci16i32 (
    cplxi16_t x
);

int32_t R_DSP_CplxMag_Fast_ci32i32 (
    cplxi32_t x
);

float R_DSP_CplxMag_Fast_cf32f32 (
    cplxf32_t x
);

/*  Vector Magnitude         */
r_dsp_status_t R_DSP_VecCplxMag_ci16i16 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_ci16i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_ci32i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_cf32f32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_Fast_ci16i16 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_Fast_ci16i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_Fast_ci32i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMag_Fast_cf32f32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

/*----------------------------*/
/*  Complex Magnitude Squared */
/*----------------------------*/
/*  Scalar Magnitude Squared  */
int16_t R_DSP_CplxMagSquared_ci16i16 (
    cplxi16_t x
);

int32_t R_DSP_CplxMagSquared_ci16i32 (
    cplxi16_t x
);

int32_t R_DSP_CplxMagSquared_ci32i32 (
    cplxi32_t x
);

float R_DSP_CplxMagSquared_cf32f32 (
    cplxf32_t x
);

/*  Vector Magnitude Squared */
r_dsp_status_t R_DSP_VecCplxMagSquared_ci16i16 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMagSquared_ci16i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMagSquared_ci32i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxMagSquared_cf32f32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);


/*----------------------------*/
/*  Complex Phase             */
/*----------------------------*/
/*    Scalar Complex Phase    */
int16_t R_DSP_CplxPhase_ci16i16 (
    cplxi16_t x
);

int32_t R_DSP_CplxPhase_ci32i32 (
    cplxi32_t x
);

float R_DSP_CplxPhase_cf32f32 (
    cplxf32_t x
);

/*    Vector Complex Phase    */
r_dsp_status_t R_DSP_VecCplxPhase_ci16i16 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxPhase_ci32i32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

r_dsp_status_t R_DSP_VecCplxPhase_cf32f32 (
    const vector_t * input, 
    vector_t * output, 
    const uint32_t N
);

/*---------------------------*/
/*  Complex Add              */
/*---------------------------*/
r_dsp_status_t R_DSP_ComplexAdd_ci16ci16 (
    const cplxi16_t inputA, 
    const cplxi16_t inputB, 
    cplxi16_t * sum
);

r_dsp_status_t R_DSP_ComplexAdd_ci32ci32 (
    const cplxi32_t inputA, 
    const cplxi32_t inputB, 
    cplxi32_t * sum
);

r_dsp_status_t R_DSP_ComplexAdd_cf32cf32 (
    const cplxf32_t inputA, 
    const cplxf32_t inputB, 
    cplxf32_t * sum
);

/*---------------------------*/
/*  Complex Subtract         */
/*---------------------------*/
r_dsp_status_t R_DSP_ComplexSub_ci16ci16 (
    const cplxi16_t inputA, 
    const cplxi16_t inputB, 
    cplxi16_t * diff
);

r_dsp_status_t R_DSP_ComplexSub_ci32ci32 (
    const cplxi32_t inputA, 
    const cplxi32_t inputB, 
    cplxi32_t * diff
);

r_dsp_status_t R_DSP_ComplexSub_cf32cf32 (
    const cplxf32_t inputA, 
    const cplxf32_t inputB, 
    cplxf32_t * diff
);

/*---------------------------*/
/*  Complex Multiply         */
/*---------------------------*/
r_dsp_status_t R_DSP_ComplexMul_ci16ci16 (
    const cplxi16_t inputA, 
    const cplxi16_t inputB, 
    cplxi16_t * product
);
r_dsp_status_t R_DSP_ComplexMul_ci16ci32 (
    const cplxi16_t inputA, 
    const cplxi16_t inputB, 
    cplxi32_t * product
);

r_dsp_status_t R_DSP_ComplexMul_ci32ci32 (
    const cplxi32_t inputA, 
    const cplxi32_t inputB, 
    cplxi32_t * product
);

r_dsp_status_t R_DSP_ComplexMul_cf32cf32 (
    const cplxf32_t inputA, 
    const cplxf32_t inputB, 
    cplxf32_t * product
);

/*---------------------------*/
/*  Complex Conjugate        */
/*---------------------------*/
/*  Scalar Version           */
r_dsp_status_t R_DSP_ComplexConjg_ci16ci16 (
    const cplxi16_t input, 
    cplxi16_t * output 
);

r_dsp_status_t R_DSP_ComplexConjg_ci32ci32 (
    const cplxi32_t input, 
    cplxi32_t * output 
);

r_dsp_status_t R_DSP_ComplexConjg_cf32cf32 (
    const cplxf32_t input, 
    cplxf32_t * output 
);

/*  Vector Version           */
r_dsp_status_t R_DSP_VecCplxConjg_ci16ci16 (
    const vector_t * input, 
    vector_t * output 
);

r_dsp_status_t R_DSP_VecCplxConjg_ci16ci32 (
    const vector_t * input, 
    vector_t * output 
);

r_dsp_status_t R_DSP_VecCplxConjg_ci32ci32 (
    const vector_t * input, 
    vector_t * output 
);

r_dsp_status_t R_DSP_VecCplxConjg_cf32cf32 (
    const vector_t * input, 
    vector_t * output 
);


#endif
/* End of file */


