/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RIIC0.c
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
volatile uint8_t  g_riic0_mode_flag;               /* RIIC0 master transmit receive flag */
volatile uint8_t  g_riic0_state;                   /* RIIC0 state */
volatile uint8_t *gp_riic0_tx_address;             /* RIIC0 transmit buffer address */
volatile uint16_t g_riic0_tx_count;                /* RIIC0 transmit data number */
volatile uint8_t *gp_riic0_rx_address;             /* RIIC0 receive buffer address */
volatile uint16_t g_riic0_rx_count;                /* RIIC0 receive data number */
volatile uint16_t g_riic0_rx_length;               /* RIIC0 receive data length */
volatile uint8_t  g_riic0_dummy_read_count;        /* RIIC0 count for dummy read */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Create
* Description  : This function initializes the RIIC0 Bus Interface
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_Create(void)
{
    MSTP(RIIC0) = 0U;                /* Cancel RIIC0 module stop state */
    RIIC0.ICCR1.BIT.ICE = 0U;        /* SCL and SDA pins in inactive state */
    RIIC0.ICCR1.BIT.IICRST = 1U;     /* RIIC reset */
    RIIC0.ICCR1.BIT.ICE = 1U;        /* Internal reset */

    /* Set SARLy and SARUy */
    RIIC0.SARU0.BYTE = _00_IIC_SARU0_7_BIT;
    RIIC0.SARL0.BYTE = 0x94U;

    /* Set ICSER */
    RIIC0.ICSER.BYTE = _01_IIC_SLAVE_0_ENABLE | _00_IIC_SLAVE_1_DISABLE | _00_IIC_SLAVE_2_DISABLE | 
                       _00_IIC_GENERAL_CALL_ADDRESS_DISABLE | _00_IIC_DEVICEID_DETECT_DISABLE;

    /* Set transfer bit rate */
    RIIC0.ICMR1.BYTE |= _30_IIC_PCLK_DIV_8;
    RIIC0.ICBRL.BYTE = _F0_IIC0_SCL_LOW_LEVEL_PERIOD;
    RIIC0.ICBRH.BYTE = _F1_IIC0_SCL_HIGH_LEVEL_PERIOD;

    /* Set ICMR2 and ICMR3 */
    RIIC0.ICMR2.BYTE = _00_IIC_TIMEOUT_LONG_MODE | _02_IIC_TIMEOUT_SCL_LOW_ENABLE | _04_IIC_TIMEOUT_SCL_HIGH_ENABLE | 
                       _00_IIC_NO_OUTPUT_DELAY;
    RIIC0.ICMR3.BIT.NF = _02_IIC_NOISE_FILTER_3;
    RIIC0.ICMR3.BIT.SMBS = 0U;

    /* Set ICFER */
    RIIC0.ICFER.BYTE = _01_IIC_TIMEOUT_FUNCTION_ENABLE | _00_IIC_NACK_ARBITRATION_DISABLE | 
                       _00_IIC_SLAVE_ARBITRATION_DISABLE | _00_IIC_NACK_SUSPENSION_DISABLE | 
                       _20_IIC_NOISE_FILTER_USED | _40_IIC_SCL_SYNCHRONOUS_USED;

    /* Set ICIER */
    RIIC0.ICIER.BYTE = _01_IIC_TIMEOUT_INTERRUPT_ENABLE | _04_IIC_START_CONDITION_INTERRUPT_ENABLE | 
                       _08_IIC_STOP_CONDITION_INTERRUPT_ENABLE | _20_IIC_RECEIVE_DATA_INTERRUPT_ENABLE | 
                       _40_IIC_TRANSMIT_END_INTERRUPT_ENABLE | _80_IIC_TRANSMIT_EMPTY_INTERRUPT_ENABLE;

    /* Cancel internal reset */
    RIIC0.ICCR1.BIT.IICRST = 0U;

    /* Set interrupt priority */
    IPR(RIIC0, TXI0) = _0F_IIC_PRIORITY_LEVEL15;
    IPR(RIIC0, TEI0) = _0F_IIC_PRIORITY_LEVEL15;
    IPR(RIIC0, RXI0) = _0F_IIC_PRIORITY_LEVEL15;
    IPR(RIIC0, EEI0) = _0F_IIC_PRIORITY_LEVEL15;

    /* Set SCL pin */
    MPC.P16PFS.BYTE = 0x0FU;
    PORT1.PMR.BYTE |= 0x40U;

    /* Set SDA pin */
    MPC.P17PFS.BYTE = 0x0FU;
    PORT1.PMR.BYTE |= 0x80U;

    R_Config_RIIC0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Start
* Description  : This function starts the RIIC0 Bus Interface
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_Start(void)
{
    /* Clear interrupt flag */
    IR(RIIC0, TXI0) = 0U;
    IR(RIIC0, RXI0) = 0U;

    /* Enable RIIC0 interrupt */
    IEN(RIIC0, TXI0) = 1U;
    IEN(RIIC0, TEI0) = 1U;
    IEN(RIIC0, RXI0) = 1U;
    IEN(RIIC0, EEI0) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Stop
* Description  : This function stops the RIIC0 Bus Interface
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_Stop(void)
{
    /* Clear interrupt flag */
    IR(RIIC0, TXI0) = 0U;
    IR(RIIC0, RXI0) = 0U;

    /* Disable RIIC0 interrupt */
    IEN(RIIC0, TXI0) = 0U;
    IEN(RIIC0, TEI0) = 0U;
    IEN(RIIC0, RXI0) = 0U;
    IEN(RIIC0, EEI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_StartCondition
* Description  : This function generates I2C start condition
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_StartCondition(void)
{
    RIIC0.ICCR2.BIT.ST = 1U;    /* Set start condition flag */
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_StopCondition
* Description  : This function generates I2C stop condition
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_StopCondition(void)
{
    RIIC0.ICCR2.BIT.SP = 1U;    /* Set stop condition flag */
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Slave_Send
* Description  : This function writes data to a master device
* Arguments    : tx_buf -
*                    transmit buffer pointer
*                tx_num -
*                    transmit data length
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/

MD_STATUS R_Config_RIIC0_Slave_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;
    g_riic0_tx_count = tx_num;
    gp_riic0_tx_address = tx_buf;
    RIIC0.ICMR3.BYTE |= 0x70U;
    RIIC0.ICMR3.BIT.ACKWP = 1U;
    RIIC0.ICMR3.BIT.ACKBT = 0U;

    g_riic0_mode_flag = _17_IIC_SLAVE_TRANSMIT;
    g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Slave_Receive
* Description  : This function reads data from a master device
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    receive data length
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/

MD_STATUS R_Config_RIIC0_Slave_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;
    g_riic0_rx_length = rx_num;
    g_riic0_rx_count = 0U;
    gp_riic0_rx_address = rx_buf;
    RIIC0.ICMR3.BYTE |= 0x70U;
    RIIC0.ICMR3.BIT.ACKWP = 1U;
    RIIC0.ICMR3.BIT.ACKBT = 0U;
    g_riic0_dummy_read_count = 0U;

    g_riic0_mode_flag = _16_IIC_SLAVE_RECEIVE;
    g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;
    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
