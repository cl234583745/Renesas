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
* File Name        : r_cg_tmr.h
* Version          : 1.2.20
* Device(s)        : R5F523E6AxFL
* Description      : General header file for TMR peripheral.
***********************************************************************************************************************/

#ifndef TMR_H
#define TMR_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Timer Control Register (TCR)
*/
/* Counter Clear (CCLR[1:0]) */
#define _00_TMR_CNT_CLR_DISABLE                     (0x00U) /* Clearing is disabled */
#define _08_TMR_CNT_CLR_COMP_MATCH_A                (0x08U) /* Cleared by compare match A */
#define _10_TMR_CNT_CLR_COMP_MATCH_B                (0x10U) /* Cleared by compare match B */
#define _18_TMR_CNT_CLR_TMRI_PIN                    (0x18U) /* Cleared by the external reset input */
/* Timer Overflow Interrupt Enable (OVIE) */
#define _00_TMR_OVI_INT_DISABLE                     (0x00U) /* Overflow interrupt requests (OVIn) are disabled */
#define _20_TMR_OVI_INT_ENABLE                      (0x20U) /* Overflow interrupt requests (OVIn) are enabled */
/* Compare Match Interrupt Enable A (CMIEA) */
#define _00_TMR_CMIA_INT_DISABLE                    (0x00U) /* Compare match A interrupt (CMIAn) are disabled */
#define _40_TMR_CMIA_INT_ENABLE                     (0x40U) /* Compare match A interrupt (CMIAn) are enabled */
/* Compare Match Interrupt Enable B (CMIEB) */
#define _00_TMR_CMIB_INT_DISABLE                    (0x00U) /* Compare match B interrupt (CMIBn) are disabled */
#define _80_TMR_CMIB_INT_ENABLE                     (0x80U) /* Compare match B interrupt (CMIBn) are enabled */

/* 
    Timer Counter Control Register (TCCR)
*/
/* Clock Select (CKS[2:0]) */
#define _00_TMR_CLK_DISABLED                        (0x00U) /* Clock input prohibited */
#define _01_TMR_EXT_CLK_RISING                      (0x01U) /* Count at external clock rising edge */
#define _02_TMR_EXT_CLK_FALLING                     (0x02U) /* Count at external clock falling edge */
#define _03_TMR_EXT_CLK_BOTH                        (0x03U) /* Count at external clock both edges */
#define _00_TMR_PCLK_DIV_1                          (0x00U) /* Count at PCLK */
#define _01_TMR_PCLK_DIV_2                          (0x01U) /* Count at PCLK/2 */
#define _02_TMR_PCLK_DIV_8                          (0x02U) /* Count at PCLK/8 */
#define _03_TMR_PCLK_DIV_32                         (0x03U) /* Count at PCLK/32 */
#define _04_TMR_PCLK_DIV_64                         (0x04U) /* Count at PCLK/64 */
#define _05_TMR_PCLK_DIV_1024                       (0x05U) /* Count at PCLK/1024 */
#define _06_TMR_PCLK_DIV_8192                       (0x06U) /* Count at PCLK/8192 */
#define _00_TMR_CLK_COMP_MATCH_A                    (0x00U) /* Count at compare match A */
/* Clock Source Select (CSS[1:0]) */
#define _00_TMR_CLK_SRC_EXT                         (0x00U) /* Use external clock */
#define _08_TMR_CLK_SRC_PCLK                        (0x08U) /* Use PCLK */
#define _18_TMR_CLK_TMR1_OVRF                       (0x18U) /* Count at TMR1.TCNT overflow signal */
#define _18_TMR_CLK_TMR0_COMP_MATCH_A               (0x18U) /* Count at TMR0 compare match A */
#define _18_TMR_CLK_TMR3_OVRF                       (0x18U) /* Count at TMR3.TCNT overflow signal */
#define _18_TMR_CLK_TMR2_COMP_MATCH_A               (0x18U) /* Count at TMR2 compare match A */
/* Timer Reset Detection Condition Select (TMRIS) */
#define _00_TMR_CLR_EXT_RESET_RISING                (0x00U) /* Clear at rising edge of external reset */
#define _80_TMR_CLR_EXT_RESET_HIGH                  (0x80U) /* Clear at high level of external reset */

/*
    Timer Control/Status Register (TCSR)
*/
/* Output Select A (OSA[1:0]) */
#define _00_TMR_COMP_MATCH_A_OUTPUT_RETAIN          (0x00U) /* No change */
#define _01_TMR_COMP_MATCH_A_OUTPUT_LOW             (0x01U) /* Low is output */
#define _02_TMR_COMP_MATCH_A_OUTPUT_HIGH            (0x02U) /* High is output */
#define _03_TMR_COMP_MATCH_A_OUTPUT_TOGGLE          (0x03U) /* Toggle output */
/* Output Select B (OSB[1:0]) */
#define _00_TMR_COMP_MATCH_B_OUTPUT_RETAIN          (0x00U) /* No change */
#define _04_TMR_COMP_MATCH_B_OUTPUT_LOW             (0x04U) /* Low is output */
#define _08_TMR_COMP_MATCH_B_OUTPUT_HIGH            (0x08U) /* High is output */
#define _0C_TMR_COMP_MATCH_B_OUTPUT_TOGGLE          (0x0CU) /* Toggle output */
#define _E0_TMR02_TCSR_DEFAULT                      (0xE0U) /* Default TCSR write value for TMR0/2 */
#define _F0_TMR13_TCSR_DEFAULT                      (0xF0U) /* Default TCSR write value for TMR1/3 */

/*
Timer Count Start Status (TCSTR)
*/
/* Timer Counter Status (TCS) */
#define _00_TMR_CNT_STOP_BY_ELC                     (0x00U) /* Count stop state in response to ELC */
#define _01_TMR_CNT_START_BY_ELC                    (0x01U) /* Count start state in response to ELC */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_TMR_PRIORITY_LEVEL0                     (0x00U) /* Level 0 (interrupt disabled) */
#define _01_TMR_PRIORITY_LEVEL1                     (0x01U) /* Level 1 */
#define _02_TMR_PRIORITY_LEVEL2                     (0x02U) /* Level 2 */
#define _03_TMR_PRIORITY_LEVEL3                     (0x03U) /* Level 3 */
#define _04_TMR_PRIORITY_LEVEL4                     (0x04U) /* Level 4 */
#define _05_TMR_PRIORITY_LEVEL5                     (0x05U) /* Level 5 */
#define _06_TMR_PRIORITY_LEVEL6                     (0x06U) /* Level 6 */
#define _07_TMR_PRIORITY_LEVEL7                     (0x07U) /* Level 7 */
#define _08_TMR_PRIORITY_LEVEL8                     (0x08U) /* Level 8 */
#define _09_TMR_PRIORITY_LEVEL9                     (0x09U) /* Level 9 */
#define _0A_TMR_PRIORITY_LEVEL10                    (0x0AU) /* Level 10 */
#define _0B_TMR_PRIORITY_LEVEL11                    (0x0BU) /* Level 11 */
#define _0C_TMR_PRIORITY_LEVEL12                    (0x0CU) /* Level 12 */
#define _0D_TMR_PRIORITY_LEVEL13                    (0x0DU) /* Level 13 */
#define _0E_TMR_PRIORITY_LEVEL14                    (0x0EU) /* Level 14 */
#define _0F_TMR_PRIORITY_LEVEL15                    (0x0FU) /* Level 15 (highest) */

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
