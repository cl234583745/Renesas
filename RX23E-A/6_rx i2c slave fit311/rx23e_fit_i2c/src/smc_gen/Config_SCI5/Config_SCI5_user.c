/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI5_user.c
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
extern volatile uint8_t * gp_sci5_tx_address;                /* SCI5 transmit buffer address */
extern volatile uint16_t  g_sci5_tx_count;                   /* SCI5 transmit data number */
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t  g_sci5_tx_end;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI5_Create_UserInit
* Description  : This function adds user code after initializing the SCI5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI5_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI5_transmit_interrupt
* Description  : This function is TXI5 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TXI5
#pragma interrupt r_Config_SCI5_transmit_interrupt(vect=VECT(SCI5,TXI5),fint)
#else
#pragma interrupt r_Config_SCI5_transmit_interrupt(vect=VECT(SCI5,TXI5))
#endif
static void r_Config_SCI5_transmit_interrupt(void)
{
    if (0U < g_sci5_tx_count)
    {
        SCI5.TDR = *gp_sci5_tx_address;
        gp_sci5_tx_address++;
        g_sci5_tx_count--;
    }
    else
    {
        SCI5.SCR.BIT.TIE = 0U;
        SCI5.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI5_transmitend_interrupt
* Description  : This function is TEI5 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TEI5
#pragma interrupt r_Config_SCI5_transmitend_interrupt(vect=VECT(SCI5,TEI5),fint)
#else
#pragma interrupt r_Config_SCI5_transmitend_interrupt(vect=VECT(SCI5,TEI5))
#endif
static void r_Config_SCI5_transmitend_interrupt(void)
{
    /* Set TXD5 pin */
    PORTH.PMR.BYTE &= 0xFDU;

    SCI5.SCR.BIT.TIE = 0U;
    SCI5.SCR.BIT.TE = 0U;
    SCI5.SCR.BIT.TEIE = 0U;

    r_Config_SCI5_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI5_callback_transmitend
* Description  : This function is a callback function when SCI5 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI5_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI5_callback_transmitend. Do not edit comment generated here */
	g_sci5_tx_end = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
