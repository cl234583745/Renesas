/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#ifndef OPENER_BOARD_IO_PIN_H_
#define OPENER_BOARD_IO_PIN_H_

/***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************/
#include "bsp_api.h"
   
/**********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

#define BOARD_PIN_DISABLED         (-1)

#if defined(BOARD_RZT2M_RSK)
/**
 * Settings For RZ/T2M RSK board
 */
/**
 * LED0     GREEN   P19_6
 * LED1     YELLOW  P19_4
 * LED2     RED     P20_0
 * LED3     RED     P23_4
 *
 * The following order complies the order on board from Ether Port side.
 * ETH_LED4 RED     P19_7   MODULE_RED      ( Need to short CN4 )
 * ETH_LED2 GREEN   P20_1   MODULE_GREEN
 * ETH_LED0 GREEN   P20_2
 * ETH_LED3 GREEN   P20_4
 * ETH_LED5 GREEN   P21_0   NETWORK_GREEN
 * ETH_LED1 RED     P20_3   NETWORK_RED
 * ETH_LED7 Y-GREEN P08_1
 * ETH_LED6 RED     P08_2   MODULE_RED      ( Alternate of ETH_LED4 if CN4 is not shorted )
 */
#define BOARD_IOLEVEL_LED_ON      (BSP_IO_LEVEL_HIGH)
#define BOARD_IOLEVEL_LED_OFF     (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_ON       (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_OFF      (BSP_IO_LEVEL_HIGH)

#define BOARD_LED0_GREEN   (BSP_IO_PORT_19_PIN_6)
#define BOARD_LED1_YELLOW  (BSP_IO_PORT_19_PIN_4)
#define BOARD_LED2_RED     (BSP_IO_PORT_20_PIN_0)
#define BOARD_LED3_RED     (BSP_IO_PORT_23_PIN_4)

#define BOARD_SW3_PIN0     (BSP_IO_PORT_11_PIN_0)
#define BOARD_SW3_PIN1     (BSP_IO_PORT_11_PIN_3)
#define BOARD_SW3_PIN2     (BSP_IO_PORT_11_PIN_4)
#define BOARD_SW3_PIN3     (BSP_IO_PORT_11_PIN_6)
#define BOARD_SW3_PIN4     (BSP_IO_PORT_10_PIN_6)
#define BOARD_SW3_PIN5     (BSP_IO_PORT_13_PIN_2)
#define BOARD_SW3_PIN6     (BSP_IO_PORT_13_PIN_7)
#define BOARD_SW3_PIN7     (BSP_IO_PORT_14_PIN_1)

#define BOARD_ETH_LED0_GREEN           (BSP_IO_PORT_20_PIN_2)
#define BOARD_ETH_LED1_RED             (BSP_IO_PORT_20_PIN_3)
#define BOARD_ETH_LED2_GREEN           (BSP_IO_PORT_20_PIN_1)
#define BOARD_ETH_LED3_GREEN           (BSP_IO_PORT_20_PIN_4)
#define BOARD_ETH_LED4_RED             (BSP_IO_PORT_19_PIN_7)
#define BOARD_ETH_LED5_GREEN           (BSP_IO_PORT_21_PIN_0)
#define BOARD_ETH_LED6_RED             (BSP_IO_PORT_08_PIN_2)
#define BOARD_ETH_LED7_YELLOW_GREEN    (BSP_IO_PORT_08_PIN_1)

#elif defined(BOARD_RZN2L_RSK)
/**
 * Settings For RZ/N2L RSK board
 */
/**
 * LED0     GREEN   P18_2
 * LED1     YELLOW  P22_3
 * LED2     RED     P04_1
 * LED3     RED     P17_3
 *
 * The following order complies the order on board from Ether Port side.
 * ETH_LED4 RED     P03_0   MODULE_RED
 * ETH_LED2 GREEN   P20_1   MODULE_GREEN
 * ETH_LED0 GREEN   P20_2
 * ETH_LED3 GREEN   P20_4
 * ETH_LED5 GREEN   P22_1   NETWORK_GREEN
 * ETH_LED1 RED     P20_3   NETWORK_RED
 * ETH_LED7 Y-GREEN P05_0
 * ETH_LED6 RED     P04_4   MODULE_RED
 */

#define BOARD_IOLEVEL_LED_ON      (BSP_IO_LEVEL_HIGH)
#define BOARD_IOLEVEL_LED_OFF     (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_ON       (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_OFF      (BSP_IO_LEVEL_HIGH)

#define BOARD_LED0_GREEN   (BSP_IO_PORT_18_PIN_2)
#define BOARD_LED1_YELLOW  (BSP_IO_PORT_22_PIN_3)
#define BOARD_LED2_RED     (BSP_IO_PORT_04_PIN_1)
#define BOARD_LED3_RED     (BSP_IO_PORT_17_PIN_3)

#define BOARD_SW3_PIN0     (BSP_IO_PORT_13_PIN_6)
#define BOARD_SW3_PIN1     (BSP_IO_PORT_13_PIN_5)
#define BOARD_SW3_PIN2     (BSP_IO_PORT_14_PIN_0)
#define BOARD_SW3_PIN3     (BSP_IO_PORT_13_PIN_7)

#define BOARD_ETH_LED0_GREEN           (BSP_IO_PORT_20_PIN_2)
#define BOARD_ETH_LED1_RED             (BSP_IO_PORT_20_PIN_3)
#define BOARD_ETH_LED2_GREEN           (BSP_IO_PORT_20_PIN_1)
#define BOARD_ETH_LED3_GREEN           (BSP_IO_PORT_20_PIN_4)
#define BOARD_ETH_LED4_RED             (BSP_IO_PORT_03_PIN_0)
#define BOARD_ETH_LED5_GREEN           (BSP_IO_PORT_22_PIN_1)
#define BOARD_ETH_LED6_RED             (BSP_IO_PORT_04_PIN_4)
#define BOARD_ETH_LED7_YELLOW_GREEN    (BSP_IO_PORT_05_PIN_0)

#elif defined(BOARD_RA6M3_EK)
/**
 * Settings for EK-RA6M3 board
 * (For prototype porting and example of hardware independency implementation.)
 */
#define BOARD_IOLEVEL_LED_ON    (BSP_IO_LEVEL_HIGH)
#define BOARD_IOLEVEL_LED_OFF   (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_ON     (BSP_IO_LEVEL_LOW)
#define BOARD_IOLEVEL_SW_OFF    (BSP_IO_LEVEL_HIGH)

/**
 * LED1 BLUE    P04_03
 * LED2 GREEN   P04_00
 * LED3 RED     P01_00
 */
#define BOARD_EKRA6M3_BLUE     ((bsp_io_port_pin_t) g_bsp_leds.p_leds[BSP_LED_LED1])
#define BOARD_EKRA6M3_GREEN    ((bsp_io_port_pin_t) g_bsp_leds.p_leds[BSP_LED_LED2])
#define BOARD_EKRA6M3_RED      ((bsp_io_port_pin_t) g_bsp_leds.p_leds[BSP_LED_LED3])

#else
/**
 * Settings
 */
#endif

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

#endif
