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
* File Name        : r_smc_interrupt.h
* Version          : 1.2.21
* Device(s)        : R5F523E6AxFL
* Description      : This file implements interrupt setting.
***********************************************************************************************************************/

#ifndef SMC_INTERRUPT_H
#define SMC_INTERRUPT_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/* Priority level of interrupt source. 
 * These macro definitions are used to set the IPR register directly
 */
#define _00_ICU_PRIORITY_LEVEL0                    (0x00U) /* Level 0 (disabled)  */
#define _01_ICU_PRIORITY_LEVEL1                    (0x01U) /* Level 1  */
#define _02_ICU_PRIORITY_LEVEL2                    (0x02U) /* Level 2  */
#define _03_ICU_PRIORITY_LEVEL3                    (0x03U) /* Level 3  */
#define _04_ICU_PRIORITY_LEVEL4                    (0x04U) /* Level 4  */
#define _05_ICU_PRIORITY_LEVEL5                    (0x05U) /* Level 5  */
#define _06_ICU_PRIORITY_LEVEL6                    (0x06U) /* Level 6  */
#define _07_ICU_PRIORITY_LEVEL7                    (0x07U) /* Level 7  */
#define _08_ICU_PRIORITY_LEVEL8                    (0x08U) /* Level 8  */
#define _09_ICU_PRIORITY_LEVEL9                    (0x09U) /* Level 9  */
#define _0A_ICU_PRIORITY_LEVEL10                   (0x0AU) /* Level 10  */
#define _0B_ICU_PRIORITY_LEVEL11                   (0x0BU) /* Level 11  */
#define _0C_ICU_PRIORITY_LEVEL12                   (0x0CU) /* Level 12  */
#define _0D_ICU_PRIORITY_LEVEL13                   (0x0DU) /* Level 13  */
#define _0E_ICU_PRIORITY_LEVEL14                   (0x0EU) /* Level 14  */
#define _0F_ICU_PRIORITY_LEVEL15                   (0x0FU) /* Level 15  */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define FAST_INTERRUPT_VECTOR                      (0)

/* The macro definitions below list the full set of priority levels as selected in the Interrupts tab
 * Please do not modify this file manually
 */
#define ICU_BSC_BUSERR_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_FCU_FRDYI_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_SWINT_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_CMT0_CMI0_PRIORITY                     (_00_ICU_PRIORITY_LEVEL0)
#define ICU_CMT1_CMI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_CAC_FERRF_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_CAC_MENDF_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_CAC_OVFF_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSPI0_SPEI0_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSPI0_SPRI0_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSPI0_SPTI0_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSPI0_SPII0_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSCAN_COMFRXINT_PRIORITY               (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSCAN_RXFINT_PRIORITY                  (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSCAN_TXINT_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSCAN_CHERRINT_PRIORITY                (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RSCAN_GLERRINT_PRIORITY                (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DOC_DOPCF_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ0_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ1_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ2_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ3_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ4_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ5_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ6_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ICU_IRQ7_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ELC_ELSR8I_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_LVD_LVD1_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_LVD_LVD2_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_S12AD_S12ADI0_PRIORITY                 (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_S12AD_GBADI_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ELC_ELSR18I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_ELC_ELSR19I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGIA0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGIB0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGIC0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGID0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TCIV0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGIE0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU0_TGIF0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU1_TGIA1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU1_TGIB1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU1_TCIV1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU1_TCIU1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU2_TGIA2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU2_TGIB2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU2_TCIV2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU2_TCIU2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU3_TGIA3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU3_TGIB3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU3_TGIC3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU3_TGID3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU3_TCIV3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU4_TGIA4_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU4_TGIB4_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU4_TGIC4_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU4_TGID4_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU4_TCIV4_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU5_TGIU5_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU5_TGIV5_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_MTU5_TGIW5_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_POE_OEI1_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_POE_OEI2_PRIORITY                      (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR0_CMIA0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR0_CMIB0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR0_OVI0_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR1_CMIA1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR1_CMIB1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR1_OVI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR2_CMIA2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR2_CMIB2_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR2_OVI2_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR3_CMIA3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR3_CMIB3_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_TMR3_OVI3_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DMAC_DMAC0I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DMAC_DMAC1I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DMAC_DMAC2I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DMAC_DMAC3I_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DSAD0_ADI0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DSAD0_SCANEND0_PRIORITY                (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DSAD1_ADI1_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_DSAD1_SCANEND1_PRIORITY                (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI1_ERI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI1_RXI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI1_TXI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI1_TEI1_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI5_ERI5_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI5_RXI5_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI5_TXI5_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI5_TEI5_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI6_ERI6_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI6_RXI6_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI6_TXI6_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI6_TEI6_PRIORITY                     (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_ERI12_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_RXI12_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_TXI12_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_TEI12_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_SCIX0_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_SCIX1_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_SCIX2_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_SCI12_SCIX3_PRIORITY                   (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RIIC0_EEI0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RIIC0_RXI0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RIIC0_TXI0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)
#define ICU_RIIC0_TEI0_PRIORITY                    (_0F_ICU_PRIORITY_LEVEL15)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Interrupt_Create(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
