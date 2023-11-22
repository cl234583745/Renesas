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
* File Name        : Config_CMT0.h
* Component Version: 2.3.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_CMT0.
***********************************************************************************************************************/

#ifndef CFG_Config_CMT0_H
#define CFG_Config_CMT0_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_cmt.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CMT0_PCLK_COUNTER_DIVISION          (512)

/* Compare Match Constant Register (CMCOR) */
#define _F423_CMT0_CMCOR_VALUE              (0xF423U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_CMT0_Create(void);
void R_Config_CMT0_Create_UserInit(void);
void R_Config_CMT0_Start(void);
void R_Config_CMT0_Stop(void);
/* Start user code for function. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_CMT0_IsTimeout
 * Description  : This function checks for timeout occurrence
 * Arguments    : bool flag
 *                    false:CMT0 not stop , true:CMT0 Stop
 * Return Value : bool
 *                    false:Timeout has not occurred, true:Timeout occurred
 *********************************************************************************************************************/
bool R_CMT0_IsTimeout (bool flag);

/**********************************************************************************************************************
 * Function Name: R_CMT0_CntClear
 * Description  : This function clear the compare match countor of CMT0
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_CMT0_CntClear (void);

/* End user code. Do not edit comment generated here */
#endif
