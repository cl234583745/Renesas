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

/*******************************************************************************************************************//**
 * @addtogroup ETHSW
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHSW_H
#define R_ETHSW_H

#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ethsw_cfg.h"
#include "r_ethsw_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHSW_CODE_VERSION_MAJOR    (1U)
#define ETHSW_CODE_VERSION_MINOR    (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ETHER SWITCH control block. DO NOT INITIALIZE.  Initialization occurs when @ref ethsw_api_t::open is called. */
typedef struct st_ethsw_instance_ctrl
{
    uint32_t open;                     ///< Used to determine if the channel is configured

    /* Configuration of Ethernet SWITCH-LSI module. */
    ethsw_cfg_t const * p_switch_cfg;  ///< Pointer to initial configurations.

    /* Base register of Ethernet SWITCH-LSI chip. */
    volatile uint32_t * p_reg_switch;  ///< Pointer to ETHERC peripheral registers.
} ethsw_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ethsw_api_t g_ethsw_on_ethsw;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_Open(ethsw_ctrl_t * const p_ctrl, ethsw_cfg_t const * const p_cfg);

fsp_err_t R_ETHSW_Close(ethsw_ctrl_t * const p_ctrl);

fsp_err_t R_ETHSW_SpeedCfg(ethsw_ctrl_t * const p_ctrl, uint8_t const port, ethsw_link_speed_t const speed);

fsp_err_t R_ETHSW_VersionGet(fsp_version_t * const p_version);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHSW)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHSW_H
