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
* File Name    : r_dsp_statistical.h
* Description  : PUBLIC header file for statistical arithmetic kernels of DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.07.10  5.00
******************************************************************************/ 

#ifndef R_DSP_STATISTICAL_H
#define R_DSP_STATISTICAL_H


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
Statistical API Function prototypes
******************************************************************************/ 
/*---------------------------*/
/*  Mean Value               */
/*---------------------------*/
r_dsp_status_t R_DSP_Mean_f32 (
    const vector_t * input, 
    float * mean
);

r_dsp_status_t R_DSP_Mean_i32 (
    const vector_t * input, 
    int32_t * mean
);

r_dsp_status_t R_DSP_Mean_i16 (
    const vector_t * input, 
    int16_t * mean
);

/*-----------------------------*/
/*  Minimum and Maximum Value  */
/*-----------------------------*/
r_dsp_status_t R_DSP_Min_f32 (
    const vector_t * input, 
    float * min
);

r_dsp_status_t R_DSP_Max_f32 (
    const vector_t * input, 
    float * max
);

r_dsp_status_t R_DSP_Min_i32 (
    const vector_t * input, 
    int32_t * min
);

r_dsp_status_t R_DSP_Max_i32 (
    const vector_t * input, 
    int32_t * max
);

r_dsp_status_t R_DSP_Min_i16 (
    const vector_t * input, 
    int16_t * min
);

r_dsp_status_t R_DSP_Max_i16 (
    const vector_t * input, 
    int16_t * max
);

/*----------------------------------------*/
/*  Minimum and Maximum Value with Index  */
/*----------------------------------------*/
r_dsp_status_t R_DSP_ArgMin_f32 (
    const vector_t * input, 
    float * minval, 
    uint16_t * imin
);

r_dsp_status_t R_DSP_ArgMax_f32 (
    const vector_t * input, 
    float * maxval, 
    uint16_t * imax
);

r_dsp_status_t R_DSP_ArgMin_i32 (
    const vector_t * input, 
    int32_t * minval, 
    uint16_t * imin
);

r_dsp_status_t R_DSP_ArgMax_i32 (
    const vector_t * input, 
    int32_t * maxval, 
    uint16_t * imax
);

r_dsp_status_t R_DSP_ArgMin_i16 (
    const vector_t * input, 
    int16_t * minval, 
    uint16_t * imin
);

r_dsp_status_t R_DSP_ArgMax_i16 (
    const vector_t * input, 
    int16_t * maxval, 
    uint16_t * imax
);

/*-------------------------------------*/
/*  Mean and Maximum Absolute Values  */
/*------------------------------------*/
r_dsp_status_t R_DSP_MeanAbs_f32 (
    const vector_t * input, 
    float * mean
);

r_dsp_status_t R_DSP_MaxAbs_f32 (
    const vector_t * input, 
    float * max
);

r_dsp_status_t R_DSP_MeanMaxAbs_f32 (
    const vector_t * input, 
    float * mean, 
    float * max
);

r_dsp_status_t R_DSP_MeanAbs_i32 (
    const vector_t * input, 
    int32_t * mean
);

r_dsp_status_t R_DSP_MaxAbs_i32 (
    const vector_t * input, 
    int32_t * max
);

r_dsp_status_t R_DSP_MeanMaxAbs_i32 (
    const vector_t * input, 
    int32_t * mean, 
    int32_t * max
);


r_dsp_status_t R_DSP_MeanAbs_i16 (
    const vector_t * input, 
    int16_t * mean
);

r_dsp_status_t R_DSP_MaxAbs_i16 (
    const vector_t * input, 
    int16_t * max
);

r_dsp_status_t R_DSP_MeanMaxAbs_i16 (
    const vector_t * input, 
    int16_t * mean, 
    int16_t * max
);

/*---------------------------*/
/*  Variance                 */
/*---------------------------*/

/*  Mean and Variance Computation */
r_dsp_status_t R_DSP_MeanVar_f32 (
    const vector_t * input, 
    float * mean, 
    float * variance 
);

r_dsp_status_t R_DSP_MeanVar_i32 (
    const vector_t * input, 
    int32_t * mean, 
    int32_t * variance 
);

r_dsp_status_t R_DSP_MeanVar_i16 (
    const vector_t * input, 
    int16_t * mean, 
    int32_t * variance 
);

/* Variance Computation (Mean already known) */
r_dsp_status_t R_DSP_Var_GivenMean_f32 (
    const vector_t * input, 
    const float mean, 
    float * variance 
);

r_dsp_status_t R_DSP_Var_GivenMean_i32 (
    const vector_t * input, 
    const int32_t mean, 
    int32_t * variance 
);

r_dsp_status_t R_DSP_Var_GivenMean_i16 (
    const vector_t * input, 
    const int16_t mean, 
    int32_t * variance 
);

/*-------------------------------------------------*/
/*  Histogram of Real Data, with Offset and Scale  */
/*-------------------------------------------------*/
r_dsp_status_t R_DSP_Histogram_f32ui16 (
    const vector_t * input, 
    vector_t * histo, 
    const float offset, 
    const float scale 
);

r_dsp_status_t R_DSP_Histogram_i32ui16 (
    const vector_t * input, 
    vector_t * histo, 
    const int32_t offset, 
    const int32_t scale 
);

r_dsp_status_t R_DSP_Histogram_i16ui16 (
    const vector_t * input, 
    vector_t * histo, 
    const int16_t offset, 
    const int16_t scale 
);

/*---------------------------------*/
/*  Mean Absolute Deviation (MAD)  */
/*---------------------------------*/
/*  Mean and MAD Computation       */
r_dsp_status_t R_DSP_MeanMAD_f32 (
    const vector_t * input,
    float * mean,
    float * mad
);

r_dsp_status_t R_DSP_MeanMAD_i32 (
    const vector_t * input,
    int32_t * mean,
    int32_t * mad
);

r_dsp_status_t R_DSP_MeanMAD_i16 (
    const vector_t * input,
    int16_t * mean,
    int16_t * mad
);

/*  MAD Computation (Mean already known)  */
r_dsp_status_t R_DSP_MAD_GivenMean_f32 (
    const vector_t * input,
    const float mean,
    float * mad
);

r_dsp_status_t R_DSP_MAD_GivenMean_i32 (
    const vector_t * input,
    const int32_t mean,
    int32_t * mad
);

r_dsp_status_t R_DSP_MAD_GivenMean_i16 (
    const vector_t * input,
    const int16_t mean,
    int16_t * mad
);

/*---------------------------------*/
/*  Median                         */
/*---------------------------------*/
/*  In-Place Median (does not require external scratch buffer, modifies input  */
r_dsp_status_t R_DSP_Median_InPlace_f32 (
    vector_t * input,
    float * median
);

r_dsp_status_t R_DSP_Median_InPlace_i32 (
    vector_t * input,
    int32_t * median
);

r_dsp_status_t R_DSP_Median_InPlace_i16 (
    vector_t * input,
    int16_t * median
);

/*  Median (require external scratch buffer, does not modify input  */
r_dsp_status_t R_DSP_Median_f32 (
    const vector_t * input,
    float * scratch,
    float * median
);

r_dsp_status_t R_DSP_Median_i32 (
    const vector_t * input,
    int32_t * scratch,
    int32_t * median
);

r_dsp_status_t R_DSP_Median_i16 (
    const vector_t * input,
    int16_t * scratch,
    int16_t * median
);

#endif
/* End of file */


