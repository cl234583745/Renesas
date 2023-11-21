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
* File Name    : r_dsp_types.h
* Description  : Common header file for DSP library
******************************************************************************/ 
/****************************************************************************** 
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2016.11.22  4.10-a2  add mask difinition for ROUNDing andS Saturation
*              : 2018.07.11  5.0
******************************************************************************/ 

#ifndef R_DSP_TYPES_H
#define R_DSP_TYPES_H


/****************************************************************************** 
Includes
******************************************************************************/ 
#include <stddef.h>
#include "r_dsp_typedefs.h"
#include "r_dsp_ver_info.h"

/****************************************************************************** 
Macro definitions 
******************************************************************************/ 

/* Return code, 32-bit */
typedef enum {  
    R_DSP_STATUS_HISTO_OUT_OF_RANGE = 3,
    R_DSP_STATUS_UNDEFINED_RESULT   = 2,
    R_DSP_STATUS_OVERFLOW           = 1,
    R_DSP_STATUS_OK                 = 0,
    R_DSP_ERR_HANDLE_NULL           = -100,
    R_DSP_ERR_INPUT_NULL,
    R_DSP_ERR_OUTPUT_NULL,
    R_DSP_ERR_STATE_NULL,
    R_DSP_ERR_COEFF_NULL,
    R_DSP_ERR_REFER_NULL,
    R_DSP_ERR_INVALID_INPUT_SIZE,
    R_DSP_ERR_INVALID_OUTPUT_SIZE,
    R_DSP_ERR_INVALID_TAPS,
    R_DSP_ERR_INVALID_STAGES,
    R_DSP_ERR_INVALID_OPTIONS,
    R_DSP_ERR_INVALID_SCALE,
    R_DSP_ERR_DIMENSIONS,
    R_DSP_ERR_INVALID_POINTS,
    R_DSP_ERR_NO_MEMORY_AVAILABLE,
    R_DSP_ERR_INVALID_QINT,
    R_DSP_ERR_INVALID_COEFF
} r_dsp_status_t; 


/* Options, 16-bit */
/* Rounding mode, bits 0-2 */
typedef enum {  
    R_DSP_ROUNDING_DEFAULT  = 0x00,
    R_DSP_ROUNDING_TRUNC    = 0x01,
    R_DSP_ROUNDING_NEAREST  = 0x02
} r_dsp_rounding_option_t; 
#define R_DSP_ROUNDING_MASK (7)

/* Saturation mode, bits 3-4 */
typedef enum {  
    R_DSP_SATURATION_DEFAULT = 0x00,
    R_DSP_NOSATURATE         = 0x08,
    R_DSP_SATURATE           = 0x10
} r_dsp_saturation_option_t; 
#define R_DSP_SATURATION_MASK (0x18)

/* FFT/DFT Scaling mode, bits 5-6 */
typedef enum {  
    R_DSP_FFT_SCALE_DEFAULT = 0x00,
    R_DSP_FFT_OPT_SCALE     = 0x20,
    R_DSP_FFT_OPT_SCALE_X2  = 0x40
} r_dsp_transform_scaling_option_t; 
#define R_DSP_FFT_SCALE_MASK (0x0060)

/* FFT/DFT Bit-reversal mode, bits 7 */
typedef enum {  
    R_DSP_FFT_BIT_REVERSAL_DEFAULT = 0x00
/*  R_DSP_FFT_OPT_NO_BITREV        = 0x80 *//* not supported in v5.0 */
} r_dsp_transform_bit_reversal_option_t; 

/* FFT/DFT Twiddle-factors mode, bits 8 */
typedef enum {  
    R_DSP_FFT_OPT_TWIDDLE_DEFAULT = 0x0000,
    R_DSP_FFT_OPT_TW32            = 0x0100
} r_dsp_transform_twiddle_factors_option_t; 


/****************************************************************************** 
Typedef definitions 
******************************************************************************/ 
typedef struct 
{ 
    float re; 
    float im; 
} cplxf32_t; 

typedef struct 
{ 
    int32_t re; 
    int32_t im; 
} cplxi32_t; 

typedef struct 
{ 
    int16_t re; 
    int16_t im; 
} cplxi16_t; 

typedef struct 
{ 
    uint32_t n; 
    void *   data; 
} vector_t; 
 
typedef struct 
{ 
    uint16_t nRows;
    uint16_t nCols; 
    void *   data; 
} matrix_t; 

typedef union
{
    float    f32;
    int32_t  i32;
} scale_t;

/****************************************************************************** 
Exported global functions (to be accessed by other files) 
******************************************************************************/ 
uint32_t R_DSP_GetVersion(void);

#endif
/* End of file */


