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
* File Name        : Config_SCI1.h
* Component Version: 1.12.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_SCI1.
***********************************************************************************************************************/

#ifndef CFG_Config_SCI1_H
#define CFG_Config_SCI1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_sci.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_SCI1_Create(void);
void R_Config_SCI1_Create_UserInit(void);
void R_Config_SCI1_Start(void);
void R_Config_SCI1_Stop(void);
MD_STATUS R_Config_SCI1_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num);
MD_STATUS R_Config_SCI1_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num);
/* Start user code for function. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_SCI1_SendStart
 * Description  : This function start transmission of SCI
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_SendStart (void);

/**********************************************************************************************************************
 * Function Name: R_SCI1_SendStop
 * Description  : This function stop transmission of SCI
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_SendStop (void);

/**********************************************************************************************************************
 * Function Name: R_SCI1_ReceiveStart
 * Description  : This function starts receiving of SCI1.
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_ReceiveStart (void);

/**********************************************************************************************************************
 * Function Name: R_SCI1_IsTransferEnd
 * Description  : This function returns the transfer status
 * Arguments    : None
 * Return Value : bool
 *                    0:Transferring, 1:Transfer end
 *********************************************************************************************************************/
bool R_SCI1_IsTransferEnd (void);

/* End user code. Do not edit comment generated here */
#endif
