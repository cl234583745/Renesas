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
* File Name        : Config_CMT0.c
* Component Version: 2.3.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_CMT0.
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
#include "Config_CMT0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_CMT0_Create
* Description  : This function initializes the CMT0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_CMT0_Create(void)
{
    /* Disable CMI0 interrupt */
    IEN(CMT0,CMI0) = 0U;

    /* Cancel CMT stop state in LPC */
    MSTP(CMT0) = 0U;

    /* Set control registers */
    CMT0.CMCR.WORD = _0003_CMT_CMCR_CLOCK_PCLK512 | _0040_CMT_CMCR_CMIE_ENABLE | _0080_CMT_CMCR_DEFAULT;

    /* Set compare match register */
    CMT0.CMCOR = _F423_CMT0_CMCOR_VALUE;

    /* Set CMI0 priority level */
    IPR(CMT0,CMI0) = _00_CMT_PRIORITY_LEVEL0;

    R_Config_CMT0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_CMT0_Start
* Description  : This function starts the CMT0 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_CMT0_Start(void)
{
    /* Enable CMI0 interrupt in ICU */
    IEN(CMT0,CMI0) = 1U;

    /* Start CMT0 count */
    CMT.CMSTR0.BIT.STR0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_CMT0_Stop
* Description  : This function stop the CMT0 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_CMT0_Stop(void)
{
    /* Stop CMT0 count */
    CMT.CMSTR0.BIT.STR0 = 0U;

    /* Disable CMI0 interrupt in ICU */
    IEN(CMT0,CMI0) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */

/**********************************************************************************************************************
 * Function Name: R_CMT0_IsTimeout
 * Description  : This function checks for timeout occurrence.
 * Arguments    : bool flag
 *                    false:CMT0 not stop , true:CMT0 Stop
 * Return Value : bool
 *                    false:Timeout has not occurred, true:Timeout occurred
 *********************************************************************************************************************/
bool R_CMT0_IsTimeout (bool flag)
{
    bool ret = false;

    /** When a timeout occurs */
    if (1U == IR(CMT0, CMI0))
    {
        /** Clear the IR(CMT0, CMI0) */
        IR(CMT0, CMI0)= 0U;
        R_Config_CMT0_Stop();
        ret = true;
    }
    /** The condition given by the argument is true */
    else if(true == flag)
    {
        R_Config_CMT0_Stop();
    }
    /** Other */
    else
    {
        /** Do nothing */
        nop();
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_CMT0_IsTimeout
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_CMT0_CntClear
 * Description  : This function clear the compare match countor of CMT0
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_CMT0_CntClear (void)
{
    /** Clear CMT0 count */
    CMT0.CMCNT = 0U;
}
/**********************************************************************************************************************
 End of function R_CMT0_CntClear
 *********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
