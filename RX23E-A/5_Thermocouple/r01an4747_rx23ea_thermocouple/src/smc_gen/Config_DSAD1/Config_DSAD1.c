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
* File Name    : Config_DSAD1.c
* Version      : 1.2.1
* Device(s)    : R5F523E6AxFL
* Description  : This file implements device driver for Config_DSAD1.
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
#include "Config_DSAD1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Create
* Description  : This function initializes the DSAD1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Create(void)
{
    uint16_t w_count;

    /* Cancel AFE module stop state */
    MSTP(AFE) = 0U;

    /* Set operation control register */
    AFE.OPCR.BIT.DSAD1EN = 1U;

    /* Wait for 400us for start-up time */
    for (w_count = 0U; w_count <= _0A01_DSAD1_STARTUP_WAIT_COUNT; w_count++)
    {
        nop();
    }

    /* Cancel DSAD1 module stop state */
    MSTP(DSAD1) = 0U;

    /* Set DSAD clock */
    DSAD1.CCR.LONG = _00000007_DSAD_PCLKB_DIV8 | _00000000_DSAD_NORMAL_MODE;

    /* Wait for operation clock stable */
    for (w_count = 0U; w_count <= _0013_DSAD1_OPERATE_CLK_WAIT_COUNT; w_count++)
    {
        nop();
    }

    /* Set operating mode register */
    DSAD1.MR.LONG = _00000000_DSAD_CONTINUOUS_MODE | _00000004_DSAD_INTER_SYNC_ENABLE | _00000000_DSAD_CHANNEL0_USED | 
                    _00000200_DSAD_CHANNEL1_UNUSED | _00000400_DSAD_CHANNEL2_UNUSED | _00000800_DSAD_CHANNEL3_UNUSED | 
                    _00001000_DSAD_CHANNEL4_UNUSED | _00002000_DSAD_CHANNEL5_UNUSED;

    /* DSAD1 Channel0 setting */
    DSAD1.MR0.LONG = _00000000_DSAD_AIN_NORMAL | _00000070_DSAD_SAMP_OSRM_USED | _00000000_DSAD_AVERAGE_DISABLE;
    DSAD1.MR0.LONG |= (_00000000_DSAD_DATA_TWOSCOMP);
    DSAD1.MR0.LONG |= (_00000000_DSAD_POSDDA_DISABLE | _00000000_DSAD_NEGDDA_DISABLE | _00000000_DSAD_DDA_CURRENT_0_5);
    DSAD1.CR0.LONG = _00100000_DSAD_PGA_ENABLE | _00050000_DSAD_PGA_GAIN_32;
    DSAD1.CR0.LONG |= (_00000800_DSAD_NUMCAL_IMMEDIATE | _00000001_DSAD_ADCONV_Y_1 | _00000000_DSAD_ADCONV_X_0);
    DSAD1.OSR0 = 0x00000C34UL;
    AFE.DS10ISR.WORD = _0007_AFE_POSIN_SIGNAL_AIN7 | _0060_AFE_NEGIN_SIGNAL_AIN6 | _0100_AFE_POSBUF_REF_ENABLE | 
                       _0200_AFE_NEGBUF_REF_ENABLE | _0C00_AFE_REFVOL_REF1P_REF1N;

    R_Config_DSAD1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Start
* Description  : This function starts the DSAD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Start(void)
{
    uint16_t w_count;

    DSAD1.MR.BIT.SYNCST = 1U;

    /* Wait for synchronization stable */
    for (w_count = 0U; w_count <= _001F_DSAD1_SYNCST_WAIT_COUNT; w_count++)
    {
        nop();
    }

    DSAD1.MR.BIT.TRGMD = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Stop
* Description  : This function stops the DSAD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Stop(void)
{
    DSAD1.MR.BIT.TRGMD = 0U;
    DSAD1.MR.BIT.SYNCST = 0U;
    DSAD1.ADSTP.BIT.STOP = 1U;

    while (DSAD1.SR.BIT.ACT == 1U)
    {
        /* Wait for the ACT flag to negate */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Set_SoftwareTrigger
* Description  : This function sets the DSAD1 software trigger
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Set_SoftwareTrigger(void)
{
    DSAD1.ADST.BIT.START = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Get_ValueResult
* Description  : This function gets result from the DSAD1 converter
* Arguments    : buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Get_ValueResult(uint32_t * const buffer)
{
    *buffer = (uint32_t)(DSAD1.DR.BIT.DATA);
}

/***********************************************************************************************************************
* Function Name: R_Config_DSAD1_Ch0_Set_DisconnectDetection
* Description  : This function sets the DSAD1 channel 0 disconnect detection
* Arguments    : pos -
*                    Positive input signal disconnect detection setting. (true:Enable false:Disable)
*                neg -
*                    Negative input signal disconnect detection setting. (true:Enable false:Disable)
* Return Value : None
***********************************************************************************************************************/

void R_Config_DSAD1_Ch0_Set_DisconnectDetection(bool pos, bool neg)
{
    if (DSAD1.MR0.BIT.DISAP != pos)
    {
        DSAD1.MR0.BIT.DISAP = pos;
    }

    if (DSAD1.MR0.BIT.DISAN != neg)
    {
        DSAD1.MR0.BIT.DISAN = neg;
    }
}

/* Start user code for adding. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_DSAD1_IsConversionEnd
 * Description  : This function returns the Conversion status of DSAD1.
 * Arguments    : None
 * Return Value : bool
 *                    false:Conversion, true:Conversion end
 *********************************************************************************************************************/
bool R_DSAD1_IsConversionEnd (void)
{
    return (bool) ((1U == IR(DSAD1, ADI1)) ? true : false);
}
/**********************************************************************************************************************
 End of function R_DSAD1_IsConversionEnd
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_DSAD1_ClearIrFlag
 * Description  : This function clears the IR flag
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_DSAD1_ClearIrFlag (void)
{
    IR(DSAD1, ADI1)= 0U;
}
/**********************************************************************************************************************
 End of function R_DSAD1_ClearIrFlag
 *********************************************************************************************************************/
/* End user code. Do not edit comment generated here */   

