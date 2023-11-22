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
* File Name    : Config_DMAC3.h
* Version      : 1.6.1
* Device(s)    : R5F523E6AxFL
* Description  : This file implements device driver for Config_DMAC3.
* Creation Date: 2020-07-02
***********************************************************************************************************************/

#ifndef CFG_Config_DMAC3_H
#define CFG_Config_DMAC3_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_dmac.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _DC_DMAC3_ACTIVATION_SOURCE                             (0xDCU) /* Activation source select */
#define _00000000_DMAC3_SRC_ADDR                                (0x00000000UL) /* Source address */
#define _0008A023_DMAC3_DST_ADDR                                (0x0008A023UL) /* Destination address */
#define _00000001_DMAC3_DMCRA_COUNT                             (0x00000001UL) /* Transfer count */
#define _0C00_DMAC3_SRC_EXT_RPT_AREA                            (0x0C00U) /* Source address extended repeat area */
#define _0000_DMAC3_DST_EXT_RPT_AREA                            (0x0000U) /* Destination address extended repeat area */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_DMAC3_Create(void);
void R_Config_DMAC3_Start(void);
void R_Config_DMAC3_Stop(void);
void R_Config_DMAC3_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_DMAC3_SetSrcAddr
 * Description  : This function set the transfer source address of DMAC3
 * Arguments    : uint8_t *p_addr
 *                    Value to be set in DMAC3.DMSAR
 * Return Value : None
 *********************************************************************************************************************/
void R_DMAC3_SetSrcAddr (void *p_addr);

/**********************************************************************************************************************
 * Function Name: R_DMAC3_SetTxCnt
 * Description  : This function set count of DMAC3 transmit.
 * Arguments    : uint32_t cnt
 *                    count of DMAC3 transmit
 * Return Value : None
 *********************************************************************************************************************/
void R_DMAC3_SetTxCnt (uint32_t cnt);

/* End user code. Do not edit comment generated here */
#endif

