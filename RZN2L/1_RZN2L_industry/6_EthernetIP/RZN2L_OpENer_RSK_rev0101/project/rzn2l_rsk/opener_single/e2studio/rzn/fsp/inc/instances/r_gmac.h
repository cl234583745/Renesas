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
 * @addtogroup ETHER
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHER_H
#define R_ETHER_H

#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_cfg.h"
#include "r_ether_api.h"
#include "r_ethsw_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define GMAC_CODE_VERSION_MAJOR    (1U)
#define GMAC_CODE_VERSION_MINOR    (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_gmac_port_mask
{
    GMAC_PORT_MASK_0   = (1U << 0),
    GMAC_PORT_MASK_1   = (1U << 1),
    GMAC_PORT_MASK_2   = (1U << 2),
    GMAC_PORT_MASK_ALL = (GMAC_PORT_MASK_0 | GMAC_PORT_MASK_1 | GMAC_PORT_MASK_2)
} gmac_port_mask_t;

/** GMAC descriptor as defined in the hardware manual. */
typedef struct st_gmac_instance_descriptor
{
    volatile uint32_t des0;            ///< TDES0/RDES0
    volatile uint32_t des1;            ///< TDES1/RDES1
    volatile uint32_t des2;            ///< TDES2/RDES2
    volatile uint32_t des3;            ///< TDES3/RDES3
} gmac_instance_descriptor_t;

typedef enum e_gmac_link_change
{
    GMAC_LINK_CHANGE_NO_CHANGE = 0,    ///< Link status is no change
    GMAC_LINK_CHANGE_LINK_DOWN = 1,    ///< Link status changes to down
    GMAC_LINK_CHANGE_LINK_UP   = 2,    ///< Link status changes to up
} gmac_link_change_t;

typedef enum e_gmac_magic_packet
{
    GMAC_MAGIC_PACKET_NOT_DETECTED = 0, ///< Magic packet is not detected
    GMAC_MAGIC_PACKET_DETECTED     = 1, ///< Magic packet is detected
} gmac_magic_packet_t;

typedef enum e_gmac_phylink
{
    GMAC_PHYLINK_DISABLE = 0,          ///< Disable
    GMAC_PHYLINK_ENABLE  = 1           ///< Enable
} gmac_phylink_t;

typedef struct st_gmac_extend_cfg
{
    gmac_instance_descriptor_t * p_rx_descriptors;                              ///< Receive descriptor buffer pool
    gmac_instance_descriptor_t * p_tx_descriptors;                              ///< Transmit descriptor buffer pool

    gmac_phylink_t phylink;                                                     ///< Enable or disable link status change by PHYLINK

    IRQn_Type pmt_irq;                                                          ///< PMT_interrupt number
    uint32_t  pmt_interrupt_priority;                                           ///< PMT_interrupt priority

    ether_phy_instance_t const *(*pp_phy_instance)[BSP_FEATURE_GMAC_MAX_PORTS]; ///< Pointer to ETHER_PHY instance
    ethsw_instance_t const * p_ethsw_instance;                                  ///< Pointer to ETHER_SWITCH instance
} gmac_extend_cfg_t;

/** ETHER control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_api_t::open is called. */
typedef struct st_gmac_instance_ctrl
{
    uint32_t open;                                ///< Used to determine if the channel is configured

    /* Configuration of ethernet module. */
    ether_cfg_t const * p_gmac_cfg;               ///< Pointer to initial configurations.

    /* Buffer of ethernet module. */
    gmac_instance_descriptor_t * p_rx_descriptor; ///< Pointer to the currently referenced transmit descriptor
    gmac_instance_descriptor_t * p_tx_descriptor; ///< Pointer to the currently referenced receive descriptor

    /* Interface for PHY-LSI chip. */
    void * p_reg_gmac;                            ///< Base register of ethernet controller for this channel

    /* Link status */
    gmac_port_mask_t link_status;                 ///< link status by callback from ethsw (bit0:port0, bit1:port1,..)

    /* Status of ethernet driver. */
    gmac_port_mask_t previous_link_status;        ///< Previous link status (bit0:port0, bit1:port1,..)

    gmac_magic_packet_t magic_packet;             ///< status of magic packet detection

    gmac_port_mask_t link_establish_status;       ///< Current Link status (bit0:port0, bit1:port1,..)

    uint32_t local_pause_bits;                    ///< Local pause bits got from PHY.
    uint32_t partner_pause_bits;                  ///< Partner pause bits got from PHY.
} gmac_instance_ctrl_t;

/*
 * PauseMaskE, PauseValE and pause_resolutionS are use to create
 * PAUSE resolution Table 28B-3 in IEEE 802.3-2008 standard.
 */
typedef enum e_gmac_pause_mask
{
    GMAC_PAUSE_MASK0,
    GMAC_PAUSE_MASK1,
    GMAC_PAUSE_MASK2,
    GMAC_PAUSE_MASK3,
    GMAC_PAUSE_MASK4,
    GMAC_PAUSE_MASK5,
    GMAC_PAUSE_MASK6,
    GMAC_PAUSE_MASK7,
    GMAC_PAUSE_MASK8,
    GMAC_PAUSE_MASK9,
    GMAC_PAUSE_MASKA,
    GMAC_PAUSE_MASKB,
    GMAC_PAUSE_MASKC,
    GMAC_PAUSE_MASKD,
    GMAC_PAUSE_MASKE,
    GMAC_PAUSE_MASKF
} gmac_pause_mask_t;

typedef enum e_gmac_pause_val
{
    GMAC_PAUSE_VAL0,
    GMAC_PAUSE_VAL1,
    GMAC_PAUSE_VAL2,
    GMAC_PAUSE_VAL3,
    GMAC_PAUSE_VAL4,
    GMAC_PAUSE_VAL5,
    GMAC_PAUSE_VAL6,
    GMAC_PAUSE_VAL7,
    GMAC_PAUSE_VAL8,
    GMAC_PAUSE_VAL9,
    GMAC_PAUSE_VALA,
    GMAC_PAUSE_VALB,
    GMAC_PAUSE_VALC,
    GMAC_PAUSE_VALD,
    GMAC_PAUSE_VALE,
    GMAC_PAUSE_VALF
} gmac_pause_val_t;

typedef struct st_gmac_pause_resolution
{
    gmac_pause_mask_t mask;
    gmac_pause_val_t  value;
    uint8_t           transmit;
    uint8_t           receive;
} gmac_pause_resolution_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_api_t g_ether_on_gmac;

/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_GMAC_Open(ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg);

fsp_err_t R_GMAC_Close(ether_ctrl_t * const p_ctrl);

fsp_err_t R_GMAC_Read(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes);

fsp_err_t R_GMAC_BufferRelease(ether_ctrl_t * const p_ctrl);

fsp_err_t R_GMAC_RxBufferUpdate(ether_ctrl_t * const p_ctrl, void * const p_buffer);

fsp_err_t R_GMAC_Write(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length);

fsp_err_t R_GMAC_LinkProcess(ether_ctrl_t * const p_ctrl);

fsp_err_t R_GMAC_GetLinkStatus(ether_ctrl_t * const p_ctrl, uint8_t port, ether_link_status_t * p_status);

fsp_err_t R_GMAC_WakeOnLANEnable(ether_ctrl_t * const p_ctrl);

fsp_err_t R_GMAC_TxStatusGet(ether_ctrl_t * const p_ctrl, void * const p_buffer_address);

fsp_err_t R_GMAC_VersionGet(fsp_version_t * const p_version);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHER_H
