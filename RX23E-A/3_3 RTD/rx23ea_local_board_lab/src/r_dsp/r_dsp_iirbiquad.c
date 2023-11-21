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
* Copyright (C) 2018-2021 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/*******************************************************************************
* File Name   : r_dsp_iirbiquad.c
* Version     : 1.50
* Description : DSP module of IIR Biquad filter processing
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.10.2018 1.00     First Release
*         : 26.04.2021 1.50     Fixed due to change of DSP library Version from 4.1 to 5.0
*         :                     Changed macro definition from DIRECT_BIQUAD_FORM_I
*         :                     to R_DSP_BIQUAD_FORM_I
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include "r_dsp_iirbiquad.h"
#include "../dsplib-rxv2/r_dsp_filters.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define IIR_PRV_NUM_TAPS_PER_BIQUAD     (5)   /* Number of filter coefficients */
#define IIR_PRV_NUM_DELAY_ELEMNT        (4)   /* Number of delayed data */
#define IIR_PRV_NUM_BIQUAD_STAGES       (1)   /* Number of IIR Biquad stages */
#define IIR_PRV_NUM_INTEGER_BITS        (1)   /* Number of integer bit (0 or 1) for coefficients */
#define IIR_PRV_FRACTION_BITS           (30)  /* Scaling factor */
#define IIR_PRV_NUM_PRESET              (3)   /* Number of filter characteristics prepared in demo system */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Store filter coefficient */
static int32_t gs_iirbiquad_coef[IIR_PRV_NUM_TAPS_PER_BIQUAD * IIR_PRV_NUM_BIQUAD_STAGES];

/* State to be stored in section-reversed order */
static int32_t gs_iirbiquad_delay[IIR_PRV_NUM_DELAY_ELEMNT];

/* Instantiate a handle */
static r_dsp_iirbiquad_t   gs_iirbiquad_handle;

/* Input and output vectors */
static vector_t gs_iirbiquad_input = { IIRBIQUAD_SAMPLES, NULL  };
static vector_t gs_iirbiquad_output = { IIRBIQUAD_SAMPLES, NULL };

/* Coefficient preset table */
static const int32_t gs_preset_coef[IIR_PRV_NUM_PRESET][IIR_PRV_NUM_TAPS_PER_BIQUAD] =
{
 /*  b0,         b1,         b2,         a1,         a2         */
    {0x40000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},  /* FLAT */
    {0x1EC60E8C, 0xC273E2E8, 0x1EC60E8C, 0xD50DDC01, 0x10261630},  /* HPF, Fc=160Hz */
    {0x190BA532, 0x32174A64, 0x190BA532, 0x17A65628, 0x0C883EA0},  /* LPF, Fc=300Hz */
};


/******************************************************************************
* Function Name: R_DSP_IIRBiquad_Init
* Description  : Initializes IIR Biquad filter processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_DSP_IIRBiquad_Init (void)
{
    /* ----  Set up the IIR filter biquads ---- */
    gs_iirbiquad_handle.stages = IIR_PRV_NUM_BIQUAD_STAGES;
    gs_iirbiquad_handle.form   = R_DSP_BIQUAD_FORM_I;

    /* Setup data format and options */
    gs_iirbiquad_handle.scale.i32 = IIR_PRV_FRACTION_BITS; /* convert Q2.62 product to Q1.31 */
    gs_iirbiquad_handle.qint      = IIR_PRV_NUM_INTEGER_BITS; /* coefficients and input all in Q2.30 */
    gs_iirbiquad_handle.options   = R_DSP_ROUNDING_DEFAULT | R_DSP_SATURATE;
    gs_iirbiquad_handle.state     = (void *) gs_iirbiquad_delay;

    /* Initialize the coefficients and internal state */
    gs_iirbiquad_handle.coefs = (void *) gs_iirbiquad_coef;

    if (R_DSP_STATUS_OK != R_DSP_IIRBiquad_Init_i32i32( &gs_iirbiquad_handle))
    {
        while (1)
        {
            nop(); /* no operation */
        }
    }

} /* End of function R_DSP_IIRBiquad_Init() */


/******************************************************************************
* Function Name: R_DSP_IIRBiquad_Operation
* Description  : Perform IIR Biquad filter processing.
* Arguments    : in_buf -
*                  Pointer of where to get the input data.
*                out_buf -
*                  Pointer of where to put the output data.
* Return Value : none
******************************************************************************/
void R_DSP_IIRBiquad_Operation (int32_t *in_buf, int32_t *out_buf)
{
    /* ---- Set up the input/output  ---- */
    gs_iirbiquad_input.data  = (void *) in_buf;
    gs_iirbiquad_output.data = (void *) out_buf;

    /* IIR Biquad Operation */
    if (R_DSP_STATUS_OK != R_DSP_IIRBiquad_i32i32( &gs_iirbiquad_handle, &gs_iirbiquad_input, &gs_iirbiquad_output))
    {
        while (1)
        {
            nop(); /* no operation */
        }
    }

} /* End of function R_DSP_IIRBiquad_Operation() */


/******************************************************************************
* Function Name: R_DSP_IIRBiquad_UpdateCoef
* Description  : Setting filter coefficients and clearing the delay data array.
* Arguments    : preset -
*                  Number of coefficient table
* Return Value : none
******************************************************************************/
void R_DSP_IIRBiquad_UpdateCoef(uint8_t preset)
{
    volatile uint8_t i;

    /* Coefficient preset table update  */
    for ( i = 0; i < IIR_PRV_NUM_TAPS_PER_BIQUAD; i++ )
    {
        gs_iirbiquad_coef[i] = gs_preset_coef[preset][i];
    }

    /* IIR Biquad initialize to clear delay data array */
    if(R_DSP_STATUS_OK != R_DSP_IIRBiquad_Init_i32i32(&gs_iirbiquad_handle))
    {
        while (1)
        {
            nop(); /* no operation */
        }
    }

} /* End of function R_DSP_IIRBiquad_UpdateCoef() */
