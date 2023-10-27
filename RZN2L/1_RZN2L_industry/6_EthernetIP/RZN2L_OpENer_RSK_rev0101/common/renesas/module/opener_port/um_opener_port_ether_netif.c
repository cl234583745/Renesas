/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_opener_port_api.h"
#include "um_opener_port_cfg.h"
#include "um_opener_port.h"
#include "um_opener_port_internal.h"
#include "um_common_api.h"

#include "um_ether_netif_api.h"

/** For getting extend configuration */
#include "r_gmac.h"
#include "r_ether_phy.h"
#include "r_ether_phy_api.h"

/** Ethernet link object. */
#include "cipethernetlink.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * For instance attribute #2 interface capability
 */
#define _ETHLINK_IFFLAG_LINK_STATUS_BIT                    ((uint32_t)0x01<<0)   /** 1: Link Enable, 0: Link Disable */
#define _ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT               ((uint32_t)0x01<<1)   /** 1: Full duplex, 0: Half duplex */
#define _ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_SHIFT       (2)   /** See opener_port_negotiation_status_t */
#define _ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_MASK        ((uint32_t)0x07<<_ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_SHIFT)
#define _ETHLINK_IFFLAG_MANUAL_SETTING_REQUIRES_RESET_BIT  ((uint32_t)0x01<<5) /** Duplicated in Interface Capability #11 Attribute to retain backwards compatibility. */
#define _ETHLINK_IFFLAG_LOCAL_HARDWARE_FAULT_BIT           ((uint32_t)0x01<<6) /** 1: Hardware fault is detected (product specific), 0: not detected. */

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_opener_port_negotiation_status
{
    _NEGOTIATION_STATUS_IN_PROGRESS = 0,                     ///< OPENER_PORT_NEGOTIATION_STATUS_IN_PROGRESS
    _NEGOTIATION_STATUS_FAILED_AND_SPEED_DETECTION_FAILED,   ///< Using default values for speed and duplex.
    _NEGOTIATION_STATUS_FAILED_BUT_SPEED_DETECTION_SUCCESS,  ///< Using default values for speed and duplex.
    _NEGOTIATION_STATUS_SUCCESS,                             ///< OPENER_PORT_NEGOTIATION_STATUS_SUCCESS
    _NEGOTIATION_STATUS_IS_NOT_ATTEMPT,                      ///< OPENER_PORT_NEGOTIATION_STATUS_IS_NOT_ATTEMPT
} opener_port_negotiation_status_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static usr_err_t _open_cip_ethlink_instance( opener_port_ether_netif_ctrl_t * const p_ctrl );
static usr_err_t _init_cip_ethlink_instance_attributes( opener_port_ether_netif_ctrl_t * const p_ctrl );
static usr_err_t _update_cip_ethlink_interface_speed_and_duplex_flags( ether_phy_instance_t const * p_ether_phy_instance,
                                                                       CipEthernetLinkObject * p_ethlink_cip_instance );
static usr_err_t _set_cip_ethlink_autonegotiaion_status( CipEthernetLinkObject * const p_ethlink_object, opener_port_negotiation_status_t status );
static void      _ether_netif_callback( ether_netif_callback_args_t * p_args );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED      Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_ether_netif_open( opener_port_ether_netif_ctrl_t * const p_ctrl,
                                           ether_netif_instance_t const * const p_ether_netif_instance )
{
    /** Error codes*/
    usr_err_t usr_err;

    /** Temporary link status */
    uint32_t link_status;

    /** Set target module instance. */
    p_ctrl->p_ether_netif_instance = p_ether_netif_instance;

    /** Open target module instance. */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->open(p_ctrl->p_ether_netif_instance->p_ctrl, p_ctrl->p_ether_netif_instance->p_cfg);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err || USR_ERR_ALREADY_OPEN == usr_err || USR_ERR_ALREADY_RUNNING == usr_err, USR_ERR_NOT_INITIALIZED);

    /** Setup CIP Ethernet link object*/
    usr_err = _open_cip_ethlink_instance( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Initialize CIP Ethernet link object attributes . */
    usr_err = _init_cip_ethlink_instance_attributes( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Add callback function. */
    p_ctrl->callback_node.p_func = _ether_netif_callback;
    p_ctrl->callback_node.p_memory = &(p_ctrl->callback_memory);
    p_ctrl->callback_node.p_context = p_ctrl;
    usr_err = p_ctrl->p_ether_netif_instance->p_api->callbackAdd( p_ctrl->p_ether_netif_instance->p_ctrl, &p_ctrl->callback_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Get link status with checking callback. */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->linkStatusGet( p_ctrl->p_ether_netif_instance->p_ctrl, &link_status, true );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start target module.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_ether_netif_start( opener_port_ether_netif_ctrl_t * const p_ctrl )
{
    /** Error codes. */
    usr_err_t usr_err;

    /** Start target module */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->start( p_ctrl->p_ether_netif_instance->p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

    /** Wait for first callback. */
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop target module.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_ether_netif_stop( opener_port_ether_netif_ctrl_t * const p_ctrl )
{
    /** Error codes. */
    usr_err_t usr_err;

    /** Stop target module. */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->stop(p_ctrl->p_ether_netif_instance->p_ctrl);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

    /** Return success code. */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Setup CIP Ethernet link instance with module controller.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _open_cip_ethlink_instance( opener_port_ether_netif_ctrl_t * const p_ctrl )
{
    /** For scanning ports */
    uint8_t port = 0;
    uint8_t cnt_instance = 0;

    /** Get gmac extend configuration. */
    gmac_extend_cfg_t const * p_gmac_extend_cfg = p_ctrl->p_ether_netif_instance->p_cfg->p_ether_instance->p_cfg->p_extend;

    /** Set the Ethernet Link object address to controller */
    for( port = 0; port < BSP_FEATURE_GMAC_MAX_PORTS; port++ )
    {
        /** Get PHY instance */
        p_ctrl->p_ether_phy_instance[port] = (*p_gmac_extend_cfg->pp_phy_instance)[port];

        /** If the PHY instance is NULL, */
        if( NULL == p_ctrl->p_ether_phy_instance[port] )
        {
            /** Set the port to NULL not to assign the object. */
            p_ctrl->p_ethlink_cip_instance[port]  = NULL;
            continue;
        }

        /** Set the pointer of Ethernet Link CIP object instance */
        p_ctrl->p_ethlink_cip_instance[port] = &g_ethernet_link[cnt_instance];

        /** Increment index of Ethernet Link instance. */
        cnt_instance++;

        /** If instances do NOT remains, */
        if( cnt_instance >= OPENER_ETHLINK_INSTANCE_CNT )
        {
            /** Stop the assignment of Ethernet link object. */
            break;
        }
    }

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Initialize CIP Ethernet link attributes
 *
 * NOTE:
 *  This function initialize all attributes even if the attributes have already initialized on definition or on CipStackInit() function.
 *  This function should be called after the CIP stack initialization to overwrite attributes by the CIP initialization.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _init_cip_ethlink_instance_attributes( opener_port_ether_netif_ctrl_t * const p_ctrl )
{
    /** For scanning ports */
    uint32_t port;

    /** For resolving types */
    ether_phy_instance_t const * p_ether_phy_instance;
    ether_phy_extend_cfg_t const * p_ether_phy_extend_cfg;
    CipEthernetLinkObject * p_ethlink_cip_instance;

    /** Scan all ports. */
    for( port = 0; port < BSP_FEATURE_GMAC_MAX_PORTS ; port++ )
    {
        /** If the pointer is NULL, continue. */
        if( NULL == p_ctrl->p_ether_phy_instance[port] || NULL == p_ctrl->p_ethlink_cip_instance[port] )
        {
            continue;
        }

        /** Resolve the types */
        p_ether_phy_instance = p_ctrl->p_ether_phy_instance[port];
        p_ethlink_cip_instance = p_ctrl->p_ethlink_cip_instance[port];

        /** Resolve extended configuration of PHY instance. */
        p_ether_phy_extend_cfg = p_ether_phy_instance->p_cfg->p_extend;

        /** Initialize the interface speed (#1) and flags (#2) which are update in callback function. */
        p_ethlink_cip_instance->interface_speed = 0;
        p_ethlink_cip_instance->interface_flags = 0;

        /** Set the MAC address (#3) */
        (void) CipEthernetLinkSetMac( (EipUint8 *) p_ctrl->p_ether_netif_instance->p_cfg->p_ether_instance->p_cfg->p_mac_address );

        /** Set the interface type (#7) */
        p_ethlink_cip_instance->interface_type = kEthLinkIfTypeTwistedPair;

#if ( (OPENER_ETHLINK_INSTANCE_CNT > (1)) || ((1) == OPENER_ETHLINK_LABEL_ENABLE) )
        /**
         * TODO: Explicitly initialize g_ethernet_link[port].interface_label (#10) when multiple ports are enabled (OPENER_ETHLINK_INSTANCE_CNT > 1)
         */
#endif

        /** Initialize the interface capability (#11), and set Auto MDX capability. */
        p_ethlink_cip_instance->interface_caps.capability_bits = kEthLinkCapAutoMDX;  /** TODO: is it right? */
        p_ethlink_cip_instance->interface_caps.speed_duplex_selector = 0;             /** Set to 0 because the #6 is NOT supported. */

        /** Settings with/without auto-negotiation. */
        switch ( p_ether_phy_extend_cfg->auto_negotiation )
        {
        case ETHER_PHY_AUTO_NEGOTIATION_ON:
            /** Add the auto-negotiation capability to the interface capability (#11). */
            p_ethlink_cip_instance->interface_caps.capability_bits |= kEthLinkCapAutoNeg;
            break;

        case ETHER_PHY_AUTO_NEGOTIATION_OFF:
            /** Set auto-negotiation status. */
            (void) _set_cip_ethlink_autonegotiaion_status(&g_ethernet_link[port], _NEGOTIATION_STATUS_IS_NOT_ATTEMPT);
            break;
        }
    }

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Callback handler for Ethernet network interface module
 *
 * @param[in] p_args                    Pointer to arguments of callback.
 **********************************************************************************************************************/
void _ether_netif_callback( ether_netif_callback_args_t * p_args )
{
    /** Resolve user context */
    opener_port_ether_netif_ctrl_t * p_ctrl = ( opener_port_ether_netif_ctrl_t * ) p_args->p_context;
    (void) p_ctrl;

    /** For scanning ports */
    uint8_t port = 0;

    /** For resolving types */
    ether_phy_instance_t const * p_ether_phy_instance;
    ether_phy_extend_cfg_t const * p_ether_phy_extend_cfg;
    CipEthernetLinkObject * p_ethlink_cip_instance;

    /** Check event */
    switch( p_args->event )
    {
    case ETHER_NETIF_CALLBACK_EVENT_ANY_PORT_LINK_STATUS_CHANGED:

        for( port = 0; port < BSP_FEATURE_GMAC_MAX_PORTS ; port++ )
        {
            /** If the pointer is NULL, continue */
            if( NULL == p_ctrl->p_ether_phy_instance[port] || NULL == p_ctrl->p_ethlink_cip_instance[port] )
            {
                continue;
            }

            /** Resolve the types */
            p_ether_phy_instance = p_ctrl->p_ether_phy_instance[port];
            p_ethlink_cip_instance = p_ctrl->p_ethlink_cip_instance[port];

            /** Resolve extended configuration of PHY instance. */
            p_ether_phy_extend_cfg = p_ether_phy_instance->p_cfg->p_extend;

            if( p_args->port_link_status & ETHER_NETIF_PORT_BIT(port) )
            {
                /** Enable link status flag (in #2) */
                p_ethlink_cip_instance->interface_flags |= _ETHLINK_IFFLAG_LINK_STATUS_BIT;

                /** Update the interface speed and flags (#1 and #2) from actual data. */
                _update_cip_ethlink_interface_speed_and_duplex_flags(p_ether_phy_instance, p_ethlink_cip_instance);

                /** Set auto-negotiation status (in #2) to success code. */
                if( ETHER_PHY_AUTO_NEGOTIATION_ON == p_ether_phy_extend_cfg->auto_negotiation )
                {
                    (void) _set_cip_ethlink_autonegotiaion_status(p_ethlink_cip_instance, _NEGOTIATION_STATUS_SUCCESS);

                    /**
                     * TODO: For getting auto-negotiaion fail status, r_ether_phy API should be expanded.
                     */
                }
            }
            else
            {
                /** Disable link status flag (in #2) */
                p_ethlink_cip_instance->interface_flags &= ~(_ETHLINK_IFFLAG_LINK_STATUS_BIT);

                /** Set auto-negotiation status (in #2) to in progress code. */
                if( ETHER_PHY_AUTO_NEGOTIATION_ON == p_ether_phy_extend_cfg->auto_negotiation )
                {
                    (void) _set_cip_ethlink_autonegotiaion_status(p_ethlink_cip_instance, _NEGOTIATION_STATUS_IN_PROGRESS);
                }
            }
        }
        break;

    case ETHER_NETIF_CALLBACK_EVENT_RECEIVE_ETHER_FRAME:
        /** Discards packet. */
        USR_HEAP_RELEASE( p_args->p_frame_packet );
        break;
    }
}

/*******************************************************************************************************************//**
 * @brief Update CIP Ethernet link interface speed (#1) and duplex flags (in #2)
 *
 * @param[in] p_ctrl                    Pointer to the controller
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _update_cip_ethlink_interface_speed_and_duplex_flags( ether_phy_instance_t const * p_ether_phy_instance,
                                                                       CipEthernetLinkObject * p_ethlink_cip_instance )
{
    /** Error code */
    fsp_err_t fsp_err;

    /** Get register address of Ethernet subsystem */
    uint32_t link_speed_duplex;
    uint32_t local_pause_bits;
    uint32_t partner_pause_bits;

    /** Unused parameters */
    (void) local_pause_bits;
    (void) partner_pause_bits;

    /**
     * Get target ability.
     * TODO:
     *  This process should be done by um_ether_netif module and
     *  the abilities should be passed from um_ether_netif module callback.
     */
    fsp_err = p_ether_phy_instance->p_api->linkPartnerAbilityGet( p_ether_phy_instance->p_ctrl,
                                                                  &link_speed_duplex,
                                                                  &local_pause_bits,
                                                                  &partner_pause_bits );
    FSP_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Set to the interface flag and speed*/
    switch( link_speed_duplex )
    {
    case ETHER_PHY_LINK_SPEED_1000F:
        p_ethlink_cip_instance->interface_flags |= _ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT;
        p_ethlink_cip_instance->interface_speed = 1000;
        break;
    case ETHER_PHY_LINK_SPEED_1000H:
        p_ethlink_cip_instance->interface_flags &= ~(_ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT);
        p_ethlink_cip_instance->interface_speed = 1000;
        break;
    case ETHER_PHY_LINK_SPEED_100F:
        p_ethlink_cip_instance->interface_flags |= _ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT;
        p_ethlink_cip_instance->interface_speed = 100;
        break;
    case ETHER_PHY_LINK_SPEED_100H:
        p_ethlink_cip_instance->interface_flags &= ~(_ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT);
        p_ethlink_cip_instance->interface_speed = 100;
        break;
    case ETHER_PHY_LINK_SPEED_10F:
        p_ethlink_cip_instance->interface_flags |= _ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT;
        p_ethlink_cip_instance->interface_speed = 10;
        break;
    case ETHER_PHY_LINK_SPEED_10H:
        p_ethlink_cip_instance->interface_flags &= ~(_ETHLINK_IFFLAG_HALF_FULL_DUPLEX_BIT);
        p_ethlink_cip_instance->interface_speed = 10;
        break;
    default:
        p_ethlink_cip_instance->interface_speed = 0;
        break;
    }

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Update CIP Ethernet link interface negotiation status flags (in #2)
 *
 * @param[in] p_ctrl                    Pointer to the controller
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _set_cip_ethlink_autonegotiaion_status( CipEthernetLinkObject * p_ethlink_cip_instance,  opener_port_negotiation_status_t status )
{
    /** Clear with bit mask */
    p_ethlink_cip_instance->interface_flags &= ~(_ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_MASK);

    /** Set status */
    p_ethlink_cip_instance->interface_flags |= ((uint32_t) status << _ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_SHIFT) & _ETHLINK_IFFLAG_NEGOTIATION_STATUS_BIT_MASK;

    /** Return success code */
    return USR_SUCCESS;
}

