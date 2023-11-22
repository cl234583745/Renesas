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
* File Name        : r_cg_dmac.h
* Version          : 1.2.20
* Device(s)        : R5F523E6AxFL
* Description      : General header file for DMAC peripheral.
***********************************************************************************************************************/

#ifndef DMAC_H
#define DMAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    DMA Transfer Mode Register (DMTMD)
*/
/* Transfer Request Source Select (DCTG[1:0]) */
#define _0000_DMAC_TRANS_REQ_SOURCE_SOFTWARE                    (0x0000U) /* Software */
#define _0001_DMAC_TRANS_REQ_SOURCE_INT                         (0x0001U) /* Interrupts */
/* Transfer Data Size Select (SZ[1:0]) */
#define _0000_DMAC_TRANS_DATA_SIZE_8                            (0x0000U) /* 8 bits */
#define _0100_DMAC_TRANS_DATA_SIZE_16                           (0x0100U) /* 16 bits */
#define _0200_DMAC_TRANS_DATA_SIZE_32                           (0x0200U) /* 32 bits */
/* Repeat Area Select (DTS[1:0]) */
#define _0000_DMAC_REPEAT_AREA_DESTINATION                      (0x0000U) /* Destination is repeat or block area */
#define _1000_DMAC_REPEAT_AREA_SOURCE                           (0x1000U) /* Source is repeat or block area */
#define _2000_DMAC_REPEAT_AREA_NONE                             (0x2000U) /* No repeat or block area */
/* Transfer Mode Select (MD[1:0]) */
#define _0000_DMAC_TRANS_MODE_NORMAL                            (0x0000U) /* Normal transfer */
#define _0000_DMAC_TRANS_MODE_FREERUN                           (0x0000U) /* Free running transfer */
#define _4000_DMAC_TRANS_MODE_REPEAT                            (0x4000U) /* Repeat transfer */
#define _8000_DMAC_TRANS_MODE_BLOCK                             (0x8000U) /* Block transfer */

/*
    DMA Interrupt Setting Register (DMINT)
*/
/* Destination Address Extended Repeat Area Overflow Interrupt Enable (DARIE) */
#define _00_DMAC_DST_EXT_RPT_OVR_INT_DISABLE                    (0x00U) /* Disable the interrupt */
#define _01_DMAC_DST_EXT_RPT_OVR_INT_ENABLE                     (0x01U) /* Enable the interrupt */
/* Source Address Extended Repeat Area Overflow Interrupt Enable (SARIE) */
#define _00_DMAC_SRC_EXT_RPT_OVR_INT_DISABLE                    (0x00U) /* Disable the interrupt */
#define _02_DMAC_SRC_EXT_RPT_OVR_INT_ENABLE                     (0x02U) /* Enable the interrupt */
/* Repeat Size End Interrupt Enable (RPTIE) */
#define _00_DMAC_RPT_SIZE_END_INT_DISABLE                       (0x00U) /* Disable the interrupt */
#define _04_DMAC_RPT_SIZE_END_INT_ENABLE                        (0x04U) /* Enable the interrupt */
/* Transfer Escape End Interrupt Enable (ESIE) */
#define _00_DMAC_TRANS_ESC_INT_DISABLE                          (0x00U) /* Disable the interrupt */
#define _08_DMAC_TRANS_ESC_INT_ENABLE                           (0x08U) /* Enable the interrupt */
/* Transfer End Interrupt Enable (DTIE) */
#define _00_DMAC_TRANS_END_INT_DISABLE                          (0x00U) /* Disable the interrupt */
#define _10_DMAC_TRANS_END_INT_ENABLE                           (0x10U) /* Enable the interrupt */

/*
    DMA Address Mode Register (DMAMD)
*/
/* Destination Address Update Mode (DM[1:0]) */
#define _0000_DMAC_DST_ADDR_UPDATE_FIXED                        (0x0000U) /* Address fixed */
#define _0040_DMAC_DST_ADDR_UPDATE_OFFSET                       (0x0040U) /* Updates by offset addition */
#define _0080_DMAC_DST_ADDR_UPDATE_INCREMENT                    (0x0080U) /* Address incremented */
#define _00C0_DMAC_DST_ADDR_UPDATE_DECREMENT                    (0x00C0U) /* Address decremented */
/* Source Address Update Mode (SM[1:0]) */
#define _0000_DMAC_SRC_ADDR_UPDATE_FIXED                        (0x0000U) /* Address fixed */
#define _4000_DMAC_SRC_ADDR_UPDATE_OFFSET                       (0x4000U) /* Updates by offset addition */
#define _8000_DMAC_SRC_ADDR_UPDATE_INCREMENT                    (0x8000U) /* Address incremented */
#define _C000_DMAC_SRC_ADDR_UPDATE_DECREMENT                    (0xC000U) /* Address decremented */

/*
    DMA Transfer Enable Register (DMCNT)
*/
/* DMA Transfer Enable (DTE) */
#define _00_DMAC_TRANS_DISABLE                                  (0x00U) /* Disable DMA transfer */
#define _01_DMAC_TRANS_ENABLE                                   (0x01U) /* Enable DMA transfer */

/*
    DMA Software Start Register (DMREQ)
*/
/* DMA Software Start (SWREQ) */
#define _01_DMAC_TRIGGER_SOFTWARE                               (0x01U) /* DMA transfer is requested */
/* DMA Software Start Bit Auto Clear Select (CLRS) */
#define _00_DMAC_TRIGGER_SOFTWARE_CLEAR_AUTO                    (0x00U) /* SWREQ is auto cleared */
#define _10_DMAC_TRIGGER_SOFTWARE_CLEAR_MANUAL                  (0x10U) /* SWREQ is not auto cleared */

/*
    DMA Status Register (DMSTS)
*/
/* Transfer Escape End Interrupt Flag (ESIF) */
#define _00_DMAC_TRANS_ESC_FLAG_CLEAR                           (0x00U) /* Transfer escape flag is not set */
#define _01_DMAC_TRANS_ESC_FLAG_SET                             (0x01U) /* Transfer escape flag is set */
/* Transfer End Interrupt Flag (DTIF) */
#define _00_DMAC_TRANS_END_FLAG_CLEAR                           (0x00U) /* Transfer end flag is not set */
#define _10_DMAC_TRANS_END_FLAG_SET                             (0x10U) /* Transfer end flag is set */
/* DMA Active Flag (ACT) */
#define _00_DMAC_OPERATION_IDLE                                 (0x00U) /* DMAC operation is idle */
#define _80_DMAC_OPERATION_ACTIVE                               (0x80U) /* DMAC operation is operating */

/*
    DMA Activation Source Flag Control Register (DMCSL)
*/
/* Interrupt Select (DISEL) */
#define _00_DMAC_INT_TRIGGER_FLAG_CLEAR                         (0x00U) /* Clears trigger flag at transfer start */
#define _01_DMAC_INT_TRIGGER_FLAG_CPU                           (0x01U) /* Keeps trigger flag for CPU interrupt */

/*
    DMAC Module Activation Register (DMAST)
*/
/* DMAC Operation Enable (DMST) */
#define _00_DMAC_ALL_CHANNEL_DISABLE                            (0x00U) /* DMAC activation is disabled */
#define _01_DMAC_ALL_CHANNEL_ENABLE                             (0x01U) /* DMAC activation is enabled */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_DMAC_PRIORITY_LEVEL0                                (0x00U) /* Level 0 (interrupt disabled) */
#define _01_DMAC_PRIORITY_LEVEL1                                (0x01U) /* Level 1 */
#define _02_DMAC_PRIORITY_LEVEL2                                (0x02U) /* Level 2 */
#define _03_DMAC_PRIORITY_LEVEL3                                (0x03U) /* Level 3 */
#define _04_DMAC_PRIORITY_LEVEL4                                (0x04U) /* Level 4 */
#define _05_DMAC_PRIORITY_LEVEL5                                (0x05U) /* Level 5 */
#define _06_DMAC_PRIORITY_LEVEL6                                (0x06U) /* Level 6 */
#define _07_DMAC_PRIORITY_LEVEL7                                (0x07U) /* Level 7 */
#define _08_DMAC_PRIORITY_LEVEL8                                (0x08U) /* Level 8 */
#define _09_DMAC_PRIORITY_LEVEL9                                (0x09U) /* Level 9 */
#define _0A_DMAC_PRIORITY_LEVEL10                               (0x0AU) /* Level 10 */
#define _0B_DMAC_PRIORITY_LEVEL11                               (0x0BU) /* Level 11 */
#define _0C_DMAC_PRIORITY_LEVEL12                               (0x0CU) /* Level 12 */
#define _0D_DMAC_PRIORITY_LEVEL13                               (0x0DU) /* Level 13 */
#define _0E_DMAC_PRIORITY_LEVEL14                               (0x0EU) /* Level 14 */
#define _0F_DMAC_PRIORITY_LEVEL15                               (0x0FU) /* Level 15 (highest) */

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

