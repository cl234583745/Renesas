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
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_PHY_H
#define R_ETHER_PHY_H

#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_phy_cfg.h"
#include "r_ether_phy_api.h"
#include "r_ether_selector.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_PHY_CODE_VERSION_MAJOR    (1U)
#define ETHER_PHY_CODE_VERSION_MINOR    (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ETHER PHY control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_phy_api_t::open is called. */
typedef struct st_ether_phy_instance_ctrl
{
    uint32_t open;                           ///< Used to determine if the channel is configured

    /* Configuration of Ethernet PHY-LSI module. */
    ether_phy_cfg_t const * p_ether_phy_cfg; ///< Pointer to initial configurations.

    /* Interface for PHY-LSI chip. */
    volatile uint32_t * p_reg_etherc;        ///< Pointer to ETHERC peripheral registers.

    /* The capabilities of the local link as PHY data */
    uint32_t local_advertise;                ///< Capabilities bitmap for local advertising.
} ether_phy_instance_ctrl_t;

/** Identify Ethernet and EtherCAT */
typedef enum e_ether_phy_port_type
{
    ETHER_PHY_PORT_TYPE_ETHERNET  = 0, ///< Ethernet
    ETHER_PHY_PORT_TYPE_ETHER_CAT = 1, ///< EtherCAT
} ether_phy_port_type_t;

/** Identify the access interface of the PHY register */
typedef enum e_ether_phy_mdio
{
    ETHER_PHY_MDIO_GMAC  = 0,          ///< GMAC
    ETHER_PHY_MDIO_ETHSW = 1,          ///< Ether switch
    ETHER_PHY_MDIO_ESC   = 2           ///< EtherCAT
} ether_phy_mdio_t;

/** Identify PHY-LSI */
typedef enum e_ether_phy_chip
{
    ETHER_PHY_CHIP_VSC8541 = (1 << 0), ///< VSC8541
    ETHER_PHY_CHIP_KSZ9131 = (1 << 1), ///< KSZ9131
    ETHER_PHY_CHIP_KSZ9031 = (1 << 2), ///< KSZ9031
    ETHER_PHY_CHIP_KSZ8081 = (1 << 3), ///< KSZ8081
    ETHER_PHY_CHIP_KSZ8041 = (1 << 4)  ///< KSZ8041
} ether_phy_chip_t;

/** PHY Speed for setting */
typedef enum e_ether_phy_speed
{
    ETHER_PHY_SPEED_10_100  = 0,       ///< 100/10Mbps      (Note:Can be specified when auto-negotiation is ON)
    ETHER_PHY_SPEED_10_1000 = 1,       ///< 1000/100/10Mbps (Note:Can be specified when auto-negotiation is ON)
    ETHER_PHY_SPEED_10      = 2,       ///< 10Mbps
    ETHER_PHY_SPEED_100     = 3,       ///< 100Mbps
    ETHER_PHY_SPEED_1000    = 4,       ///< 1000Mbps
} ether_phy_speed_t;

/** PHY Duplex for setting */
typedef enum e_ether_phy_duplex
{
    ETHER_PHY_DUPLEX_HALF = 0,         ///< Half duplex
    ETHER_PHY_DUPLEX_FULL = 1,         ///< Full duplex
} ether_phy_duplex_t;

/** PHY Auto Negotiation for setting */
typedef enum e_ether_phy_auto_negotiation
{
    ETHER_PHY_AUTO_NEGOTIATION_OFF = 0, ///< Auto Negotiation is ON
    ETHER_PHY_AUTO_NEGOTIATION_ON  = 1, ///< Auto Negotiation is OFF
} ether_phy_auto_negotiation_t;

/** Extended configuration */
typedef struct s_ether_phy_extend_cfg
{
    ether_phy_port_type_t port_type;                 ///< Port type
    ether_phy_chip_t      phy_chip;                  ///< PHY chip type
    ether_phy_mdio_t      mdio_type;                 ///< MDIO type

    ether_phy_speed_t            bps;                ///< PHY Speed
    ether_phy_duplex_t           duplex;             ///< PHY Duplex
    ether_phy_auto_negotiation_t auto_negotiation;   ///< Auto Negotiation ON/OFF

    bsp_io_port_pin_t phy_reset_pin;                 ///< PHY reset pin
    uint32_t          phy_reset_time;                ///< PHY reset assert time in millsecond

    ether_selector_instance_t * p_selector_instance; ///< Instance of selector driver
} ether_phy_extend_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_phy_api_t g_ether_phy_on_ether_phy;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open(ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_PHY_Close(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_StartAutoNegotiate(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet(ether_phy_ctrl_t * const p_ctrl,
                                            uint32_t * const         p_line_speed_duplex,
                                            uint32_t * const         p_local_pause,
                                            uint32_t * const         p_partner_pause);

fsp_err_t R_ETHER_PHY_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_VersionGet(fsp_version_t * const p_version);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHER_PHY_H
