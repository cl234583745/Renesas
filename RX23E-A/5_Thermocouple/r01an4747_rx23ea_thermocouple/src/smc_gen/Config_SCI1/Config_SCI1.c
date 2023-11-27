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
* File Name    : Config_SCI1.c
* Version      : 1.8.1
* Device(s)    : R5F523E6AxFL
* Description  : This file implements device driver for Config_SCI1.
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
#include "Config_SCI1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Create
* Description  : This function initializes the SCI1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI1_Create(void)
{
    /* Cancel SCI stop state */
    MSTP(SCI1) = 0U;

    /* Set interrupt priority */
    IPR(SCI1,TXI1) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI1.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI1.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI1.SIMR1.BIT.IICM = 0U;
    SCI1.SPMR.BYTE = _02_SCI_CTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI1.SMR.BYTE = _00_SCI_CLOCK_PCLK | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_STOP_1 | _00_SCI_PARITY_DISABLE | 
                    _00_SCI_DATA_LENGTH_8 | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
    SCI1.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI1.SEMR.BYTE = _04_SCI_BIT_MODULATION_ENABLE | _10_SCI_8_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                     _40_SCI_BAUDRATE_DOUBLE | _00_SCI_LOW_LEVEL_START_BIT;

    /* Set bit rate */
    SCI1.BRR = 0x00U;
    SCI1.MDDR = 0xC0U;

    /* Set RXD1 pin */
    MPC.P30PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x01U;

    /* Set TXD1 pin */
    MPC.P26PFS.BYTE = 0x0AU;
    PORT2.PODR.BYTE |= 0x40U;
    PORT2.PDR.BYTE |= 0x40U;

    /* Set CTS1# pin */
    MPC.P31PFS.BYTE = 0x0BU;
    PORT3.PMR.BYTE |= 0x02U;

    R_Config_SCI1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Start
* Description  : This function starts the SCI1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI1_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI1,TXI1) = 0U;
    IR(SCI1,RXI1) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI1,TXI1) = 1U;
    IEN(SCI1,TEI1) = 1U;
    IEN(SCI1,RXI1) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Stop
* Description  : This function stops the SCI1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI1_Stop(void)
{
    /* Set TXD1 pin */
    PORT2.PMR.BYTE &= 0xBFU;

    /* Disable serial transmit */
    SCI1.SCR.BIT.TE = 0U;

    /* Disable serial receive */
    SCI1.SCR.BIT.RE = 0U;

    /* Disable SCI interrupt */
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.RIE = 0U;
    IEN(SCI1,TXI1) = 0U;
    IEN(SCI1,TEI1) = 0U;
    IR(SCI1,TXI1) = 0U;
    IEN(SCI1,RXI1) = 0U;
    IR(SCI1,RXI1) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Serial_Receive
* Description  : This function receives SCI1 data
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DMAC/DTC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI1_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    SCI1.SCR.BIT.RIE = 1U;
    SCI1.SCR.BIT.RE = 1U;

    return MD_OK;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Serial_Send
* Description  : This function transmits SCI1 data
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DMAC/DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI1_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    /* Set TXD1 pin */
    PORT2.PMR.BYTE |= 0x40U;

    SCI1.SCR.BIT.TIE = 1U;
    SCI1.SCR.BIT.TE = 1U;

    return MD_OK;
}

/* Start user code for adding. Do not edit comment generated here */
/**********************************************************************************************************************
 * Function Name: R_SCI1_IsTransferEnd
 * Description  : This function returns the transfer status of SCI1.
 * Arguments    : None
 * Return Value : bool
 *                    false:Transferring, true:Transfer end
 *********************************************************************************************************************/
bool R_SCI1_IsTransferEnd (void)
{
    /** true:Transmit is end, false:Transmitting */
    return (bool) ((1U == SCI1.SSR.BIT.TEND) ? true : false);
}
/**********************************************************************************************************************
 End of function R_SCI1_IsTransferEnd()
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCI1_SendStart
 * Description  : This function start transmission of SCI1
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_SendStart (void)
{
    /** Disable SCI interrupt request */
    SCI1.SCR.BIT.TIE = 0U;

    /** Disable serial transmit */
    SCI1.SCR.BIT.TE = 0U;

    /** Set TXD1 pin */
    PORT2.PMR.BYTE |= 0x40U;

    /** Clear interrupt flag */
    IR(SCI1,TXI1)= 0U;

    /** Enable SCI interrupt */
    IEN(SCI1,TXI1)= 1U;

    /** Enable SCI interrupt request */
    SCI1.SCR.BIT.TIE = 1U;

    /** Enable serial transmit */
    SCI1.SCR.BIT.TE = 1U;

    return MD_OK;
}
/**********************************************************************************************************************
 End of function R_SCI1_SendStart()
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCI1_SendStop
 * Description  : This function stop transmission of SCI1
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_SendStop (void)
{
    /** Set TXD1 pin */
    PORT2.PMR.BYTE &= 0xBFU;

    /** Disable serial transmit */
    SCI1.SCR.BIT.TE = 0U;

    /** Disable SCI interrupt request */
    SCI1.SCR.BIT.TIE = 0U;

    /** Disable SCI interrupt */
    IEN(SCI1,TXI1)= 0U;

    /** Clear interrupt flag */
    IR(SCI1,TXI1)= 0U;

    return MD_OK;
}
/**********************************************************************************************************************
 End of function R_SCI1_SendStop()
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCI1_ReceiveStart
 * Description  : This function starts receiving of SCI1.
 * Arguments    : None
 * Return Value : MD_STATUS
 *                    MD_OK
 *********************************************************************************************************************/
MD_STATUS R_SCI1_ReceiveStart (void)
{
    /** Clear interrupt flag */
    IR(SCI1,RXI1)= 0U;

    /** Enable SCI interrupt */
    IEN(SCI1,RXI1) = 1U;

    /** Enable SCI interrupt request */
    SCI1.SCR.BIT.RIE = 1U;

    /** Enable serial receive */
    SCI1.SCR.BIT.RE = 1U;

    return MD_OK;
}
/**********************************************************************************************************************
 End of function R_SCI1_ReceiveStart()
 *********************************************************************************************************************/

/* End user code. Do not edit comment generated here */   

