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
* File Name    : Config_AFE_user.c
* Version      : 1.0.0
* Device(s)    : R5F523E6AxFL
* Description  : This file implements device driver for Config_AFE.
* Creation Date: 2020-07-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_AFE.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_rtd_api.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_AFE_Create_UserInit
* Description  : This function adds user code after initializing the AFE module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_AFE_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
#if D_RTD_SELECT == 1U
    uint16_t w_count;

	/** 3-wire RTD */
    AFE.OPCR.BIT.IEXCEN = 0U;

    /* Set excitation current control register */
    AFE.EXCCR.BYTE = _02_AFE_CUR_250 | _00_AFE_MODE_2;

    /* Set excitation current output select register */
    AFE.EXCOSR.BIT.IEXC0SEL = _0009_AFE_IEXCkSEL_AIN9;
    AFE.EXCOSR.BIT.IEXC1SEL = _0008_AFE_IEXCkSEL_AIN8;

    AFE.OPCR.BIT.IEXCEN = 1U;

    /* Wait 400us for the output of IEXC stable */
    for (w_count = 0U; w_count <= _0A01_AFE_IEXC_WAIT_COUNT; w_count++)
    {
        nop();
    }
#endif
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */   
