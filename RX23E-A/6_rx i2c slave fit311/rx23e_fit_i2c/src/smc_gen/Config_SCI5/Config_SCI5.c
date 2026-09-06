/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI5.c
* Component Version: 1.12.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_SCI5.
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
#include "Config_SCI5.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci5_tx_address;                /* SCI5 transmit buffer address */
volatile uint16_t  g_sci5_tx_count;                   /* SCI5 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI5_Create
* Description  : This function initializes the SCI5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI5_Create(void)
{
    /* Cancel SCI stop state */
    MSTP(SCI5) = 0U;

    /* Set interrupt priority */
    IPR(SCI5,TXI5) = _0D_SCI_PRIORITY_LEVEL13;

    /* Clear the control register */
    SCI5.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI5.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI5.SIMR1.BIT.IICM = 0U;
    SCI5.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI5.SMR.BYTE = _00_SCI_CLOCK_PCLK | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_STOP_1 | _00_SCI_PARITY_DISABLE | 
                    _00_SCI_DATA_LENGTH_8 | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
    SCI5.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI5.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE | _10_SCI_8_BASE_CLOCK | _40_SCI_BAUDRATE_DOUBLE;

    /* Set bit rate */
    SCI5.BRR = 0x22U;

    /* Set TXD5 pin */
    MPC.PH1PFS.BYTE = 0x0AU;
    PORTH.PODR.BYTE |= 0x02U;
    PORTH.PDR.BYTE |= 0x02U;

    R_Config_SCI5_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI5_Start
* Description  : This function starts the SCI5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI5_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI5,TXI5) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI5,TXI5) = 1U;
    IEN(SCI5,TEI5) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI5_Stop
* Description  : This function stops the SCI5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI5_Stop(void)
{
    /* Set TXD5 pin */
    PORTH.PMR.BYTE &= 0xFDU;

    /* Disable serial transmit */
    SCI5.SCR.BIT.TE = 0U;

    /* Disable SCI interrupt */
    SCI5.SCR.BIT.TIE = 0U;
    IEN(SCI5,TXI5) = 0U;
    IEN(SCI5,TEI5) = 0U;
    IR(SCI5,TXI5) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI5_Serial_Send
* Description  : This function transmits SCI5 data
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DMAC/DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR or MD_ERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI5_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else if (0U == IEN(SCI5, TXI5))
    {
        status = MD_ERROR;
    }
    else
    {
        gp_sci5_tx_address = tx_buf;
        g_sci5_tx_count = tx_num;
        IEN(SCI5, TXI5) = 0U;
        SCI5.SCR.BYTE |= 0xA0U;

        /* Set TXD5 pin */
        PORTH.PMR.BYTE |= 0x02U;
        IEN(SCI5, TXI5) = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
