/***********************************************************************************************************************
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riic_rx_pin_config.h
 * Description  : Pin configures the RIIC drivers
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 04.03.2016 1.90     First Release
 *         : 02.06.2017 2.10     Deleted RIIC port definitions of RIIC3.
 *         : 30.10.2017 2.30     Port K is added.
 *         : 15.03.2025 3.01     Updated disclaimer.
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIIC_PIN_CONFIG_H
    #define RIIC_PIN_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
/*------------------------------------------------------------------------------*/
/*   Set using port as riic port                                                */
/*------------------------------------------------------------------------------*/
/* Set using port as riic port.                                                                 */
/* If you want to include the port configuration process(RIIC_CFG_PORT_SET_PROCESSING is "1"),  */
/* please choose which ports to use for the SCL/SDA of RIIC with the following setting.         */
/* Select the port group and pin used by setting
 "R_RIIC_CFG_RIICx_SCLx_PORT (select from port group 0 to K)"
 and "R_RIIC_CFG_RIICx_SCLx_BIT (select from pin number 0 to 7)"
 and "R_RIIC_CFG_RIICx_SDAx_PORT (select from port group 0 to K)"
 and "R_RIIC_CFG_RIICx_SDAx_BIT (select from pin number 0 to 7)",
 respectively. */

/* Select the ports (SCL0 and SDA0) to use in RIIC0 */
    #define R_RIIC_CFG_RIIC0_SCL0_PORT   '1'     /* Port Number */
    #define R_RIIC_CFG_RIIC0_SCL0_BIT    '6'     /* Bit Number  */

    #define R_RIIC_CFG_RIIC0_SDA0_PORT   '1'     /* Port Number */
    #define R_RIIC_CFG_RIIC0_SDA0_BIT    '7'     /* Bit Number  */

/* Select the ports (SCL1 and SDA1) to use in RIIC1 */
    #define R_RIIC_CFG_RIIC1_SCL1_PORT   '2'     /* Port Number */
    #define R_RIIC_CFG_RIIC1_SCL1_BIT    '1'     /* Bit Number  */

    #define R_RIIC_CFG_RIIC1_SDA1_PORT   '2'     /* Port Number */
    #define R_RIIC_CFG_RIIC1_SDA1_BIT    '0'     /* Bit Number  */

/* Select the ports (SCL2 and SDA2) to use in RIIC2 */
    #define R_RIIC_CFG_RIIC2_SCL2_PORT   '1'     /* Port Number */
    #define R_RIIC_CFG_RIIC2_SCL2_BIT    '6'     /* Bit Number  */

    #define R_RIIC_CFG_RIIC2_SDA2_PORT   '1'     /* Port Number */
    #define R_RIIC_CFG_RIIC2_SDA2_BIT    '7'     /* Bit Number  */

#endif /* RIIC_PIN_CONFIG_H */

