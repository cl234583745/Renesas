/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : r_cg_afe.h
* Version          : 1.2.20
* Device(s)        : R5F523E6AxFL
* Description      : General header file for AFE peripheral.
***********************************************************************************************************************/

#ifndef AFE_H
#define AFE_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Voltage Detection Enable register (VDETER)
*/
/* LVDET0 Operation Enable (LVDET0) */
#define _0001_AFE_LVDET0_ENABLE               (0x0001U) /* LVDET0 operating */
/* LVDET1 Operation Enable (LVDET1) */
#define _0002_AFE_LVDET1_ENABLE               (0x0002U) /* LVDET1 operating */
/* IEXC0DET Operation Enable (IEXC0DET) */
#define _0010_AFE_IEXC0DET_ENABLE             (0x0010U) /* IEXC0DET operating */
/* IEXC1DET Operation Enable (IEXC1DET) */
#define _0020_AFE_IEXC1DET_ENABLE             (0x0020U) /* IEXC1DET operating */
/* IEXC2DET Operation Enable (IEXC2DET) */
#define _0040_AFE_IEXC2DET_ENABLE             (0x0040U) /* IEXC2DET operating */
/* IEXC3DET Operation Enable (IEXC3DET) */
#define _0080_AFE_IEXC3DET_ENABLE             (0x0080U) /* IEXC3DET operating */

/*
    Bias Voltage Output Select Register (VBOSR)
*/
/* AIN0 Pin VBIAS Output Control */
#define _0001_AFE_VBIASEN0_ENABLE             (0x0001U) /* AIN0 pin output VBIAS */
/* AIN1 Pin VBIAS Output Control */
#define _0002_AFE_VBIASEN1_ENABLE             (0x0002U) /* AIN1 pin output VBIAS */
/* AIN2 Pin VBIAS Output Control */
#define _0004_AFE_VBIASEN2_ENABLE             (0x0004U) /* AIN2 pin output VBIAS */
/* AIN3 Pin VBIAS Output Control */
#define _0008_AFE_VBIASEN3_ENABLE             (0x0008U) /* AIN3 pin output VBIAS */
/* AIN4 Pin VBIAS Output Control */
#define _0010_AFE_VBIASEN4_ENABLE             (0x0010U) /* AIN4 pin output VBIAS */
/* AIN5 Pin VBIAS Output Control */
#define _0020_AFE_VBIASEN5_ENABLE             (0x0020U) /* AIN5 pin output VBIAS */
/* AIN6 Pin VBIAS Output Control */
#define _0040_AFE_VBIASEN6_ENABLE             (0x0040U) /* AIN6 pin output VBIAS */
/* AIN7 Pin VBIAS Output Control */
#define _0080_AFE_VBIASEN7_ENABLE             (0x0080U) /* AIN7 pin output VBIAS */
/* AIN8 Pin VBIAS Output Control */
#define _0100_AFE_VBIASEN8_ENABLE             (0x0100U) /* AIN8 pin output VBIAS */
/* AIN9 Pin VBIAS Output Control */
#define _0200_AFE_VBIASEN9_ENABLE             (0x0200U) /* AIN9 pin output VBIAS */
/* AIN10 Pin VBIAS Output Control */
#define _0400_AFE_VBIASEN10_ENABLE            (0x0400U) /* AIN10 pin output VBIAS */
/* AIN11 Pin VBIAS Output Control */
#define _0800_AFE_VBIASEN11_ENABLE            (0x0800U) /* AIN11 pin output VBIAS */

/*
    Excitation Current Control Register (EXCCR)
*/

/* Excitation Current Setting (CUR[2:0]) */
#define _00_AFE_CUR_50                        (0x00U) /* 50 µA */
#define _01_AFE_CUR_100                       (0x01U) /* 100 µA */
#define _02_AFE_CUR_250                       (0x02U) /* 250 µA */
#define _03_AFE_CUR_500                       (0x03U) /* 500 µA */
#define _04_AFE_CUR_750                       (0x04U) /* 750 µA */
#define _05_AFE_CUR_1000                      (0x05U) /* 1000 µA */
/* Operation Mode Select (MODE) */
#define _00_AFE_MODE_2                        (0x00U) /* 2-channel output mode */
#define _10_AFE_MODE_4                        (0x10U) /* 4-channel output mode */

/*
    Excitation Current Output Select Register (EXCOSR)
*/

/* IEXCk Output Pin Select (IEXCkSEL[3:0], k = 0 to 3) */
#define _0000_AFE_IEXCkSEL_AIN0               (0x0000U) /* AIN0 pin */
#define _0001_AFE_IEXCkSEL_AIN1               (0x0001U) /* AIN1 pin */
#define _0002_AFE_IEXCkSEL_AIN2               (0x0002U) /* AIN2 pin */
#define _0003_AFE_IEXCkSEL_AIN3               (0x0003U) /* AIN3 pin */
#define _0004_AFE_IEXCkSEL_AIN4               (0x0004U) /* AIN4 pin */
#define _0005_AFE_IEXCkSEL_AIN5               (0x0005U) /* AIN5 pin */
#define _0006_AFE_IEXCkSEL_AIN6               (0x0006U) /* AIN6 pin */
#define _0007_AFE_IEXCkSEL_AIN7               (0x0007U) /* AIN7 pin */
#define _0008_AFE_IEXCkSEL_AIN8               (0x0008U) /* AIN8 pin */
#define _0009_AFE_IEXCkSEL_AIN9               (0x0009U) /* AIN9 pin */
#define _000A_AFE_IEXCkSEL_AIN10              (0x000AU) /* AIN10 pin */
#define _000B_AFE_IEXCkSEL_AIN11              (0x000BU) /* AIN11 pin */
#define _000F_AFE_IEXCkSEL_DISABLE            (0x000FU) /* Output disabled */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
