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
* File Name        : Config_DMAC0.c
* Component Version: 1.8.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_DMAC0.
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
#include "Config_DMAC0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Create
* Description  : This function initializes the DMAC0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Create(void)
{
    /* Cancel DMAC/DTC module stop state in LPC */
    MSTP(DMAC) = 0U;

    /* Disable DMAC interrupts */
    IEN(DMAC,DMAC0I) = 0U;

    /* Disable DMAC0 transfer */
    DMAC0.DMCNT.BIT.DTE = 0U;

    /* Set DMAC0 activation source */
    ICU.DMRSR0 = _DB_DMAC0_ACTIVATION_SOURCE;

    /* Set DMAC0 transfer address update and extended repeat setting */
    DMAC0.DMAMD.WORD = _0000_DMAC_SRC_ADDR_UPDATE_FIXED | _0080_DMAC_DST_ADDR_UPDATE_INCREMENT | 
                       _0000_DMAC0_SRC_EXT_RPT_AREA | _0009_DMAC0_DST_EXT_RPT_AREA;

    /* Set DMAC0 transfer mode, data size and repeat area */
    DMAC0.DMTMD.WORD = _0000_DMAC_TRANS_MODE_FREERUN | _2000_DMAC_REPEAT_AREA_NONE | _0000_DMAC_TRANS_DATA_SIZE_8 | 
                       _0001_DMAC_TRANS_REQ_SOURCE_INT;

    /* Set DMAC0 interrupt flag control */
    DMAC0.DMCSL.BYTE = _00_DMAC_INT_TRIGGER_FLAG_CLEAR;

    /* Set DMAC0 source address */
    DMAC0.DMSAR = (void *)_0008A025_DMAC0_SRC_ADDR;

    /* Set DMAC0 destination address */
    DMAC0.DMDAR = (void *)_00000000_DMAC0_DST_ADDR;

    /* Clear DMAC0 transfer count (free running) */
    DMAC0.DMCRA = _00000000_DMAC0_DMCRA_COUNT;

    /* Enable DMAC activation */
    DMAC.DMAST.BIT.DMST = 1U;
    
    R_Config_DMAC0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Start
* Description  : This function enable the DMAC0 activation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Start(void)
{
    DMAC0.DMCNT.BIT.DTE = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Stop
* Description  : This function disable the DMAC0 activation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Stop(void)
{
    DMAC0.DMCNT.BIT.DTE = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_DMAC0_SetDestAddr
 * Description  : This function set the transfer destination address of DMAC0
 * Arguments    : void *p_addr
 *                    Value to be set in DMAC0.DMDAR
 * Return Value : None
 *********************************************************************************************************************/
void R_DMAC0_SetDestAddr (void *p_addr)
{
    /** Set DMAC0.DMDAR */
    DMAC0.DMDAR = p_addr;
}
/**********************************************************************************************************************
 End of function R_DMAC0_SetDestAddr
 *********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
