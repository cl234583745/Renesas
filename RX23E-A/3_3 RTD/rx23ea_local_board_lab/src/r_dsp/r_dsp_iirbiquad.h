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
* File Name   : r_dsp_iirbiquad.h
* Version     : 1.50
* Description : Header file for the DSP module of iirbiquad processing
******************************************************************************/
/***************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.10.2018 1.00     First Release
*         : 26.04.2021 1.50    Fixed due to change of DSP library Version from 4.1 to 5.0
**************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#ifndef R_DSP_IIRBIQUAD_H
#define R_DSP_IIRBIQUAD_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define IIRBIQUAD_SAMPLES   (1024) /* Number of input or output samples to IIRBiquad */

/* Filter Characteristic definition */
#define IIRBIQUAD_COEF_FLAT (0)    /* Pass-through characteristics */
#define IIRBIQUAD_COEF_HPF  (1)    /* High pass filter characteristics */
#define IIRBIQUAD_COEF_LPF  (2)    /* Low pass filter characteristics */


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void R_DSP_IIRBiquad_Init (void);
void R_DSP_IIRBiquad_Operation (int32_t *in_buf, int32_t *out_buf);
void R_DSP_IIRBiquad_UpdateCoef (uint8_t preset);

#endif /* R_DSP_IIRBIQUAD_H */
