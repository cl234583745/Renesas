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

#ifndef UM_OPENER_PORT_API_H
#define UM_OPENER_PORT_API_H

/*******************************************************************************************************************//**
 * @defgroup OPENER_PORT_API EtherNet/IP OpENer Port
 * @ingroup RENESAS_INTERFACES
 * @brief Interface for OpENer porting module
 *
 * @section OPENER_PORT_SUMMARY Summary
 * The OPENER_PORT interface provides the features for Ethernet/IP adapter device.
 *
 * Implemented by:
 * - @ref OPENER_PORT
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Include APIs */
#include "um_common_api.h"

/** Include dependencies. */
#include "um_lwip_port_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief Module status following the CIP networks library Vol.2 1.30 Table 9-4.1 Module Status Indicator
 */
typedef enum st_opener_port_module_state
{
    OPENER_PORT_MODULE_STATE_NO_POWER,                    ///< No power is supplied to the device.
    OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL,          ///< The device is operating correctly.
    OPENER_PORT_MODULE_STATE_STANDBY,                     ///< The device has not been configured.
    OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT,     ///< The device has detected a major recoverable fault such as incorrect configuration.
    OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT,   ///< The device has detected a major unrecoverable fault.
    OPENER_PORT_MODULE_STATE_SELF_TEST,                   ///< While the device is performing its power up testing
} opener_port_module_state_t;

/**
 * @brief Enumerator for callback event
 */
typedef enum e_opener_port_callback_event
{
    OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED,     ///< Event when stack require to change module status
    OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED,    ///< Event when stack require to change network status
} opener_port_callback_event_t;

/**
 * @brief Enumerator for LED indicator
 */
typedef enum e_opener_port_indicator_flash_state
{
    OPENER_PORT_INDICATOR_STATE_STEADY_OFF,     ///< Requires to change the LED to OFF
    OPENER_PORT_INDICATOR_STATE_STEADY_ON,      ///< Requires to change the LED to ON
    OPENER_PORT_INDICATOR_STATE_FLASHING,       ///< Requires to change the LED to FLASHING
} opener_port_indicator_flash_state_t;

/**
 * @brief Structure for Green/Red LED indicator
 */
typedef struct e_opener_port_indicator_status
{
    opener_port_indicator_flash_state_t green;  ///< State of green LED
    opener_port_indicator_flash_state_t red;    ///< State of red LED
} opener_port_indicator_status_t;

/**
 * @brief Structure for callback arguments
 */
typedef struct st_opener_port_callback_args
{
    opener_port_callback_event_t    event;              ///< Event when the callback is issued.
    opener_port_indicator_status_t  indicator_status;   ///< Indicator status
    void const                      *p_context;         ///< User context set by @ref opener_port_api_t::callbackSet.
} opener_port_callback_args_t;

/**
 * @brief Structure for callback add
 */
typedef struct st_opener_port_callback_link_node
{
    struct st_opener_port_callback_link_node * p_next;  ///< Pointer to next elements
    void (* p_func)(opener_port_callback_args_t *);     ///< Pointer to callback function.
    opener_port_callback_args_t * p_memory;             ///< Pointer to callback arguments.
    void const *                  p_context;            ///< Pointer to user context set to p_callback_memory when callback is issued.
} opener_port_callback_link_node_t;

/** UM_OPENER_PORT control block. Allocate an instance specific control block to pass into the UM_OPENER_PORT API calls.
 * @par Implemented as
 * - eip_adapter_instance_ctrl_t
 */
typedef void opener_port_ctrl_t;

/**
 * Configuration for TCP/IP settable non-volatile data.
 */
typedef struct st_opener_port_cip_tcpip_nvdata
{
    lwip_port_dhcp_t config_method;                   ///< TCP/IP Interface object attribute #3 bit 0-3 */
    uint16_t encapsulation_inactivity_timeout_sec;    ///< TCP/IP Interface object attribute #13. */
} opener_port_cip_tcpip_nvdata_t;

/**
 * Configuration for QoS settable non-volatile data.
 */
typedef struct st_opener_port_cip_nvdata
{
    uint8_t dscp_urgent;            ///< QoS object attribute #4
    uint8_t dscp_scheduled;         ///< QoS object attribute #5
    uint8_t dscp_low;               ///< QoS object attribute #6
    uint8_t dscp_high;              ///< QoS object attribute #7
    uint8_t dscp_explict;           ///< QoS object attribute #8
} opener_port_cip_qos_nvdata_t;

/**
 * Structure for configuration of CIP attribute which is settable non-volatile data which is set via EtherNet/IP data.
 */
typedef struct st_opener_port_cip_settable_nvdata
{
    opener_port_cip_tcpip_nvdata_t * p_tcpip_nvdata;      ///< TCP/IP Interface object attribute #3 and #13. */
    opener_port_cip_qos_nvdata_t * p_qos_nvdata;          ///< QoS object attribute #4 ~ #8. */
} opener_port_cip_settable_nvdata_t;

/**
 * Structure for configuration of CIP attributes for each user device.
 */
typedef struct st_opener_port_cip_user_device_cfg
{
    uint16_t const incarnation_id;                                         ///< Incarnation ID for EtherNet/IP connection ID (Vol2 3.3.7.1 Network Connection ID) */
    uint32_t const device_serial_number;                                   ///< Identity interface object attribute #6 */
} opener_port_cip_user_device_cfg_t;

/**
 * Structure for instance configuration.
 */
typedef struct st_opener_port_cfg
{
    /** Submodules */
	lwip_port_instance_t   const * const p_lwip_port_instance;      ///< Pointer to lwIP port sub module corresponding to TCP/IP interface object.
	ether_netif_instance_t const * const p_ether_netif_instance;    ///< Pointer to Ethernet interface sub module corresponding to Ethernet link object.

	/** Configurations */
	opener_port_cip_user_device_cfg_t  const * const p_cip_user_device_cfg;  ///< Pointer to CIP user device configuration */
    opener_port_cip_settable_nvdata_t const * p_cip_settable_nvdata; ///< Pointer to CIP Non-volatile data configuration. */

    /** Extended configuration */
    void * p_extend;

} opener_port_cfg_t;

/** UM_TCPIP_NETIF API structure. General net functions implemented at the HAL layer follow this API. */
typedef struct st_opener_port_api
{
    /** Open instance.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_Open()
     *
     * @param [in]  p_ctrl          Pointer to control block set in @ref netif_api_t::open call for this net.
     * @param [in]  p_cfg     	    Pointer to configuration structure. All elements of this structures must be set by user.
     * @return
     */
    usr_err_t (* open)( opener_port_ctrl_t * const p_ctrl, opener_port_cfg_t const * const p_cfg );

    /** Start network interface.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_Start()
     * @return
     */
    usr_err_t (* start)( opener_port_ctrl_t * const p_ctrl );

    /** Stop network interface.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_Stop()
     * @return
     */
    usr_err_t (* stop)( opener_port_ctrl_t * const p_ctrl );

    /** Close instance.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_Close()
     * @return
     */
    usr_err_t (* close)( opener_port_ctrl_t * const p_ctrl );

    /** Add callback function and optional context pointer and working memory pointer.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_CallbackAdd()
     *
     * @param[in]  p_ctrl            Pointer to control structure.
     * @param[in]  p_callback_node   Pointer to callback node
     */
    usr_err_t (* callbackAdd)( opener_port_ctrl_t * const p_ctrl, opener_port_callback_link_node_t * p_callback_node );

    /** Set the module state which is applied to indicator and identity object
     * @par Implemented as
     * - @ref UM_OPENER_PORT_ModuleStateSet()
     *
     * @param[in]  p_ctrl            Pointer to control structure.
     * @param[in]  module_state      Module state
     */
    usr_err_t (* moduleStateSet)( opener_port_ctrl_t * const p_ctrl, opener_port_module_state_t module_state );

    /** Get the settable nvdata configuration structure to be set the current settable non-volatile attribute.
     * @par Implemented as
     * - @ref UM_OPENER_PORT_SettableNvdataGet()
     *
     * @param[in]  p_ctrl            Pointer to control structure.
     * @param[out] p_nvdata_cfg      Pointer to settable nvdata configuration structure to be set the current settable non-volatile attribute.
     */
    usr_err_t (* settableNvdataGet)( opener_port_ctrl_t * const p_ctrl, opener_port_cip_settable_nvdata_t * const p_nvdata_cfg );

} opener_port_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_opener_port_instance
{
	opener_port_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    opener_port_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    opener_port_api_t const * p_api;         ///< Pointer to the API structure for this instance
} opener_port_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup OPENER_PORT_API)
 **********************************************************************************************************************/

#endif		/** UM_OPENER_PORT_API_H */
