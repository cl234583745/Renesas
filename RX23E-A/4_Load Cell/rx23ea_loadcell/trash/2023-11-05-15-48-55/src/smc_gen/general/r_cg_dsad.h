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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_dsad.h
* Version      : 1.1.101
* Device(s)    : R5F523E6AxFL
* Description  : General header file for DSAD peripheral.
* Creation Date: 2020-07-02
***********************************************************************************************************************/

#ifndef DSAD_H
#define DSAD_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    DSAD Operating Clock Control Register (CCR))
*/
/* Operating Clock Division Ratio Setting (CLKDIV) */
#define _00000000_DSAD_PCLKB                  (0x00000000UL) /* PCLKB (no division) */
#define _00000001_DSAD_PCLKB_DIV2             (0x00000001UL) /* PCLKB/2 (division by 2) */
#define _00000002_DSAD_PCLKB_DIV3             (0x00000002UL) /* PCLKB/3 (division by 3) */
#define _00000003_DSAD_PCLKB_DIV4             (0x00000003UL) /* PCLKB/4 (division by 4) */
#define _00000004_DSAD_PCLKB_DIV5             (0x00000004UL) /* PCLKB/5 (division by 5) */
#define _00000005_DSAD_PCLKB_DIV6             (0x00000005UL) /* PCLKB/6 (division by 6) */
#define _00000006_DSAD_PCLKB_DIV7_5           (0x00000006UL) /* PCLKB/7.5 (division by 7.5) */
#define _00000007_DSAD_PCLKB_DIV8             (0x00000007UL) /* PCLKB/8 (division by 8) */
#define _0000000C_DSAD_PCLKB                  (0x0000000CUL) /* PCLKB (no division) */
#define _0000000D_DSAD_PCLKB_DIV2             (0x0000000DUL) /* PCLKB/2 (division by 2) */
/* Low Power Mode Setting (LPMD) */
#define _00000000_DSAD_NORMAL_MODE            (0x00000000UL) /* Normal mode */
#define _00000080_DSAD_LOWPOWER_MODE          (0x00000080UL) /* Low power mode */

/*
    DSAD Operating Mode Register (MR)
*/
/* Scan Mode Select (SCMD) */
#define _00000000_DSAD_CONTINUOUS_MODE        (0x00000000UL) /* Continuous scan mode */
#define _00000001_DSAD_SINGLE_MODE            (0x00000001UL) /* Single scan mode */
/* Inter-Unit Synchronized Start Enable (SYNCST) */
#define _00000000_DSAD_INTER_SYNC_DISABLE     (0x00000000UL) /* Disable inter-unit synchronized start */
#define _00000004_DSAD_INTER_SYNC_ENABLE      (0x00000004UL) /* Enable inter-unit synchronized start*/
/* Trigger Mode Select (TRGMD) */
#define _00000000_DSAD_START_SOFTWARE         (0x00000000UL) /* Software trigger */
#define _00000010_DSAD_START_HARDWARE         (0x00000010UL) /* Hardware trigger */
/* Channel 0 A/D Conversion Enable (CH0EN) */
#define _00000000_DSAD_CHANNEL0_USED          (0x00000000UL) /* AIN0 A/D conversion enabled */
#define _00000100_DSAD_CHANNEL0_UNUSED        (0x00000100UL) /* AIN0 A/D conversion disabled */
/* Channel 1 A/D Conversion Enable (CH1EN) */
#define _00000000_DSAD_CHANNEL1_USED          (0x00000000UL) /* AIN1 A/D conversion enabled */
#define _00000200_DSAD_CHANNEL1_UNUSED        (0x00000200UL) /* AIN1 A/D conversion disabled */
/* Channel 2 A/D Conversion Enable (CH2EN) */
#define _00000000_DSAD_CHANNEL2_USED          (0x00000000UL) /* AIN2 A/D conversion enabled */
#define _00000400_DSAD_CHANNEL2_UNUSED        (0x00000400UL) /* AIN2 A/D conversion disabled */
/* Channel 3 A/D Conversion Enable (CH3EN) */
#define _00000000_DSAD_CHANNEL3_USED          (0x00000000UL) /* AIN3 A/D conversion enabled */
#define _00000800_DSAD_CHANNEL3_UNUSED        (0x00000800UL) /* AIN3 A/D conversion disabled */
/* Channel 4 A/D Conversion Enable (CH4EN) */
#define _00000000_DSAD_CHANNEL4_USED          (0x00000000UL) /* AIN4 A/D conversion enabled */
#define _00001000_DSAD_CHANNEL4_UNUSED        (0x00001000UL) /* AIN4 A/D conversion disabled */
/* Channel 5 A/D Conversion Enable (CH5EN) */
#define _00000000_DSAD_CHANNEL5_USED          (0x00000000UL) /* AIN5 A/D conversion enabled */
#define _00002000_DSAD_CHANNEL5_UNUSED        (0x00002000UL) /* AIN5 A/D conversion disabled */

/*
    Channel m Operating Mode Register (MRm))
*/
/* A/D Conversion Mode Select (CVMD) */
#define _00000000_DSAD_AIN_NORMAL             (0x00000000UL) /* Normal operation */
#define _00000002_DSAD_AIN_SINGLECYCLE        (0x00000002UL) /* Single cycle settling */
/* Stored Data Format Select (SDF) */
#define _00000000_DSAD_DATA_TWOSCOMP          (0x00000000UL) /* TwoÅfs complement */
#define _00000004_DSAD_DATA_BINARY            (0x00000004UL) /* Straight binary */
/* Oversampling Ratio Select (OSR) */
#define _00000000_DSAD_SAMP_RATIO_64          (0x00000000UL) /* 64 */
#define _00000010_DSAD_SAMP_RATIO_128         (0x00000010UL) /* 128 */
#define _00000020_DSAD_SAMP_RATIO_256         (0x00000020UL) /* 256 */
#define _00000030_DSAD_SAMP_RATIO_512         (0x00000030UL) /* 512 */
#define _00000040_DSAD_SAMP_RATIO_1024        (0x00000040UL) /* 1024 */
#define _00000050_DSAD_SAMP_RATIO_2048        (0x00000050UL) /* 2048 */
#define _00000070_DSAD_SAMP_OSRM_USED         (0x00000070UL) /* The setting of the OSRm register is used */
/* Positive Input Signal Disconnect Detection Assist (DISAP) */
#define _00000000_DSAD_POSDDA_DISABLE         (0x00000000UL) /* Positive disconnect detection assist is disabled */
#define _00001000_DSAD_POSDDA_ENABLE          (0x00001000UL) /* Positive disconnect detection assist is enabled */
/* Negative Input Signal Disconnect Detection Assist (DISAN) */
#define _00000000_DSAD_NEGDDA_DISABLE         (0x00000000UL) /* Negative disconnect detection assist is disabled */
#define _00002000_DSAD_NEGDDA_ENABLE          (0x00002000UL) /* Negative disconnect detection assist is enabled */
/* Averaging Mode Select (AVMD) */
#define _00000000_DSAD_AVERAGE_DISABLE        (0x00000000UL) /* Results are not averaged */
#define _00020000_DSAD_ADI_EACHCOMPLETE       (0x00020000UL) /* Results are averaged. The A/D conversion end interrupt
                                                                is output each time A/D conversion is com-pleted */
#define _00030000_DSAD_ADI_SELECTCOMPLETE     (0x00030000UL) /* Results are averaged. The A/D conversion end interrupt
                                                                is output when the average of the selected number of
                                                                values is calculated */
/* Averaging Data Number Select (AVDN) */
#define _00000000_DSAD_AVERAGE_DATA_8         (0x00000000UL) /* 8 */
#define _00040000_DSAD_AVERAGE_DATA_16        (0x00040000UL) /* 16 */
#define _00080000_DSAD_AVERAGE_DATA_32        (0x00080000UL) /* 32 */
#define _000C0000_DSAD_AVERAGE_DATA_64        (0x000C0000UL) /* 64 */
/* Disconnect Detection Assist Current Select (DISC) */
#define _00000000_DSAD_DDA_CURRENT_0_5        (0x00000000UL) /* 0.5 uA */
#define _10000000_DSAD_DDA_CURRENT_2          (0x10000000UL) /* 2 uA */
#define _20000000_DSAD_DDA_CURRENT_4          (0x20000000UL) /* 4 uA */
#define _30000000_DSAD_DDA_CURRENT_20         (0x30000000UL) /* 20 uA */

/*
    Channel m Control Register (CRm))
*/
/* A/D Conversion Number Setting Y (CNY)*/
#define _00000000_DSAD_ADCONV_Y_0             (0x00000000UL) /* The value of y is 0 */
#define _00000001_DSAD_ADCONV_Y_1             (0x00000001UL) /* The value of y is 1 */
#define _00000002_DSAD_ADCONV_Y_2             (0x00000002UL) /* The value of y is 2 */
#define _00000003_DSAD_ADCONV_Y_3             (0x00000003UL) /* The value of y is 3 */
#define _00000004_DSAD_ADCONV_Y_4             (0x00000004UL) /* The value of y is 4 */
#define _00000005_DSAD_ADCONV_Y_5             (0x00000005UL) /* The value of y is 5 */
#define _00000006_DSAD_ADCONV_Y_6             (0x00000006UL) /* The value of y is 6 */
#define _00000007_DSAD_ADCONV_Y_7             (0x00000007UL) /* The value of y is 7 */
#define _00000008_DSAD_ADCONV_Y_8             (0x00000008UL) /* The value of y is 8 */
#define _00000009_DSAD_ADCONV_Y_9             (0x00000009UL) /* The value of y is 9 */
#define _0000000A_DSAD_ADCONV_Y_10            (0x0000000AUL) /* The value of y is 10 */
#define _0000000B_DSAD_ADCONV_Y_11            (0x0000000BUL) /* The value of y is 11 */
#define _0000000C_DSAD_ADCONV_Y_12            (0x0000000CUL) /* The value of y is 12 */
#define _0000000D_DSAD_ADCONV_Y_13            (0x0000000DUL) /* The value of y is 13 */
#define _0000000E_DSAD_ADCONV_Y_14            (0x0000000EUL) /* The value of y is 14 */
#define _0000000F_DSAD_ADCONV_Y_15            (0x0000000FUL) /* The value of y is 15 */
#define _00000010_DSAD_ADCONV_Y_16            (0x00000010UL) /* The value of y is 16 */
#define _00000011_DSAD_ADCONV_Y_17            (0x00000011UL) /* The value of y is 17 */
#define _00000012_DSAD_ADCONV_Y_18            (0x00000012UL) /* The value of y is 18 */
#define _00000013_DSAD_ADCONV_Y_19            (0x00000013UL) /* The value of y is 19 */
#define _00000014_DSAD_ADCONV_Y_20            (0x00000014UL) /* The value of y is 20 */
#define _00000015_DSAD_ADCONV_Y_21            (0x00000015UL) /* The value of y is 21 */
#define _00000016_DSAD_ADCONV_Y_22            (0x00000016UL) /* The value of y is 22 */
#define _00000017_DSAD_ADCONV_Y_23            (0x00000017UL) /* The value of y is 23 */
#define _00000018_DSAD_ADCONV_Y_24            (0x00000018UL) /* The value of y is 24 */
#define _00000019_DSAD_ADCONV_Y_25            (0x00000019UL) /* The value of y is 25 */
#define _0000001A_DSAD_ADCONV_Y_26            (0x0000001AUL) /* The value of y is 26 */
#define _0000001B_DSAD_ADCONV_Y_27            (0x0000001BUL) /* The value of y is 27 */
#define _0000001C_DSAD_ADCONV_Y_28            (0x0000001CUL) /* The value of y is 28 */
#define _0000001D_DSAD_ADCONV_Y_29            (0x0000001DUL) /* The value of y is 29 */
#define _0000001E_DSAD_ADCONV_Y_30            (0x0000001EUL) /* The value of y is 30 */
#define _0000001F_DSAD_ADCONV_Y_31            (0x0000001FUL) /* The value of y is 31 */
/* A/D Conversion Number Setting X (CNX)*/
#define _00000000_DSAD_ADCONV_X_0             (0x00000000UL) /* The value of x is 0 */
#define _00000020_DSAD_ADCONV_X_1             (0x00000020UL) /* The value of x is 1 */
#define _00000040_DSAD_ADCONV_X_2             (0x00000040UL) /* The value of x is 2 */
#define _00000060_DSAD_ADCONV_X_3             (0x00000060UL) /* The value of x is 3 */
#define _00000080_DSAD_ADCONV_X_4             (0x00000080UL) /* The value of x is 4 */
#define _000000A0_DSAD_ADCONV_X_5             (0x000000A0UL) /* The value of x is 5 */
#define _000000C0_DSAD_ADCONV_X_6             (0x000000C0UL) /* The value of x is 6 */
#define _000000E0_DSAD_ADCONV_X_7             (0x000000E0UL) /* The value of x is 7 */
/* A/D Conversion Number Calculation Mode Select (CNMD)*/
#define _00000000_DSAD_NUMCAL_EXPONENTIAL     (0x00000000UL) /* Exponential operating mode (the number of A/D
                                                               conversions is from 1 to 8032) */
#define _00000800_DSAD_NUMCAL_IMMEDIATE       (0x00000800UL) /* Immediate value mode (the number of A/D conversions
                                                                is from 1 to 255)*/
/* PGA Gain Select (GAIN)*/
#define _00000000_DSAD_PGA_DISABLE            (0x00000000UL) /* PGA disable */
#define _00010000_DSAD_BUFAMP_POS_ENABLE      (0x00010000UL) /* The PGA is bypassed and only the positive buffer
                                                                amplifier is enabled */
#define _00020000_DSAD_BUFAMP_NEG_ENABLE      (0x00020000UL) /* The PGA is bypassed and only the negative buffer
                                                                amplifier is enabled */
#define _00030000_DSAD_BUFAMP_BOTH_ENABLE     (0x00030000UL) /* The PGA is bypassed and both buffer amplifiers
                                                                are enabled */
#define _00100000_DSAD_PGA_ENABLE             (0x00100000UL) /* PGA enable */
#define _00000000_DSAD_PGA_GAIN_1             (0x00000000UL) /* PGA gain: Å~1 */
#define _00010000_DSAD_PGA_GAIN_2             (0x00010000UL) /* PGA gain: Å~2 */
#define _00020000_DSAD_PGA_GAIN_4             (0x00020000UL) /* PGA gain: Å~4 */
#define _00030000_DSAD_PGA_GAIN_8             (0x00030000UL) /* PGA gain: Å~8 */
#define _00040000_DSAD_PGA_GAIN_16            (0x00040000UL) /* PGA gain: Å~16 */
#define _00050000_DSAD_PGA_GAIN_32            (0x00050000UL) /* PGA gain: Å~32 */
#define _00060000_DSAD_PGA_GAIN_64            (0x00060000UL) /* PGA gain: Å~64 */
#define _00070000_DSAD_PGA_GAIN_128           (0x00070000UL) /* PGA gain: Å~128 */

/*
    Conversion Start Register (ADST)
*/
#define _00000001_DSAD_AUTOSCAN_START         (0x00000001UL) /* Auto Scan Start */

/*
    A/D Conversion Stop Register (ADSTP)
*/
#define _00000001_DSAD_AUTOSCAN_STOP          (0x00000001UL) /* Auto Scan Stop */

/*
    DSAD0 Channel m Input Select Register (DS0mISR)
*/
/* Positive Input Signal Select (PSEL)*/
#define _0000_AFE_POSIN_SIGNAL_AIN0           (0x0000U) /* AIN0 pin */
#define _0001_AFE_POSIN_SIGNAL_AIN1           (0x0001U) /* AIN1 pin */
#define _0002_AFE_POSIN_SIGNAL_AIN2           (0x0002U) /* AIN2 pin */
#define _0003_AFE_POSIN_SIGNAL_AIN3           (0x0003U) /* AIN3 pin */
#define _0004_AFE_POSIN_SIGNAL_AIN4           (0x0004U) /* AIN4 pin */
#define _0005_AFE_POSIN_SIGNAL_AIN5           (0x0005U) /* AIN5 pin */
#define _0006_AFE_POSIN_SIGNAL_AIN6           (0x0006U) /* AIN6 pin */
#define _0007_AFE_POSIN_SIGNAL_AIN7           (0x0007U) /* AIN7 pin */
#define _0008_AFE_POSIN_SIGNAL_AIN8           (0x0008U) /* AIN8 pin */
#define _0009_AFE_POSIN_SIGNAL_AIN9           (0x0009U) /* AIN9 pin */
#define _000A_AFE_POSIN_SIGNAL_AIN10          (0x000AU) /* AIN10 pin */
#define _000B_AFE_POSIN_SIGNAL_AIN11          (0x000BU) /* AIN11 pin */
#define _000C_AFE_POSIN_SIGNAL_OFFMESUR       (0x000CU) /* Offset error measurement */
#define _000F_AFE_POSIN_SIGNAL_NO             (0x000FU) /* No connection */
/* Negative Input Signal Select (NSEL)*/
#define _0000_AFE_NEGIN_SIGNAL_AIN0           (0x0000U) /* AIN0 pin */
#define _0010_AFE_NEGIN_SIGNAL_AIN1           (0x0010U) /* AIN1 pin */
#define _0020_AFE_NEGIN_SIGNAL_AIN2           (0x0020U) /* AIN2 pin */
#define _0030_AFE_NEGIN_SIGNAL_AIN3           (0x0030U) /* AIN3 pin */
#define _0040_AFE_NEGIN_SIGNAL_AIN4           (0x0040U) /* AIN4 pin */
#define _0050_AFE_NEGIN_SIGNAL_AIN5           (0x0050U) /* AIN5 pin */
#define _0060_AFE_NEGIN_SIGNAL_AIN6           (0x0060U) /* AIN6 pin */
#define _0070_AFE_NEGIN_SIGNAL_AIN7           (0x0070U) /* AIN7 pin */
#define _0080_AFE_NEGIN_SIGNAL_AIN8           (0x0080U) /* AIN8 pin */
#define _0090_AFE_NEGIN_SIGNAL_AIN9           (0x0090U) /* AIN9 pin */
#define _00A0_AFE_NEGIN_SIGNAL_AIN10          (0x00A0U) /* AIN10 pin */
#define _00B0_AFE_NEGIN_SIGNAL_AIN11          (0x00B0U) /* AIN11 pin */
#define _00F0_AFE_NEGIN_SIGNAL_NO             (0x00F0U) /* No connection */
/* Reference Voltage Select (RSEL)*/
#define _0000_AFE_POSBUF_REF_DISABLE          (0x0000U) /* Buffer for the positive reference voltage is disabled */
#define _0100_AFE_POSBUF_REF_ENABLE           (0x0100U) /* Buffer for the positive reference voltage is enabled */
#define _0000_AFE_NEGBUF_REF_DISABLE          (0x0000U) /* Buffer for the negative reference voltage is disabled */
#define _0200_AFE_NEGBUF_REF_ENABLE           (0x0200U) /* Buffer for the negative reference voltage is enabled */
#define _0000_AFE_REFVOL_AVCC0_AVSS0          (0x0000U) /* AVCC0/AVSS0 */
#define _0400_AFE_REFVOL_REFOUT_AVSS0         (0x0400U) /* REFOUT/AVSS0 */
#define _0800_AFE_REFVOL_REF0P_REF0N          (0x0800U) /* REF0P/REF0N */
#define _0C00_AFE_REFVOL_REF1P_REF1N          (0x0C00U) /* REF1P/REF1N */
/* Temperature Sensor Connection (TSEN)*/
#define _8000_AFE_POSIN_SIGNAL_TEMPSEN        (0x8000U) /* The temperature sensor is connected */

/*
    Voltage Detector Control Register (VDETCR)
*/
/* VR0P Disconnect Detection Assist (VR0PDISA) */
#define _0100_DSAD0_REF_POS_DC_DET_ENABLE     (0x0100U) /* Disconnect detection assist is enabled */
/* VR0P Disconnect Detection Charge Mode Setting (VR0PDISC) */
#define _0200_DSAD0_REF_POS_DC_DETMODE_CHARGE (0x0200U) /* Charge */
/* VR0N Disconnect Detection Assist (VR0NDISA) */
#define _0400_DSAD0_REF_NEG_DC_DET_ENABLE     (0x0400U) /* Disconnect detection assist is enabled */
/* VR0N Disconnect Detection Charge Mode Setting (VR0NDISC) */
#define _0800_DSAD0_REF_NEG_DC_DETMODE_CHARGE (0x0800U) /* Charge */
/* VR1P Disconnect Detection Assist (VR1PDISA) */
#define _1000_DSAD1_REF_POS_DC_DET_ENABLE     (0x1000U) /* Disconnect detection assist is enabled */
/* VR1P Disconnect Detection Charge Mode Setting (VR1PDISC) */
#define _2000_DSAD1_REF_POS_DC_DETMODE_CHARGE (0x2000U) /* Charge */
/* VR1N Disconnect Detection Assist (VR1NDISA) */
#define _4000_DSAD1_REF_NEG_DC_DET_ENABLE     (0x4000U) /* Disconnect detection assist is enabled */
/* VR1N Disconnect Detection Charge Mode Setting (VR1NDISC) */
#define _8000_DSAD1_REF_NEG_DC_DETMODE_CHARGE (0x8000U) /* Charge */

/*
    Voltage Detection Enable register (VDETER)
*/
/* DS0PDET Operation Enable (DS0PDET) */
#define _0100_DSAD0_IN_POS_DET_ENABLE         (0x0100U) /* Operating */
/* DS0NDET Operation Enable (DS0NDET) */
#define _0200_DSAD0_IN_NEG_DET_ENABLE         (0x0200U) /* Operating */
/* DS0RDET Operation Enable (DS0RDET) */
#define _0400_DSAD0_REF_DET_ENABLE            (0x0400U) /* Operating */
/* DS1PDET Operation Enable (DS1PDET) */
#define _1000_DSAD1_IN_POS_DET_ENABLE         (0x1000U) /* Operating */
/* DS1NDET Operation Enable (DS1NDET) */
#define _2000_DSAD1_IN_NEG_DET_ENABLE         (0x2000U) /* Operating */
/* DS1RDET Operation Enable (DS1RDET) */
#define _4000_DSAD1_REF_DET_ENABLE            (0x4000U) /* Operating */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_DSAD_PRIORITY_LEVEL0              (0x00U) /* Level 0 (interrupt disabled) */
#define _01_DSAD_PRIORITY_LEVEL1              (0x01U) /* Level 1 */
#define _02_DSAD_PRIORITY_LEVEL2              (0x02U) /* Level 2 */
#define _03_DSAD_PRIORITY_LEVEL3              (0x03U) /* Level 3 */
#define _04_DSAD_PRIORITY_LEVEL4              (0x04U) /* Level 4 */
#define _05_DSAD_PRIORITY_LEVEL5              (0x05U) /* Level 5 */
#define _06_DSAD_PRIORITY_LEVEL6              (0x06U) /* Level 6 */
#define _07_DSAD_PRIORITY_LEVEL7              (0x07U) /* Level 7 */
#define _08_DSAD_PRIORITY_LEVEL8              (0x08U) /* Level 8 */
#define _09_DSAD_PRIORITY_LEVEL9              (0x09U) /* Level 9 */
#define _0A_DSAD_PRIORITY_LEVEL10             (0x0AU) /* Level 10 */
#define _0B_DSAD_PRIORITY_LEVEL11             (0x0BU) /* Level 11 */
#define _0C_DSAD_PRIORITY_LEVEL12             (0x0CU) /* Level 12 */
#define _0D_DSAD_PRIORITY_LEVEL13             (0x0DU) /* Level 13 */
#define _0E_DSAD_PRIORITY_LEVEL14             (0x0EU) /* Level 14 */
#define _0F_DSAD_PRIORITY_LEVEL15             (0x0FU) /* Level 15 (highest) */

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
