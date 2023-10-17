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
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** FSP module instances. */
#include "hal_data.h"
#include "common_data.h"
#include "main_thread.h"

/** User module instance APIs. */
#include "um_common_cfg.h"
#include "um_common_api.h"
#include "um_serial_io_api.h"
#include "um_serial_io.h"
#include "um_ether_netif_api.h"
#include "um_ether_netif.h"
#include "um_lwip_port_api.h"
#include "um_lwip_port.h"
#include "um_opener_port_api.h"
#include "um_opener_port.h"
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
#include "um_dlr_api.h"
#include "um_dlr.h"
#endif

/** lwIP supports */
#include "lwip/def.h"
#include "lwip/inet.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * Example of CIP user device configuration
 */
#define SAMPLE_CIP_INCARNATION_ID          (0xA5A5)     ///< Incarnation ID for EtherNet/IP CIP connection ID
#define SAMPLE_CIP_DEVICE_SERIAL_NUMBER    (0x12345678) ///< Identity object: Device serial number (#6)

/**
 * Example of CIP Non-volatile attribute configuration.
 * These should be gotten from Non-volatile memory, but use compiled fixed value in this example.
 */
#define SAMPLE_TCPIP_CONFIG_METHOD               (LWIP_PORT_DHCP_DISABLE) ///< Related to TCP/IP interface object attribute : Configuration Control (#3 bit0-3, 0 or 2)
#define SAMPLE_TCPIP_ENCAP_TIMEOUT_VALUE         (120)  ///< TCP/IP interface object attribute : encapsulation timeout value (#13).
#define SAMPLE_QOS_DSCP_VALUE_URGENT             (55U)  ///< QoS Object attribute: DSCP urgent (#4)
#define SAMPLE_QOS_DSCP_VALUE_SCHEDULED          (47U)  ///< QoS Object attribute: DSCP scheduled (#5)
#define SAMPLE_QOS_DSCP_VALUE_HIGH               (43U)  ///< QoS Object attribute: DSCP high (#6)
#define SAMPLE_QOS_DSCP_VALUE_LOW                (31U)  ///< QoS Object attribute: DSCP low (#7)
#define SAMPLE_QOS_DSCP_VALUE_EXPLICIT           (27U)  ///< QoS Object attribute: DSCP explicit (#8)

/**
 * Example of TCP/IP configuration related to CIP TCP/IP interface object.
 */
#define SAMPLE_STATIC_IP_ADDRESS        PP_HTONL(LWIP_MAKEU32(192,168,  1,170)) ///< TCP/IP interface object attribute: interface configuration (#5)
#define SAMPLE_STATIC_SUBNET_MASK       PP_HTONL(LWIP_MAKEU32(255,255,255,  0)) ///< TCP/IP interface object attribute: interface configuration (#5)
#define SAMPLE_STATIC_GATEWAY_ADDRESS   PP_HTONL(LWIP_MAKEU32(192,168,  1,  1)) ///< TCP/IP interface object attribute: interface configuration (#5)
#define SAMPLE_HOSTNAME                 "OPENER_NETIF0"                         ///< TCP/IP interface object attribute: host name (#6)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/**
 * Serial IO module instance
 */
static serial_io_instance_ctrl_t g_serial_io0_ctrl;
static serial_io_cfg_t const g_serial_io0_cfg =
{
        .p_uart_instance = &g_uart0,
};
serial_io_instance_t const g_serial_io0 =
{
        .p_ctrl = &g_serial_io0_ctrl,
        .p_cfg  = &g_serial_io0_cfg,
        .p_api  = &g_serial_io_on_serial_io,
};

/**
 * Ethernet network interface module instance
 */
static ether_netif_instance_ctrl_t g_ether_netif0_ctrl;
static ether_netif_cfg_t const g_ether_netif0_cfg =
{
         .p_ether_instance = &g_ether0,
};
ether_netif_instance_t const g_ether_netif0 =
{
         .p_ctrl = &g_ether_netif0_ctrl,
         .p_cfg  = &g_ether_netif0_cfg,
         .p_api  = &g_ether_netif_on_ether_netif
};

/**
 * lwIP network interface module instance
 */
static lwip_port_instance_ctrl_t g_lwip_port0_ctrl;
static lwip_port_common_ctrl_t g_lwip_port0_common_ctrl;

static uint8_t gp_lwip_port0_hostname[16] = SAMPLE_HOSTNAME;
static lwip_port_netif_cfg_t g_lwip_port0_netif_cfg =
{
        .dhcp            = SAMPLE_TCPIP_CONFIG_METHOD,
        .ip_address      = SAMPLE_STATIC_IP_ADDRESS,
        .subnet_mask     = SAMPLE_STATIC_SUBNET_MASK,
        .gateway_address = SAMPLE_STATIC_GATEWAY_ADDRESS,
        .p_host_name     = gp_lwip_port0_hostname
};

static lwip_port_common_cfg_t const g_lwip_port0_common_cfg =
{
        .p_ether_netif_instance = &g_ether_netif0,
        .p_common_ctrl = &g_lwip_port0_common_ctrl,
};

static lwip_port_cfg_t const g_lwip_port0_cfg =
{
        .p_netif_cfg  = &g_lwip_port0_netif_cfg,
        .p_common_cfg = &g_lwip_port0_common_cfg,
};

lwip_port_instance_t const g_lwip_port0 =
{
        .p_ctrl = &g_lwip_port0_ctrl,
        .p_cfg  = &g_lwip_port0_cfg,
        .p_api  = &g_lwip_port_on_lwip_port
};

/**
 * DLR module instance
 */
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
static dlr_instance_ctrl_t g_dlr0_ctrl;
static dlr_cfg_t g_dlr0_cfg =
{
     .p_ether_netif_instance = &g_ether_netif0,
     .p_ether_switch_instance = &g_ethsw0,
     .p_ip_address = &g_lwip_port0_netif_cfg.ip_address,
};

static dlr_instance_t g_dlr0 =
{
     .p_ctrl = &g_dlr0_ctrl,
     .p_cfg = &g_dlr0_cfg,
     .p_api = &g_dlr_on_dlr
};
#endif

/**
 * OpENer Port module instance
 */
static opener_port_instance_ctrl_t g_opener_port0_ctrl;
static opener_port_cip_tcpip_nvdata_t g_opener_port0_cip_tcpip_nvdata =
{
          .encapsulation_inactivity_timeout_sec = SAMPLE_TCPIP_ENCAP_TIMEOUT_VALUE,
          .config_method = SAMPLE_TCPIP_CONFIG_METHOD,
};
static opener_port_cip_qos_nvdata_t g_opener_port0_cip_qos_nvdata =
{
         .dscp_urgent    = SAMPLE_QOS_DSCP_VALUE_URGENT,
         .dscp_scheduled = SAMPLE_QOS_DSCP_VALUE_SCHEDULED,
         .dscp_high      = SAMPLE_QOS_DSCP_VALUE_HIGH,
         .dscp_low       = SAMPLE_QOS_DSCP_VALUE_LOW,
         .dscp_explict   = SAMPLE_QOS_DSCP_VALUE_EXPLICIT,
};
static opener_port_cip_settable_nvdata_t g_opener_port0_cip_nvdata =
{
         .p_qos_nvdata = &g_opener_port0_cip_qos_nvdata,
         .p_tcpip_nvdata = &g_opener_port0_cip_tcpip_nvdata,
};
static opener_port_cip_user_device_cfg_t g_opener_port0_cip_cfg =
{
         .incarnation_id = SAMPLE_CIP_INCARNATION_ID,
         .device_serial_number = SAMPLE_CIP_DEVICE_SERIAL_NUMBER,
};

#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
static opener_port_extend_cfg_t g_opener_port_extend_cfg =
{
         .p_dlr_instance = &g_dlr0,
};
#endif

static opener_port_cfg_t const g_opener_port0_cfg =
{
        .p_ether_netif_instance = &g_ether_netif0,
		.p_lwip_port_instance = &g_lwip_port0,
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
		.p_extend = &g_opener_port_extend_cfg,
#endif
		.p_cip_user_device_cfg = &g_opener_port0_cip_cfg,
        .p_cip_settable_nvdata = &g_opener_port0_cip_nvdata,
};
static opener_port_instance_t const g_opener_port0 =
{
		.p_ctrl = &g_opener_port0_ctrl,
		.p_cfg  = &g_opener_port0_cfg,
		.p_api  = &g_opener_port_on_opener_port,
};

/**
 * Export pointer of instance.
 */
serial_io_instance_t const * gp_serial_io0 = &g_serial_io0;
ether_netif_instance_t const * gp_ether_netif0 = &g_ether_netif0;
lwip_port_instance_t const * gp_lwip_port0 = &g_lwip_port0;
opener_port_instance_t const * gp_opener_port0 = &g_opener_port0;
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
dlr_instance_t const * gp_dlr0 = &g_dlr0;
#endif

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
void opener_port_user_instance_init(void);
void opener_port_user_instance_init(void)
{
    /** Error status */
    usr_err_t usr_err;

    /******************************************************************************************************************
     * Startup Serial I/O module for debug print
     ******************************************************************************************************************/
    /** Open Serial I/O module. */
    usr_err = gp_serial_io0->p_api->open( gp_serial_io0->p_ctrl, gp_serial_io0->p_cfg );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_BLOCK_CPU();
    }

    /** Start Serial I/O module. */
    usr_err = gp_serial_io0->p_api->start( gp_serial_io0->p_ctrl );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Started Serial I/O interface." );

    /******************************************************************************************************************
     * Startup Ethernet network interface.
     ******************************************************************************************************************/
    /** Open Ethernet network interface. */
    usr_err = gp_ether_netif0->p_api->open(gp_ether_netif0->p_ctrl, gp_ether_netif0->p_cfg);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to initialize Ethernet network interface." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Initialized Ethernet network interface." );

    /** Start Ethernet network interface. */
    usr_err = gp_ether_netif0->p_api->start(gp_ether_netif0->p_ctrl);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to start Ethernet network interface." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Started Ethernet network interface." );

    /******************************************************************************************************************
     * Startup Ethernet network interface.
     ******************************************************************************************************************/
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
    /** Open target module instance. */
    usr_err = gp_dlr0->p_api->open(gp_dlr0->p_ctrl, gp_dlr0->p_cfg);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to initialize EtherNet/IP DLR module." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Initialized EtherNet/IP DLR module." );

    /** Start Ethernet network interface. */
    usr_err = gp_dlr0->p_api->start(gp_dlr0->p_ctrl);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to start EtherNet/IP DLR module." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Started EtherNet/IP module." );
#endif

    /******************************************************************************************************************
     * Startup lwIP TCP/IP network interface.
     ******************************************************************************************************************/
    /** Open lwIP TCP/IP network interface. */
    usr_err = gp_lwip_port0->p_api->open(gp_lwip_port0->p_ctrl, gp_lwip_port0->p_cfg);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to initialize lwIP TCP/IP stack." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Initialized lwIP TCP/IP stack." );

    /** Start lwIP TCP/IP network interface. */
    usr_err = gp_lwip_port0->p_api->start(gp_lwip_port0->p_ctrl);
    if ( USR_SUCCESS != usr_err )
    {
        USR_LOG_ERROR( "Failed to start the lwIP TCP/IP stack." );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Started lwIP TCP/IP stack tasks." );

    /******************************************************************************************************************
     * Open OpENer EtherNet/IP network interface.
     ******************************************************************************************************************/
    usr_err = gp_opener_port0->p_api->open( gp_opener_port0->p_ctrl, gp_opener_port0->p_cfg );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_PRINT( "Failed to initialize OpENer port stack. \n" );
        USR_DEBUG_BLOCK_CPU();
    }
    USR_LOG_INFO( "Initialized OpENer port stack." );
}
