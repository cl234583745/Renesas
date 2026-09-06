/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RIIC0_user.c
* Component Version: 1.11.0
* Device(s)        : R5F523E6AxFL
* Description      : This file implements device driver for Config_RIIC0.
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
#include "Config_RIIC0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t    g_riic0_mode_flag;               /* RIIC0 master transmit receive flag */
extern volatile uint8_t    g_riic0_state;                   /* RIIC0 master state */
extern volatile uint8_t   *gp_riic0_tx_address;             /* RIIC0 transmit buffer address */
extern volatile uint16_t   g_riic0_tx_count;                /* RIIC0 transmit data number */
extern volatile uint8_t   *gp_riic0_rx_address;             /* RIIC0 receive buffer address */
extern volatile uint16_t   g_riic0_rx_count;                /* RIIC0 receive data number */
extern volatile uint16_t   g_riic0_rx_length;               /* RIIC0 receive data length */
extern volatile uint8_t    g_riic0_dummy_read_count;        /* RIIC0 count for dummy read */
/* Start user code for global. Do not edit comment generated here */
extern volatile uint8_t g_slave_rx_buf[];
extern volatile uint8_t g_slave_tx_buf[];
extern volatile uint8_t g_rx_done;
extern volatile uint8_t g_tx_done;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Create_UserInit
* Description  : This function adds user code after initializing the RIIC0 bus interface
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_transmit_interrupt
* Description  : This function is TXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TXI0
#pragma interrupt r_Config_RIIC0_transmit_interrupt(vect=VECT(RIIC0,TXI0),fint)
#else
#pragma interrupt r_Config_RIIC0_transmit_interrupt(vect=VECT(RIIC0,TXI0))
#endif
static void r_Config_RIIC0_transmit_interrupt(void)
{
    if (_17_IIC_SLAVE_TRANSMIT == g_riic0_mode_flag)
    {
        if (_11_IIC_SLAVE_SENDS_DATA == g_riic0_state)
        {
            RIIC0.ICDRT = *gp_riic0_tx_address;
            gp_riic0_tx_address++;
            g_riic0_tx_count--;

            if (0U == g_riic0_tx_count)
            {
                g_riic0_state = _12_IIC_SLAVE_SENDS_END;
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_transmitend_interrupt
* Description  : This function is TEI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TEI0
#pragma interrupt r_Config_RIIC0_transmitend_interrupt(vect=VECT(RIIC0,TEI0),fint)
#else
#pragma interrupt r_Config_RIIC0_transmitend_interrupt(vect=VECT(RIIC0,TEI0))
#endif
static void r_Config_RIIC0_transmitend_interrupt(void)
{
    volatile uint8_t dummy;

    if (_12_IIC_SLAVE_SENDS_END == g_riic0_state)
    {
        g_riic0_state = _13_IIC_SLAVE_SENDS_STOP;

        /* Dummy read to release SCL */
        dummy = RIIC0.ICDRR;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_receive_interrupt
* Description  : This function is RXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_RXI0
#pragma interrupt r_Config_RIIC0_receive_interrupt(vect=VECT(RIIC0,RXI0),fint)
#else
#pragma interrupt r_Config_RIIC0_receive_interrupt(vect=VECT(RIIC0,RXI0))
#endif
static void r_Config_RIIC0_receive_interrupt(void)
{
    volatile uint8_t dummy;

    if (_10_IIC_SLAVE_RECEIVES_DATA == g_riic0_state)
    {
        if (1U > g_riic0_dummy_read_count)
        {
            dummy = RIIC0.ICDRR;
            g_riic0_dummy_read_count++;
            return;
        }

        RIIC0.ICMR3.BIT.ACKWP = 1U;
        RIIC0.ICMR3.BIT.ACKBT = 0U;
        *gp_riic0_rx_address = RIIC0.ICDRR;
        gp_riic0_rx_address++;
        g_riic0_rx_count++;

        if (1U == RIIC0.ICSR2.BIT.STOP)
        {
            /* check stop request */
            g_riic0_state = _14_IIC_SLAVE_RECEIVES_STOP;
        }
        else if (g_riic0_rx_count == g_riic0_rx_length)
        {
            g_riic0_state = _14_IIC_SLAVE_RECEIVES_STOP;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (_11_IIC_SLAVE_SENDS_DATA == g_riic0_state)
    {
        dummy = RIIC0.ICDRR;
    }
    else
    {
        /* Do nothing */
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_error_interrupt
* Description  : This function is EEI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_EEI0
#pragma interrupt r_Config_RIIC0_error_interrupt(vect=VECT(RIIC0,EEI0),fint)
#else
#pragma interrupt r_Config_RIIC0_error_interrupt(vect=VECT(RIIC0,EEI0))
#endif
static void r_Config_RIIC0_error_interrupt(void)
{
    volatile uint8_t dummy;

    if ((1U == RIIC0.ICIER.BIT.ALIE) && (1U == RIIC0.ICSR2.BIT.AL))
    {
        r_Config_RIIC0_callback_error(MD_ERROR1);
    }
    else if ((1U == RIIC0.ICIER.BIT.TMOIE) && (1U == RIIC0.ICSR2.BIT.TMOF))
    {
        r_Config_RIIC0_callback_error(MD_ERROR2);
    }
    else if ((1U == RIIC0.ICIER.BIT.NAKIE) && (1U == RIIC0.ICSR2.BIT.NACKF))
    {
        if (_17_IIC_SLAVE_TRANSMIT == g_riic0_mode_flag)
        {
            /* Dummy read to release SCL */
            dummy = RIIC0.ICDRR;

            if (0U == g_riic0_tx_count)
            {
                RIIC0.ICSR2.BIT.NACKF = 0U;
                g_riic0_state = _13_IIC_SLAVE_SENDS_STOP;
            }
            else
            {
                /* WAIT_LOOP */
                while (1U != RIIC0.ICSR2.BIT.STOP)
                {
                    nop();
                }

                RIIC0.ICSR2.BIT.NACKF = 0U;
                RIIC0.ICSR2.BIT.STOP = 0U;
                RIIC0.ICIER.BIT.SPIE = 0U;
                RIIC0.ICIER.BIT.STIE = 1U;
                r_Config_RIIC0_callback_transmitend();
            }
        }
        r_Config_RIIC0_callback_error(MD_ERROR3);
    }
    else if (_16_IIC_SLAVE_RECEIVE == g_riic0_mode_flag)
    {
        if (_14_IIC_SLAVE_RECEIVES_STOP == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;
            r_Config_RIIC0_callback_receiveend();
        }
        else if (_15_IIC_SLAVE_WAIT_START_CONDITION == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            RIIC0.ICIER.BIT.SPIE = 1U;
            g_riic0_state = _10_IIC_SLAVE_RECEIVES_DATA;
        }
        else if (_10_IIC_SLAVE_RECEIVES_DATA == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;
            g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;
        }
        else
        {
            r_Config_RIIC0_callback_error(MD_ERROR4);
        }
    }
    else if (_17_IIC_SLAVE_TRANSMIT == g_riic0_mode_flag)
    {
        if (_13_IIC_SLAVE_SENDS_STOP == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;
            r_Config_RIIC0_callback_transmitend();
        }
        else if (_15_IIC_SLAVE_WAIT_START_CONDITION == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            RIIC0.ICIER.BIT.SPIE = 1U;
            g_riic0_state = _11_IIC_SLAVE_SENDS_DATA;
        }
        else if (_11_IIC_SLAVE_SENDS_DATA == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;
            g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;
        }
        else
        {
            r_Config_RIIC0_callback_error(MD_ERROR4);
        }
    }
    else
    {
        r_Config_RIIC0_callback_error(MD_ERROR4);
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_transmitend
* Description  : This function is a callback function when I2C bus interface completed data transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_transmitend(void)
{
    /* Start user code for r_Config_RIIC0_callback_transmitend. Do not edit comment generated here */
	g_tx_done = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_receiveend
* Description  : This function is a callback function when I2C bus interface completed data reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_receiveend(void)
{
    /* Start user code for r_Config_RIIC0_callback_receiveend. Do not edit comment generated here */
	g_rx_done = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_error
* Description  : This function is a callback function when I2C bus interface encounters error
* Arguments    : status  -
*                    error status
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_error(MD_STATUS status)
{
    switch (status)
    {
        case MD_ERROR1:
        {
            /* Start user code for arbitration-lost error. Do not edit comment generated here */
            /* End user code. Do not edit comment generated here */

            RIIC0.ICSR2.BIT.AL = 0U;
            break;
        }
        case MD_ERROR2:
        {
            /* Start user code for timeout error. Do not edit comment generated here */
            /* End user code. Do not edit comment generated here */

            RIIC0.ICSR2.BIT.TMOF = 0U;
            break;
        }
        case MD_ERROR3:
        {
            /* Start user code for NACK signal. Do not edit comment generated here */
            /* End user code. Do not edit comment generated here */
            break;
        }
        case MD_ERROR4:
        {
            /* Start user code for communication sequence error. Do not edit comment generated here */
            /* End user code. Do not edit comment generated here */
            break;
        }
        default:
        {
            break;
        }
    }

    /* Start user code for others. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
