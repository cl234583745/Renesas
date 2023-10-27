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
#include "um_lwip_port_api.h"

#include "ciptcpipinterface.h"
#include "cipethernetlink.h"    ///< for getting object number of Ethernet link object.
#include "cipstring.h"
#include "generic_networkhandler.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** For Status attribute (#1) */
#define _TCPIP_STATUS_IFCFG_BIT_SHIFT                               ((uint32_t) 0)
#define _TCPIP_STATUS_IFCFG_BIT_MASK                                ((uint32_t) 0x0F<<_TCPIP_STATUS_IFCFG_BIT_SHIFT)
#define _TCPIP_STATUS_IFCFG_MCAST_PENDING_BIT                       ((uint32_t) kTcpipStatusMcastPend)     ///< bit 4,
#define _TCPIP_STATUS_IFCFG_CONFIGURATION_PENDING_BIT               ((uint32_t) kTcpipStatusIfaceCfgPend)  ///< bit 5,
#define _TCPIP_STATUS_IFCFG_ACD_STATUS_BIT                          ((uint32_t) kTcpipStatusAcdStatus)     ///< bit 6,
#define _TCPIP_STATUS_IFCFG_ACD_FAULT_BIT                           ((uint32_t) kTcpipStatusAcdFault)      ///< bit 7,
#define _TCPIP_STATUS_IFCFG_IANA_PORT_ADMIN_CHANGE_PENDING_BIT      ((uint32_t) 0x01<<8)
#define _TCPIP_STATUS_IFCFG_IANA_PROTOCOL_ADMIN_CHANGE_PENDING_BIT  ((uint32_t) 0x01<<9)

/** For Configuration Capability (#2) */
#define _TCPIP_CFGCAP_BOOTP_CLIENT_BIT                      ((uint32_t) 0x01<<0)
#define _TCPIP_CFGCAP_DNS_CLIENT_BIT                        ((uint32_t) 0x01<<1)
#define _TCPIP_CFGCAP_DHCP_CLIENT_BIT                       ((uint32_t) 0x01<<2)
#define _TCPIP_CFGCAP_DHCP_DNS_UPDATE_BIT                   ((uint32_t) 0x01<<3)
#define _TCPIP_CFGCAP_CONFIGURATION_SETTABLE_CLIENT_BIT     ((uint32_t) 0x01<<4)
#define _TCPIP_CFGCAP_HARDWARE_CONFIGURABLE_BIT             ((uint32_t) 0x01<<5)
#define _TCPIP_CFGCAP_IFCFG_CHANGE_REQUIRES_RESET           ((uint32_t) 0x01<<6)
#define _TCPIP_CFGCAP_ACD_CAPABLE                           ((uint32_t) 0x01<<7)

/** For TTL Value (#8) */
#define _TCPIP_TTL_VALUE_DEFALUT    (1)

/** For Mcast Config (#9) */
#define _TCPIP_MCASTCFG_ALLOC_CONTROL_DEFAULT_ALGORITHM     (0)

/** For Encapsulation timeout value (#13) */
#define EIP_ENCAP_TIMEOUT_VALUE_DEFALUT  (120)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** For Interface Configuration Status bits of Status attribute (#1) */
typedef enum e_tcpip_status_ifcfg
{
    _TCPIP_STATUS_IFCFG_NOT_CONFIGURED                  = 0x00,  ///< The Interface Configuration (#5) has not been configured.
    _TCPIP_STATUS_IFCFG_CONFIGURED_BOOTP_DHCP_NVSTORAGE = 0x01,  ///< The Interface Configuration (#5) obtained from BOOTP, DHCP, non-volatile storage.
    _TCPIP_STATUS_IFCFG_CONFIGURED_HARDWARE_SETTING     = 0x02   ///< The Interface Configuration (#5) obtained from hardware settings (e.g. psuhwheel etc.)
} _tcpip_status_ifcfg;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Export to cip/cipconnectionmanager.c */
NetworkStatus g_network_status;             ///< This global variable holding the current network status used for open multicast connection

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
static void _lwip_port_callback_func( lwip_port_callback_args_t * p_arg );
static usr_err_t _open_cip_tcpip_instance( opener_port_lwip_port_ctrl_t * const p_ctrl );
static usr_err_t _init_cip_tcpip_instance_attributes( opener_port_lwip_port_ctrl_t * const p_ctrl );
static usr_err_t _set_cip_tcpip_interface_configuration_status( opener_port_lwip_port_ctrl_t * const p_ctrl, _tcpip_status_ifcfg status_ifcfg );
static usr_err_t _set_cip_tcpip_network_configuration( opener_port_lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_open( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                         lwip_port_instance_t const * const p_lwip_port_instance,
                                         opener_port_cip_user_device_cfg_t const * const p_cip_device_cfg,
                                         opener_port_cip_settable_nvdata_t const * const p_cip_nvdata_cfg)
{
	/** Error codes*/
	usr_err_t usr_err;

	/** Set the configuration */
    p_ctrl->p_cip_nvdata = p_cip_nvdata_cfg;
    p_ctrl->p_cip_device_cfg = p_cip_device_cfg;

    /** Set target module instance. */
	p_ctrl->p_lwip_port_instance = p_lwip_port_instance;

	/** Set parent task handle */
	p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

	/** Set callback properties */
    p_ctrl->callback_node.p_func    = _lwip_port_callback_func;
    p_ctrl->callback_node.p_context = p_ctrl;
    p_ctrl->callback_node.p_memory  = &p_ctrl->callback_memory;

    /** Open TCP/IP interface object instance */
    usr_err = _open_cip_tcpip_instance(p_ctrl);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);

    /** Open TCP/IP interface object instance */
    usr_err = _init_cip_tcpip_instance_attributes(p_ctrl);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);

    /** Open target module instance. */
    usr_err = p_ctrl->p_lwip_port_instance->p_api->open(p_ctrl->p_lwip_port_instance->p_ctrl, p_ctrl->p_lwip_port_instance->p_cfg);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err || USR_ERR_ALREADY_OPEN == usr_err , USR_ERR_NOT_INITIALIZED);

    /** Set callback utilities into TCPIP network interface. */
    usr_err = p_ctrl->p_lwip_port_instance->p_api->callbackAdd( p_ctrl->p_lwip_port_instance->p_ctrl, & p_ctrl->callback_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start target module.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_ABORTED              Process has failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_start( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
	/** Error codes. */
	usr_err_t usr_err;

	/** Start target module */
	usr_err = p_ctrl->p_lwip_port_instance->p_api->start(p_ctrl->p_lwip_port_instance->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop target module.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_ABORTED              Process has failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_stop( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
	/** Error codes. */
	usr_err_t usr_err;

	/** Stop target module. */
	usr_err = p_ctrl->p_lwip_port_instance->p_api->stop(p_ctrl->p_lwip_port_instance->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Wait until network status is up
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_ABORTED              Process has failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_wait_netif_up( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
    /** Error codes. */
    usr_err_t usr_err;

    /** lwIP port netif status */
    lwip_port_netif_state_t netif_state = LWIP_PORT_NETIF_STATE_DOWN;

    /** Stop target module. */
    while( LWIP_PORT_NETIF_STATE_UP == netif_state )
    {
        /** Get network status */
        usr_err = p_ctrl->p_lwip_port_instance->p_api->netifStateGet(p_ctrl->p_lwip_port_instance->p_ctrl, &netif_state, false);
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
        vTaskDelay(1);
    }

    /** Get network status with indication to callback. */
    usr_err = p_ctrl->p_lwip_port_instance->p_api->netifStateGet(p_ctrl->p_lwip_port_instance->p_ctrl, &netif_state, true);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief return whether lwIP network interface up or down
 *
 * @retval USR_SUCCESS                  The network interface is up
 * @retval USR_NOT_ENABLE               The network interface is down
 * @retval USR_ERR_ABORTED              Process has failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_netif_is_up( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
    /** Error codes. */
    usr_err_t usr_err;

    /** Get netif state */
    lwip_port_netif_state_t netif_state;

    /** Get network status */
    usr_err = p_ctrl->p_lwip_port_instance->p_api->netifStateGet(p_ctrl->p_lwip_port_instance->p_ctrl, &netif_state, false);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** If netif is down, return non-success code */
    if( LWIP_PORT_NETIF_STATE_DOWN == netif_state )
    {
        return USR_ERR_NOT_ENABLED;
    }

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get network configuration
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_ABORTED              Process has failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_get_netif_config( opener_port_lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * const p_netif_cfg )
{
    /** Error codes. */
    usr_err_t usr_err;

    /** Get network status */
    usr_err = p_ctrl->p_lwip_port_instance->p_api->netifConfigGet(p_ctrl->p_lwip_port_instance->p_ctrl, p_netif_cfg);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get encapsulation inactivity timeout of TCP/IP
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_get_encap_inactivity_timeout( opener_port_lwip_port_ctrl_t * const p_ctrl,  uint32_t * p_timeout_sec )
{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /** Copy the timeout. */
    *p_timeout_sec = p_tcpip_cip_instance->encapsulation_inactivity_timeout;

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get encapsulation inactivity timeout of TCP/IP
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ASSERTION                Assertion error.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_get_mcast_address_and_ttl_value( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                                                    uint32_t * p_mcast_starting_address,
                                                                    uint8_t * p_mcast_ttl_value )

{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /** Check output argument pointer */
    USR_ASSERT( p_mcast_starting_address );
    USR_ASSERT( p_mcast_ttl_value );

    /** Copy the multicast address and ttl value */
    *p_mcast_starting_address = p_tcpip_cip_instance->mcast_config.starting_multicast_address;
    *p_mcast_ttl_value = p_tcpip_cip_instance->mcast_ttl_value;

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get TCP/IP interface settable non-volatile configuration.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ASSERTION                Assertion error.
 **********************************************************************************************************************/
usr_err_t um_opener_port_lwip_port_get_cip_tcpip_nvdata( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                                         opener_port_cip_tcpip_nvdata_t * const p_cip_tcpip_cfg )

{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /** Check output argument pointer */
    USR_ASSERT( p_cip_tcpip_cfg );

    /** Get #13 encapsulation inactivity timeout */
    p_cip_tcpip_cfg->encapsulation_inactivity_timeout_sec = p_tcpip_cip_instance->encapsulation_inactivity_timeout;

    /** Get #3 configuration control config method */
    if( p_tcpip_cip_instance->config_control & kTcpipCfgCtrlDhcp )
    {
        p_cip_tcpip_cfg->config_method = LWIP_PORT_DHCP_ENABLE;
    }
    else
    {
        p_cip_tcpip_cfg->config_method = LWIP_PORT_DHCP_DISABLE;
    }

    /** Return success */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Callback function definition
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 **********************************************************************************************************************/
static void _lwip_port_callback_func( lwip_port_callback_args_t * p_arg )
{
    /** Error status */
    usr_err_t usr_err;

	/** Resolve context. */
	opener_port_lwip_port_ctrl_t * p_ctrl = (opener_port_lwip_port_ctrl_t *) p_arg->p_context;

	/** lwIP network configuration */
	lwip_port_netif_cfg_t lwip_port_netif_cfg;

	switch(p_arg->event)
	{
    case LWIP_PORT_CALLBACK_EVENT_NETIF_UP:

        /** Get lwIP network interface configuration. */
        usr_err = p_ctrl->p_lwip_port_instance->p_api->netifConfigGet(p_ctrl->p_lwip_port_instance->p_ctrl, &lwip_port_netif_cfg );
        if( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }

        /** Update configuration status */
        usr_err = _set_cip_tcpip_interface_configuration_status( p_ctrl, _TCPIP_STATUS_IFCFG_CONFIGURED_BOOTP_DHCP_NVSTORAGE );
        if( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }

        /** Set network interface configuration to CIP stack. */
        usr_err = _set_cip_tcpip_network_configuration( p_ctrl, &lwip_port_netif_cfg );
        if( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }

        break;

    case LWIP_PORT_CALLBACK_EVENT_NETIF_DOWN:

        /** Update configuration status */
        usr_err = _set_cip_tcpip_interface_configuration_status( p_ctrl, _TCPIP_STATUS_IFCFG_NOT_CONFIGURED );
        if( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }

        break;
	}

    /** Notify user task. */
	xTaskNotifyGive( p_ctrl->p_parent_task_handle );
}

/*******************************************************************************************************************//**
 * @brief Open CIP TCP/IP interface object instance
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _open_cip_tcpip_instance( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
    /** Set the CIP TCP/IP interface instance */
    p_ctrl->p_tcpip_cip_instance = &g_tcpip;

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Initialize CIP TCP/IP instance attributes
 *
 * NOTE:
 *  This function initialize all attributes even if the attributes have already initialized on definition or on CipStackInit() function.
 *  This function should be called after the CIP stack initialization to overwrite attributes by the CIP initialization.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _init_cip_tcpip_instance_attributes( opener_port_lwip_port_ctrl_t * const p_ctrl )
{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /** Initialize the status (#1) */
    p_tcpip_cip_instance->status = 0;

    /** Initialize the Configuration Capability (#2) */
    p_tcpip_cip_instance->config_capability = 0;
    p_tcpip_cip_instance->config_capability |= _TCPIP_CFGCAP_DHCP_CLIENT_BIT;

    /** Initialize the configuration control (#3) */
    p_tcpip_cip_instance->config_control = 0;
    switch( p_ctrl->p_lwip_port_instance->p_cfg->p_netif_cfg->dhcp  )
    {
    case LWIP_PORT_DHCP_ENABLE:
        p_tcpip_cip_instance->config_control |= kTcpipCfgCtrlDhcp;
        break;
    case LWIP_PORT_DHCP_DISABLE:
        p_tcpip_cip_instance->config_control |= kTcpipCfgCtrlStaticIp;
    }

    /** Set the Physical Link Object path (#4) */
    p_tcpip_cip_instance->physical_link_object.path_size = 2;
    p_tcpip_cip_instance->physical_link_object.class_id = CIP_ETHERNETLINK_CLASS_CODE;
    p_tcpip_cip_instance->physical_link_object.instance_number = OPENER_ETHLINK_INSTANCE_CNT;
    p_tcpip_cip_instance->physical_link_object.attribute_number = 0;

    /** Initialize Interface Configuration (#5) which is set by callback from lwIP porting module. */
    p_tcpip_cip_instance->interface_configuration.ip_address    = INADDR_ANY;
    p_tcpip_cip_instance->interface_configuration.network_mask  = INADDR_ANY;
    p_tcpip_cip_instance->interface_configuration.gateway       = INADDR_ANY;
    p_tcpip_cip_instance->interface_configuration.name_server   = INADDR_ANY;
    p_tcpip_cip_instance->interface_configuration.name_server_2 = INADDR_ANY;
    SetCipStringByCstr(&p_tcpip_cip_instance->interface_configuration.domain_name, "");

    /** Set the host name (#6) */
    (void) SetCipStringByCstr( &p_tcpip_cip_instance->hostname, (char const * const) p_ctrl->p_lwip_port_instance->p_cfg->p_netif_cfg->p_host_name );

    /** Set the TTL value (#8) to default value. */
    p_tcpip_cip_instance->mcast_ttl_value = _TCPIP_TTL_VALUE_DEFALUT;

    /** Initialize the Mcast Configuration (#9). The starting multicast address is set after getting IP address on callback. */
    p_tcpip_cip_instance->mcast_config.alloc_control = _TCPIP_MCASTCFG_ALLOC_CONTROL_DEFAULT_ALGORITHM;
    p_tcpip_cip_instance->mcast_config.number_of_allocated_multicast_addresses = 1;
    p_tcpip_cip_instance->mcast_config.starting_multicast_address = 0;
    p_tcpip_cip_instance->mcast_config.reserved_shall_be_zero = 0;

    /** Set the SelectACD (#10) which is not settable because ACD is not supported by this stack. */
    p_tcpip_cip_instance->select_acd = false;

    /** Set the encapsulation inactivity timeout (#13) to specified value */
    p_tcpip_cip_instance->encapsulation_inactivity_timeout = p_ctrl->p_cip_nvdata->p_tcpip_nvdata->encapsulation_inactivity_timeout_sec;

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set the status (#1) attribute.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _set_cip_tcpip_interface_configuration_status( opener_port_lwip_port_ctrl_t * const p_ctrl, _tcpip_status_ifcfg status_ifcfg )
{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /** Clear target bits */
    p_tcpip_cip_instance->status &= ~(_TCPIP_STATUS_IFCFG_BIT_MASK);

    /** Set target bits */
    p_tcpip_cip_instance->status |= ((uint32_t) status_ifcfg << _TCPIP_STATUS_IFCFG_BIT_SHIFT) & _TCPIP_STATUS_IFCFG_BIT_MASK;

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set network interface configuration to TCP/IP object.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _set_cip_tcpip_network_configuration( opener_port_lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg )
{
    /** Resolve type */
    CipTcpIpObject * p_tcpip_cip_instance = p_ctrl->p_tcpip_cip_instance;

    /**
     * Set the Interface configuration (#5)
     * TODO: Implement DNS parameter settings
     */
    p_tcpip_cip_instance->interface_configuration.ip_address   = p_netif_cfg->ip_address;
    p_tcpip_cip_instance->interface_configuration.network_mask = p_netif_cfg->subnet_mask;
    p_tcpip_cip_instance->interface_configuration.gateway      = p_netif_cfg->gateway_address;

    /** Calculate & set multi-cast address (#9) */
    (void) CipTcpIpCalculateMulticastIp(p_tcpip_cip_instance);

    /** Set the IP address and network mask for connection manager object. */
    g_network_status.ip_address   = p_netif_cfg->ip_address;
    g_network_status.network_mask = p_netif_cfg->subnet_mask;

    /** Return success */
    return USR_SUCCESS;
}
