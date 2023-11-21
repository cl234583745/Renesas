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
* File Name    : r_dsp_ver_info.h
* Description  : 
******************************************************************************/
/******************************************************************************
* History      : YYYY.MM.DD  Version  Description
*              : 21.06.2012  1.00     First Release
*              : 2014.01.02  4.00     Fourth version
*              : 2018.05.11  5.00
******************************************************************************/

#ifndef R_DSP_VER_INFO_H
#define R_DSP_VER_INFO_H

/******************************************************************************
Includes
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version number for CPU */
#define RX_DSP_CPU_V1   (0x00000000)
#define RX_DSP_CPU_V2   (0x00000010)
#define RX_DSP_CPU_V3   (0x00000020)

#if defined(__RXV3)
    #define RX_DSP_CPU RX_DSP_CPU_V3
#elif defined(__RXV2)
    #define RX_DSP_CPU RX_DSP_CPU_V2
#else
    #define RX_DSP_CPU RX_DSP_CPU_V1
#endif

/* Version number for DSP Library */
/* Format: 2 digit major.2 digit minor.2 digit incremental.2 digit information */

/* For identification of DSP Library */
/* (bit 0) 1 = With error handling; 0 = Without error handling  */
#define RX_DSP_LIB_NOCHECK  (0x00000000)
#define RX_DSP_LIB_CHECK    (0x00000001)

/* (bit 1) 1 = Big-endian; 0 = Little-endian  */
#define RX_DSP_LIB_LITTLE   (0x00000000)
#define RX_DSP_LIB_BIG      (0x00000002)

/* (bit 2) 1 = with FPU; 0 = without FPU  */
#define RX_DSP_LIB_NOFPU    (0x00000000)
#define RX_DSP_LIB_FPU      (0x00000004)

typedef enum {    // for readability
    NOFPU_LE_NOCHECK = (RX_DSP_LIB_NOFPU | RX_DSP_LIB_LITTLE | RX_DSP_LIB_NOCHECK),
    NOFPU_LE_CHECK   = (RX_DSP_LIB_NOFPU | RX_DSP_LIB_LITTLE | RX_DSP_LIB_CHECK  ),
    NOFPU_BE_NOCHECK = (RX_DSP_LIB_NOFPU | RX_DSP_LIB_BIG    | RX_DSP_LIB_NOCHECK),
    NOFPU_BE_CHECK   = (RX_DSP_LIB_NOFPU | RX_DSP_LIB_BIG    | RX_DSP_LIB_CHECK  ),
    FPU_LE_NOCHECK   = (RX_DSP_LIB_FPU   | RX_DSP_LIB_LITTLE | RX_DSP_LIB_NOCHECK),
    FPU_LE_CHECK     = (RX_DSP_LIB_FPU   | RX_DSP_LIB_LITTLE | RX_DSP_LIB_CHECK  ),
    FPU_BE_NOCHECK   = (RX_DSP_LIB_FPU   | RX_DSP_LIB_BIG    | RX_DSP_LIB_NOCHECK),
    FPU_BE_CHECK     = (RX_DSP_LIB_FPU   | RX_DSP_LIB_BIG    | RX_DSP_LIB_CHECK  )

} r_dsp_lib_info;

#define R_DSP_VERSION_INFO_MASK    (0xffffff00)
#define R_DSP_CPU_INFO_MASK        (0x000000f0)
#define R_DSP_VARIATION_INFO_MASK  (0x0000000f)

#endif
