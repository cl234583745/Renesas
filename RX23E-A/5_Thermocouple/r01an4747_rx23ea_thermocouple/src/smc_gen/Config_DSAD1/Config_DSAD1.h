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
* File Name    : Config_DSAD1.h
* Version      : 1.2.1
* Device(s)    : R5F523E6AxFL
* Description  : This file implements device driver for Config_DSAD1.
* Creation Date: 2020-07-02
***********************************************************************************************************************/

#ifndef CFG_Config_DSAD1_H
#define CFG_Config_DSAD1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_dsad.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _0013_DSAD1_OPERATE_CLK_WAIT_COUNT              (0x0013U) /* DSAD1 operation clock wait count */
#define _001F_DSAD1_SYNCST_WAIT_COUNT                   (0x001FU) /* DSAD1 synchronization wait count */
#define _0A01_DSAD1_STARTUP_WAIT_COUNT                  (0x0A01U) /* DSAD1 start-up wait count */
#define _0004_DSAD1_STOP_WAIT_COUNT                     (0x0004U) /* DSAD1 stop wait count */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_DSAD1_Create(void);
void R_Config_DSAD1_Start(void);
void R_Config_DSAD1_Stop(void);
void R_Config_DSAD1_Set_SoftwareTrigger(void);
void R_Config_DSAD1_Get_ValueResult(uint32_t * const buffer);
void R_Config_DSAD1_Ch0_Set_DisconnectDetection(bool pos, bool neg);
void R_Config_DSAD1_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_DSAD1_IsConversionEnd
 * Description  : This function returns the Conversion status of DSAD1.
 * Arguments    : None
 * Return Value : bool
 *                    false:Conversion, true:Conversion end
 *********************************************************************************************************************/
bool R_DSAD1_IsConversionEnd (void);

/**********************************************************************************************************************
 * Function Name: R_DSAD1_ClearIrFlag
 * Description  : This function clears the IR flag
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_DSAD1_ClearIrFlag (void);

/* End user code. Do not edit comment generated here */
#endif

