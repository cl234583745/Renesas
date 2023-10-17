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

#ifndef UM_LWIP_PORT_API_H
#define UM_LWIP_PORT_API_H

/*******************************************************************************************************************//**
 * @defgroup TCPIP network interface
 * @ingroup RENESAS_INTERFACES
 * @brief Interface for UM_LWIP_PORT functions.
 *
 * @section UM_LWIP_PORT_API_SUMMARY Summary
 * The UM_LWIP_PORT interface provides the network interface for utilizing TCP/IP communication.
 * - Provide the network interface for TCP/IP communication with TCP/IP protocol stack.
 *
 * Implemented by:
 * - @ref UM_LWIP_PORT
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Include APIs */
#include "um_common_api.h"
#include "um_ether_netif_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_lwip_port_dhcp
{
    LWIP_PORT_DHCP_ENABLE,          ///< Event when DHCP is enabled
    LWIP_PORT_DHCP_DISABLE,         ///< Event when DHCP is disabled
} lwip_port_dhcp_t;

/**
 * Structure for IP configuration.
 */
typedef struct st_lwip_port_netif_cfg
{
    lwip_port_dhcp_t dhcp;           ///< Use DHCP
    uint32_t         ip_address;            ///< IP address
    uint32_t         subnet_mask;           ///< Subnet mask
    uint32_t         gateway_address;       ///< Gateway address
    uint8_t const *  p_host_name;    ///< Pointer to host name
} lwip_port_netif_cfg_t;

/**
 * Enumeration for the state indicating whether netif is up or down
 */
typedef enum e_lwip_port_netif_state
{
    LWIP_PORT_NETIF_STATE_UP,      ///< Event when DHCP is up
    LWIP_PORT_NETIF_STATE_DOWN,    ///< Event when DHCP is down
} lwip_port_netif_state_t;

/**
 * Enumeration for callback event
 */
typedef enum e_lwip_port_callback_event
{
    LWIP_PORT_CALLBACK_EVENT_NETIF_UP,	 ///< Event when network interface links up
    LWIP_PORT_CALLBACK_EVENT_NETIF_DOWN, ///< Event when network interface links down.
} lwip_port_callback_event_t;

/**
 * Structure for callback arguments
 */
typedef struct st_lwip_port_callback_args
{
    lwip_port_callback_event_t event;		///< Event when the callback is issued.
    void const  			   *p_context;	///< User context set by @ref lwip_port_api_t::callbackSet.
} lwip_port_callback_args_t;

/**
 * Structure for callback add
 */
typedef struct st_lwip_port_callback_link_node
{
    struct st_lwip_port_callback_link_node * p_next;  ///< Pointer to next elements
    void (* p_func)(lwip_port_callback_args_t *);     ///< Pointer to callback function.
    lwip_port_callback_args_t * p_memory;             ///< Pointer to callback arguments.
    void const *                  p_context;            ///< Pointer to user context set to p_callback_memory when callback is issued.
} lwip_port_callback_link_node_t;

/** UM_LWIP_PORT control block. Allocate an instance specific control block to pass into the UM_LWIP_PORT API calls.
 * @par Implemented as
 * - lwip_instance_ctrl_t
 */
typedef void lwip_port_ctrl_t;

/**
 * Structure for instance configuration.
 */
typedef struct st_lwip_port_cfg
{
    lwip_port_netif_cfg_t const * p_netif_cfg;  	///< Pointer to static IP configuration
    void const * p_common_cfg;
} lwip_port_cfg_t;

/** UM_LWIP_PORT API structure. General net functions implemented at the HAL layer follow this API. */
typedef struct st_lwip_port_api
{
    /** Open TCP/IP and Ethernet network interface.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Open()
     *
     * @param [in]  p_ctrl              Pointer to control block set in @ref lwip_port_api_t::open call for this net.
     * @param [in]  p_lwip_port_cfg     Pointer to configuration structure. All elements of this structures must be set by user.
     * @return
     */
    usr_err_t (* open)( lwip_port_ctrl_t * const p_ctrl, lwip_port_cfg_t const * const p_cfg );

    /** Start tasks.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Close()
     * @return
     */
    usr_err_t (* start)( lwip_port_ctrl_t * const p_ctrl );

    /** Stop tasks.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Close()
     * @return
     */
    usr_err_t (* stop)( lwip_port_ctrl_t * const p_ctrl );

    /** Specify callback function and optional context pointer and working memory pointer.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_CallbackSet()
     *
     * @param[in]  p_ctrl            Pointer to control structure.
     * @param[in]  p_callback_node   Pointer to callback node
     */
    usr_err_t (* callbackAdd)( lwip_port_ctrl_t * const p_ctrl, lwip_port_callback_link_node_t * p_callback_node );

    /** Get the IP configuration of lwIP network interface.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_NetifConfigGet()
     *
     * @param[in]  p_ctrl         Pointer to control structure.
     * @param[out]  p_netif_cfg   Pointer to netif configuration.
     * @return
     */
    usr_err_t (* netifConfigGet)( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg );

    /** Get the state of lwIP network interface.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_NetifStateGet()
     *
     * @param[in]   p_ctrl          Pointer to control structure.
     * @param[out]  p_netif_state   Pointer to netif state.
     * @param[in]   notify_callback Whether notify the state to callback functions or not
     * @return
     */
    usr_err_t (* netifStateGet)( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_state_t * p_netif_state, uint8_t notify_callback );

} lwip_port_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_lwip_port_instance
{
    lwip_port_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    lwip_port_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    lwip_port_api_t const * p_api;         ///< Pointer to the API structure for this instance
} lwip_port_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup UM_LWIP_PORT_API)
 **********************************************************************************************************************/

#endif		// UM_LWIP_PORT_API_H
