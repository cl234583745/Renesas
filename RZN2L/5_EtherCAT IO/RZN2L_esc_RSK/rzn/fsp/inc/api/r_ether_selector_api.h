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
 * @defgroup ETHER_SELECTOR_API Ethernet Selector Interface
 * @brief Interface for Ethernet Selector functions.
 *
 * @section ETHER_SELECTOR_API_Summary Summary
 * The Ethernet Selector module (r_ether_selector) provides an API for standard Ethernet Selector communications applications that use
 * the Ethernet Subsystem peripheral.
 *
 * The Ethernet Selector interface supports the following features:
 * - Ethernet port select support
 * - RMII/RGMII converter control support
 * - PHY link signal polarity control support
 *
 * Implemented by:
 * - @ref ETHER_SELECTOR
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_SELECTOR_API_H
#define R_ETHER_SELECTOR_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_ether_selector_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_SELECTOR_API_VERSION_MAJOR    (1U)
#define ETHER_SELECTOR_API_VERSION_MINOR    (0U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** PHY link signal polarity */
typedef enum e_ether_selector_phylink_polarity
{
    ETHER_SELECTOR_PHYLINK_POLARITY_LOW  = 0, ///< Active-Low link signal
    ETHER_SELECTOR_PHYLINK_POLARITY_HIGH = 1, ///< Active-High link signal
} ether_selector_phylink_polarity_t;

/** RMII/RGMII converter mode */
typedef enum e_ether_selector_interface
{
    ETHER_SELECTOR_INTERFACE_MII   = 0, ///< MII
    ETHER_SELECTOR_INTERFACE_RMII  = 1, ///< RMII
    ETHER_SELECTOR_INTERFACE_GMII  = 2, ///< GMII
    ETHER_SELECTOR_INTERFACE_RGMII = 3, ///< RGMII
} ether_selector_interface_t;

/** RMII/RGMII converter speed */
typedef enum e_ether_selector_speed
{
    ETHER_SELECTOR_SPEED_10MBPS   = 0, ///< 10Mbit/s
    ETHER_SELECTOR_SPEED_100MBPS  = 1, ///< 100Mbit/s
    ETHER_SELECTOR_SPEED_1000MBPS = 2, ///< 1000Mbit/s
} ether_selector_speed_t;

/** RMII/RGMII converter duplex */
typedef enum e_ether_selector_duplex
{
    ETHER_SELECTOR_DUPLEX_HALF = 0,    ///< Half-Duplex
    ETHER_SELECTOR_DUPLEX_FULL = 1,    ///< Full-Duplex
} ether_selector_duplex_t;

/** RMII/RGMII converter reference clock */
typedef enum e_ether_selector_ref_clock
{
    ETHER_SELECTOR_REF_CLOCK_INPUT  = 0, ///< REF_CLK input
    ETHER_SELECTOR_REF_CLOCK_OUTPUT = 1, ///< REF_CLK outpt
} ether_selector_ref_clock_t;

/** Control block.  Allocate an instance specific control block to pass into the API calls.
 * @par Implemented as
 * - ether_selector_instance_ctrl_t
 */
typedef void ether_selector_ctrl_t;

/** Configuration parameters. */
typedef struct st_ether_selector_cfg
{
    uint8_t port;                              ///< Port number
    ether_selector_phylink_polarity_t phylink; ///< PHY link signal polarity

    ether_selector_interface_t interface;      ///< Converter mode
    ether_selector_speed_t     speed;          ///< Converter Speed
    ether_selector_duplex_t    duplex;         ///< Converter Duplex
    ether_selector_ref_clock_t ref_clock;      ///< Converter REF_CLK
    void const               * p_extend;       ///< Placeholder for user extension.
} ether_selector_cfg_t;

/** Functions implemented at the HAL layer will follow this API. */
typedef struct st_ether_selector_api
{
    /** Open driver. Set Ethernet mode such as Ethernet MAC or Ethernet Switch or EtherCAT for each port.
     * @par Implemented as
     * - @ref R_ETHER_SELECTOR_Open()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_cfg            Pointer to selector configuration structure.
     */
    fsp_err_t (* open)(ether_selector_ctrl_t * const p_api_ctrl, ether_selector_cfg_t const * const p_cfg);

    /** Close driver.
     * @par Implemented as
     * - @ref R_ETHER_SELECTOR_Close()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(ether_selector_ctrl_t * const p_api_ctrl);

    /** Set the Converter speed and duplex in runtime.
     * @par Implemented as
     * - @ref R_ETHER_SELECTOR_ConverterSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  speed            Converter speed.
     * @param[in]  duplex           Converter duplex mode.
     */
    fsp_err_t (* converterSet)(ether_selector_ctrl_t * const p_api_ctrl, ether_selector_speed_t speed,
                               ether_selector_duplex_t duplex);

    /** Return the version of the driver.
     * @par Implemented as
     * - @ref R_ETHER_SELECTOR_VersionGet()
     *
     * @param[out] p_data       Memory address to return version information to.
     */
    fsp_err_t (* versionGet)(fsp_version_t * const p_data);
} ether_selector_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ether_selector_instance
{
    ether_selector_ctrl_t      * p_ctrl; ///< Pointer to the control structure for this instance
    ether_selector_cfg_t const * p_cfg;  ///< Pointer to the configuration structure for this instance
    ether_selector_api_t const * p_api;  ///< Pointer to the API structure for this instance
} ether_selector_instance_t;

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_SELECTOR_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHER_SELECTOR_API_H */
