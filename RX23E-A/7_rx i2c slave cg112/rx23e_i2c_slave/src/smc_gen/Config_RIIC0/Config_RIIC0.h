/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RIIC0.h
* Component Version: 1.11.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_RIIC0.
***********************************************************************************************************************/

#ifndef CFG_Config_RIIC0_H
#define CFG_Config_RIIC0_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_riic.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _F0_IIC0_SCL_LOW_LEVEL_PERIOD                          (0xF0U) /* SCL clock low-level period setting */
#define _F1_IIC0_SCL_HIGH_LEVEL_PERIOD                         (0xF1U) /* SCL clock high-level period setting */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_RIIC0_Create(void);
void R_Config_RIIC0_Create_UserInit(void);
void R_Config_RIIC0_Start(void);
void R_Config_RIIC0_Stop(void);
MD_STATUS R_Config_RIIC0_Slave_Send(uint8_t * const tx_buf, uint16_t tx_num);
MD_STATUS R_Config_RIIC0_Slave_Receive(uint8_t * const rx_buf, uint16_t rx_num);
void R_Config_RIIC0_StartCondition(void);
void R_Config_RIIC0_StopCondition(void);
static void r_Config_RIIC0_callback_transmitend(void);
static void r_Config_RIIC0_callback_receiveend(void);
static void r_Config_RIIC0_callback_error(MD_STATUS status);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
