/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef BSP_RESET_H
#define BSP_RESET_H

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Key code for writing reset register. */
#define BSP_PRV_RESET_KEY            (0x4321A501U)
#define BSP_PRV_RESET_RELEASE_KEY    (0x00000000U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/** CPU to be reset target.*/
typedef enum e_bsp_reset
{
    BSP_RESET_CPU0 = 0,                ///< Software reset for CPU0
} bsp_reset_t;

/** The different types of registers that can control the reset of peripheral modules related to Ethernet. */
typedef enum e_bsp_module_reset
{
    /** Enables writing to the registers related to GMAC (ACLK clock domain) reset control. */
    BSP_MODULE_RESET_GMAC_ACLK = 0,

    /** Enables writing to the registers related to GMAC (HCLK clock domain) reset control. */
    BSP_MODULE_RESET_GMAC_HCLK,

    /** Enables writing to the registers related to ETHSW reset control. */
    BSP_MODULE_RESET_ETHSW,

    /** Enables writing to the registers related to ESC (HCLK clock domain) reset control. */
    BSP_MODULE_RESET_ESC_HCLK,

    /** Enables writing to the registers related to ESC (ESC_CLK clock domain) reset control. */
    BSP_MODULE_RESET_ESC_ESCCLK,

    /** Enables writing to the registers related to Ethernet accessary register reset control. */
    BSP_MODULE_RESET_ESC_ETH_ACCESSARY,

    /** Enables writing to the registers related to MII converter reset control. */
    BSP_MODULE_RESET_MII,
} bsp_module_reset_t;

/** @} (end addtogroup BSP_MCU) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
