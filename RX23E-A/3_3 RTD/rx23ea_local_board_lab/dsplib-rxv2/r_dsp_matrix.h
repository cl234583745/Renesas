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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     
*******************************************************************************/ 

/******************************************************************************* 
* File Name    : r_dsp_matrix.h
* Description  : PUBLIC header file for Matrix arithmetic kernels of DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.05.18  5.00
******************************************************************************/ 

#ifndef R_DSP_MATRIX_H
#define R_DSP_MATRIX_H


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
Matrix API Function prototypes
******************************************************************************/ 
/*---------------------------*/
/*  Matrix Add               */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixAdd_f32f32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_i32i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_i16i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_i16i16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_cf32cf32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_ci32ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_ci16ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixAdd_ci16ci16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);
 
/*---------------------------*/
/*  Matrix Subtract          */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixSub_f32f32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_i32i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_i16i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_i16i16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_cf32cf32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_ci32ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_ci16ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixSub_ci16ci16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output
);

/*---------------------------*/
/*  Matrix Multiply          */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixMul_f32f32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_i32i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_i16i32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_i16i16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_cf32cf32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_ci32ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci32 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci16 (
    const matrix_t * inputA, 
    const matrix_t * inputB, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

/*---------------------------*/
/*  Matrix Transpose         */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixTrans_f32f32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_i32i32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_i16i32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_i16i16 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_cf32cf32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_ci32ci32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_ci16ci32 (
    const matrix_t * input, 
    matrix_t * output
);

r_dsp_status_t R_DSP_MatrixTrans_ci16ci16 (
    const matrix_t * input, 
    matrix_t * output
);


/*---------------------------*/
/*  Matrix Scale             */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixScale_f32f32 (
    const matrix_t * input, 
    const float  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_i32i32 (
    const matrix_t * input, 
    const int32_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_i16i32 (
    const matrix_t * input, 
    const int16_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_i16i16 (
    const matrix_t * input, 
    const int16_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_cf32cf32 (
    const matrix_t * input, 
    const float  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_ci32ci32 (
    const matrix_t * input, 
    const int32_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci32 (
    const matrix_t * input, 
    const int16_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci16 (
    const matrix_t * input, 
    const int16_t  scalar, 
    matrix_t * output,
    scale_t    shift,
    uint16_t   options
);

/******************************************************************************
Matrix Internal Function prototypes
******************************************************************************/
/*---------------------------*/
/*  Matrix Multiply          */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixMul_f32f32_asm(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i32i32_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i32i32_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i32i32_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i32i32_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i32_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i32_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i32_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i32_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i16_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i16_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i16_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_i16i16_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_cf32cf32_asm(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci32ci32_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci32ci32_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci32ci32_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci32ci32_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci32_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci32_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci32_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci32_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci16_asm_nt(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci16_asm_n2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci16_asm_st(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixMul_ci16ci16_asm_s2(
    const matrix_t * inputA,
    const matrix_t * inputB,
    matrix_t * output,
    scale_t    shift
);

/*---------------------------*/
/*  Matrix Scale             */
/*---------------------------*/
r_dsp_status_t R_DSP_MatrixScale_f32f32_asm(
    const matrix_t * input,
    const float  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i32i32_asm_nt(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i32i32_asm_n2(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i32i32_asm_st(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i32i32_asm_s2(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i32_asm_nt(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i32_asm_n2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i32_asm_st(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i32_asm_s2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i16_asm_nt(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i16_asm_n2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i16_asm_st(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_i16i16_asm_s2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_cf32cf32_asm(
    const matrix_t * input,
    const float  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci32ci32_asm_nt(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci32ci32_asm_n2(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci32ci32_asm_st(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci32ci32_asm_s2(
    const matrix_t * input,
    const int32_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci32_asm_nt(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci32_asm_n2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci32_asm_st(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci32_asm_s2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci16_asm_nt(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci16_asm_n2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci16_asm_st(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

r_dsp_status_t R_DSP_MatrixScale_ci16ci16_asm_s2(
    const matrix_t * input,
    const int16_t  scalar,
    matrix_t * output,
    scale_t    shift
);

#endif
/* End of file */


