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
 * @ingroup RENESAS_INTERFACES
 * @defgroup ETHSW_API Ethernet Switch Interface
 * @brief Interface for Ethernet Switch functions.
 *
 * @section ETHSW_API_Summary Summary
 * The ETHSW module (r_ethsw) provides an API for Ethernet Switch that use the ETHSW peripheral.
 *
 * The ETHSW interface supports the following features:
 * - Link change check support
 *
 * Implemented by:
 * - @ref ETHSW
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHSW_API_H
#define R_ETHSW_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHSW_API_VERSION_MAJOR    (1U)
#define ETHSW_API_VERSION_MINOR    (0U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**  speed and duplex of the port */
typedef enum e_ethsw_link_speed
{
    ETHSW_LINK_SPEED_NO_LINK = 0,      ///< Link is not established
    ETHSW_LINK_SPEED_10H     = 1,      ///< Link status is 10Mbit/s and half duplex
    ETHSW_LINK_SPEED_10F     = 2,      ///< Link status is 10Mbit/s and full duplex
    ETHSW_LINK_SPEED_100H    = 3,      ///< Link status is 100Mbit/s and half duplex
    ETHSW_LINK_SPEED_100F    = 4,      ///< Link status is 100Mbit/s and full duplex
    ETHSW_LINK_SPEED_1000H   = 5,      ///< Link status is 1000Mbit/s and half duplex
    ETHSW_LINK_SPEED_1000F   = 6       ///< Link status is 1000Mbit/s and full duplex
} ethsw_link_speed_t;

/** Event code of callback function */
typedef enum e_ethsw_event
{
    ETHSW_EVENT_LINK_CHG               ///< Link status change detection event
} ethsw_event_t;

/** Enable/Disable Management Port Specific Frame Tagging */
typedef enum e_ethsw_specifig_tag
{
    ETHSW_SPECIFIC_TAG_DISABLE = 0,    ///< Disable
    ETHSW_SPECIFIC_TAG_ENABLE  = 1     ///< Enable
} ethsw_specifig_tag_t;

/** Enable/Disable PHYLINK Change Interrupt */
typedef enum e_ethsw_phylink
{
    ETHSW_PHYLINK_DISABLE = 0,         ///< Disable
    ETHSW_PHYLINK_ENABLE  = 1          ///< Enable
} ethsw_phylink_t;

/** Callback function parameter data */
typedef struct st_ethsw_callback_args
{
    uint32_t      channel;             ///< Device channel number
    ethsw_event_t event;               ///< Event code

    uint32_t status_link;              ///< Link status bit0:port0. bit1:port1. bit2:port2, bit3:port3

    void const * p_context;            ///< Placeholder for user data.  Set in @ref ethsw_api_t::open function in @ref ethsw_cfg_t.
} ethsw_callback_args_t;

/** Control block.  Allocate an instance specific control block to pass into the API calls.
 * @par Implemented as
 * - ethsw_instance_ctrl_t
 */
typedef void ethsw_ctrl_t;

/** Configuration parameters. */
typedef struct st_ethsw_cfg
{
    uint8_t channel;                                     ///< Channel

    ethsw_specifig_tag_t specific_tag;                   ///< Management port specific frmame tag
    uint32_t             specific_tag_id;                ///< Management port specific frmame tag ID

    ethsw_phylink_t phylink;                             ///< Enable or disable link status change by PHYLINK
    ///< Call callback function when enable

    IRQn_Type irq;                                       ///< MCU interrupt number
    uint8_t   ipl;                                       ///< MCU interrupt priority

    void (* p_callback)(ethsw_callback_args_t * p_args); ///< Callback provided when an ISR occurs.

    /** Placeholder for user data.  Passed to the user callback in ethsw_callback_args_t. */
    void const * p_context;
    void const * p_extend;                               ///< Placeholder for user extension.
} ethsw_cfg_t;

/** Functions implemented at the HAL layer will follow this API. */
typedef struct st_ethsw_api
{
    /** Open driver.
     * @par Implemented as
     * - @ref R_ETHSW_Open()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_cfg            Pointer to pin configuration structure.
     */
    fsp_err_t (* open)(ethsw_ctrl_t * const p_api_ctrl, ethsw_cfg_t const * const p_cfg);

    /** Close driver.
     * @par Implemented as
     * - @ref R_ETHSW_Close()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(ethsw_ctrl_t * const p_api_ctrl);

    /** Configs speed and duplex of the port.
     * @par Implemented as
     * - @ref R_ETHSW_SpeedCfg()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  spee             Speed and Duplex,
     */
    fsp_err_t (* speedCfg)(ethsw_ctrl_t * const p_api_ctrl, uint8_t const port, ethsw_link_speed_t const speed);

    /** Return the version of the driver.
     * @par Implemented as
     * - @ref R_ETHSW_VersionGet()
     *
     * @param[out] p_data           Memory address to return version information to.
     */
    fsp_err_t (* versionGet)(fsp_version_t * const p_data);
} ethsw_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ethsw_instance
{
    ethsw_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    ethsw_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    ethsw_api_t const * p_api;         ///< Pointer to the API structure for this instance
} ethsw_instance_t;

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHSW_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHSW_API_H */
