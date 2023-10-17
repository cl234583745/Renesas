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

/** User module instance APIs. */
#include "um_serial_io_api.h"
#include "um_opener_port.h"
#include "um_common_api.h"
#include "um_common_cfg.h"

/** Board IO definitions */
#include "board_io_define.h"

/** OpENer libraries */
#include "opener_api.h"
#include "cipqos.h"
#include "ciptcpipinterface.h"
#include "cipidentity.h"
#include "appcontype.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * Sample application version
 */
#define SAMPLE_APP_VERSION_MAJOR    (1)
#define SAMPLE_APP_VERSION_MINOR    (0)

/**
 * Settings for Module/Network statue Red/Green LED.
 */
#if defined(BOARD_RZT2M_RSK)    ///< Settings For RSK+RZ/T2M board
#define BOARD_PIN_MOD_GREEN     (BOARD_ETH_LED2_GREEN)
#define BOARD_PIN_MOD_RED       (BOARD_ETH_LED6_RED)
#define BOARD_PIN_NET_GREEN     (BOARD_ETH_LED5_GREEN)
#define BOARD_PIN_NET_RED       (BOARD_ETH_LED1_RED)

#elif defined(BOARD_RZN2L_RSK)  ///< Settings For RSK+RZ/N2L board
#define BOARD_PIN_MOD_GREEN     (BOARD_ETH_LED2_GREEN)
#define BOARD_PIN_MOD_RED       (BOARD_ETH_LED6_RED)
#define BOARD_PIN_NET_GREEN     (BOARD_ETH_LED5_GREEN)
#define BOARD_PIN_NET_RED       (BOARD_ETH_LED1_RED)

#elif defined(BOARD_RA6M3_EK)   ///< Settings For EK-RA6M3 board
#define BOARD_PIN_MOD_GREEN    (BOARD_PIN_DISABLED)     ///< If LED lacks, set BOARD_PIN_DISABLED
#define BOARD_PIN_MOD_RED      (BOARD_EKRA6M3_RED)
#define BOARD_PIN_NET_GREEN    (BOARD_EKRA6M3_GREEN)
#define BOARD_PIN_NET_RED      (BOARD_EKRA6M3_BLUE)

#else
#define BOARD_PIN_MOD_GREEN    (BOARD_PIN_DISABLED)
#define BOARD_PIN_MOD_RED      (BOARD_PIN_DISABLED)
#define BOARD_PIN_NET_GREEN    (BOARD_PIN_DISABLED)
#define BOARD_PIN_NET_RED      (BOARD_PIN_DISABLED)
#endif

/**
 * The number of pins of pin arrays which access to LED SW efficiently.
 */
#define DEMO_APP_PIN_ARRAY_LED_PIN_NUM           (4)
#define DEMO_APP_PIN_ARRAY_SW_PIN_NUM            (4)

/**
 * Definition of the number of assembly object instances.
 */
#define DEMO_ASSEMBLY_LED_INPUT                     (100) /// 0x64, Instance Type: Static Input */
#define DEMO_ASSEMBLY_SW_INPUT                      (101) /// 0x65, Instance Type: Static Input */
#define DEMO_ASSEMBLY_LED_OUTPUT                    (150) /// 0x96, Instance Type: Static Output */
#define DEMO_ASSEMBLY_HEARTBEAT_FOR_INPUT_ONLY      (238) /// 0xEE, Instance Type: Static Output */
#define DEMO_ASSEMBLY_HEARTBEAT_FOR_LISTEN_ONLY     (237) /// 0xED, Instance Type: Static Output */
#define DEMO_ASSEMBLY_CONFIG                        (151) /// 0x97, Instance Type: Static Configuration */
#define DEMO_ASSEMBLY_EXPLICT                       (154) /// 0x9A, Instance Type: Static I/O */

/**
 * Definition of Assembly object instance attribute #4 Size of data
 */
#define DEMO_ASSEMBLY_LED_INPUT_BYTES    (1)    /// For instance ID #100
#define DEMO_ASSEMBLY_SW_INPUT_BYTES     (1)    /// For instance ID #101
#define DEMO_ASSEMBLY_LED_OUTPUT_BYTES   (1)    /// For instance ID #150
#define DEMO_ASSEMBLY_CONFIG_BYTES       (4)    /// For instance ID #151
#define DEMO_ASSEMBLY_EXPLICT_BYTES      (4)    /// For instance ID #154

/**
 * Definition of connection object instance IDs
 */
#define DEMO_CONNECTION_LED_EXCLUSIVE_OWNER (0)    /// For input assembly #100 and output assembly #150
#define DEMO_CONNECTION_LED_LISTEN_ONLY     (0)    /// For input assembly #100 and output assembly #237 (heart beat)
#define DEMO_CONNECTION_SW_INPUT_ONLY       (0)    /// For input assembly #101 and output assembly #238 (heart beat)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * Structure of controller for example application utilizing the OpENer porting module.
 */
typedef struct st_user_app_example_ctrl
{
    opener_port_instance_t const *    p_opener_port_instance;               ///< Pointer to controller of OpENer port module
    opener_port_callback_link_node_t  callback_link_node;                   ///< Callback link node.
    opener_port_cip_tcpip_nvdata_t    pseudo_nvstorage_for_tcpip_nvdata;    ///< Pseudo Non-volatile storage for TCP/IP interface non-volatile data.
    opener_port_cip_qos_nvdata_t      pseudo_nvstorage_for_qos_nvdata;      ///< Pseudo Non-volatile storage for QoS non-volatile data.
} user_app_example_ctrl_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/**
 * Callback function which OpENer porting module calls when some event occurs such as requesting user to access Module/Network status module.
 */
static void _user_callback_func( opener_port_callback_args_t * );

/**
 * Callback function which OpENer calls when non-volatile attribute is changed by Set_Attribute_Single service.
 */
static EipStatus _callback_qos_nvdata_set( CipInstance *const p_instance, CipAttributeStruct *const p_attribute, CipByte service );
static EipStatus _callback_tcpip_nvdata_set( CipInstance *const p_instance, CipAttributeStruct *const p_attribute, CipByte service );

/**
 * Utility function to access LED and SW pin on the target board.
 */
static usr_err_t _set_indicator_led( bsp_io_port_pin_t io_port_pin, opener_port_indicator_flash_state_t indicator_state );
static usr_err_t _set_pin_array( uint8_t const * const p_byte_array,
                                 size_t const byte_array_size,
                                 bsp_io_port_pin_t const * const p_pin_array,
                                 bsp_io_level_t const * const p_pin_inactive_levels,
                                 size_t const num_pins);
static usr_err_t _get_pin_array( uint8_t * const byte_array,
                                 size_t const byte_array_size,
                                 bsp_io_port_pin_t const * const p_pin_array,
                                 bsp_io_level_t const * const p_pin_inactive_levels,
                                 size_t const num_pins);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/**
 * Instance of structure of controller for example application.
 */
static user_app_example_ctrl_t g_user_app_example0_ctrl;
static user_app_example_ctrl_t * const gp_user_app_example0_ctrl = &g_user_app_example0_ctrl;

/**
 * Pseudo Non-volatile storages for CIP object which has non-volatile attribute.
 */
static CipQosObject   g_pseudo_nvstorage_for_qos_object;      ///< Pseudo Non-volatile storage for QoS Object.
static CipTcpIpObject g_pseudo_nvstorage_for_tcpip_object;    ///< Pseudo Non-volatile storage for TCP/IP Interface Object

/**
 * Buffers assembled to Assembly object instances.
 */
static EipUint8 g_assembly_data_led_input[DEMO_ASSEMBLY_LED_INPUT_BYTES]     = {0}; /** Input */
static EipUint8 g_assembly_data_sw_input[DEMO_ASSEMBLY_SW_INPUT_BYTES]       = {0}; /** Input */
static EipUint8 g_assembly_data_led_output[DEMO_ASSEMBLY_LED_OUTPUT_BYTES]   = {0}; /** Output */
static EipUint8 g_assembly_data_config[DEMO_ASSEMBLY_CONFIG_BYTES]           = {0}; /** Configuration */
static EipUint8 g_assembly_data_explicit[DEMO_ASSEMBLY_EXPLICT_BYTES]        = {0}; /** Explicit */

/**
 * Arrays for efficiently accessing LED/SW pins.
 */
static const bsp_io_port_pin_t g_led_pin_array[DEMO_APP_PIN_ARRAY_LED_PIN_NUM] =
{
#if defined(BOARD_RZT2M_RSK)
     BOARD_LED0_GREEN,
     BOARD_LED1_YELLOW,
     BOARD_LED2_RED,
     BOARD_LED3_RED,
#elif defined(BOARD_RZN2L_RSK)
     BOARD_LED0_GREEN,
     BOARD_LED1_YELLOW,
     BOARD_LED2_RED,
     BOARD_LED3_RED,
#else
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
#endif
};

static const bsp_io_port_pin_t g_sw_pin_array[DEMO_APP_PIN_ARRAY_SW_PIN_NUM] =
{
#if defined(BOARD_RZT2M_RSK)
     BOARD_SW3_PIN0,
     BOARD_SW3_PIN1,
     BOARD_SW3_PIN2,
     BOARD_SW3_PIN3,
#elif defined(BOARD_RZN2L_RSK)
     BOARD_SW3_PIN0,
     BOARD_SW3_PIN1,
     BOARD_SW3_PIN2,
     BOARD_SW3_PIN3,
#else
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
     BOARD_PIN_DISABLED,
#endif
};

static const bsp_io_level_t g_led_pin_array_incactive_levels[DEMO_APP_PIN_ARRAY_LED_PIN_NUM] =
{
#if defined(BOARD_RZT2M_RSK)
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
#elif defined(BOARD_RZN2L_RSK)
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
#else
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
     BOARD_IOLEVEL_LED_OFF,
#endif
};

static const bsp_io_level_t g_sw_pin_array_incactive_levels[DEMO_APP_PIN_ARRAY_SW_PIN_NUM] =
{
#if defined(BOARD_RZT2M_RSK)
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
#elif defined(BOARD_RZN2L_RSK)
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
#else
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
     BOARD_IOLEVEL_SW_OFF,
#endif
};

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/**
 * Extern declaration of OpENer porting module from opener_port_intance.c
 */
extern opener_port_instance_t const * gp_opener_port0;

/**
 * Define the errno for lwIP BSD socket interface (EWARM only)
 */
#if defined(__ICCARM__)
int errno;
#endif

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Main function to launching OpENer and its porting module.
 **********************************************************************************************************************/
void opener_port_user_main( void );
void opener_port_user_main( void )
{
    /** Error code */
    usr_err_t usr_err;

    /** Load controller of example application. */
    user_app_example_ctrl_t * p_ctrl = gp_user_app_example0_ctrl;

    /** Initialize OpENer port module and related modules. */
    extern void opener_port_user_instance_init( void );
    (void) opener_port_user_instance_init();

    /** Enabled the printf() function via SCI UART. */
    USR_DEBUG_PRINT( "\n" );
    USR_DEBUG_PRINT( "/** Started sample application (v%d.%d) for OpENer Port. **/",
                     SAMPLE_APP_VERSION_MAJOR, SAMPLE_APP_VERSION_MINOR);
    USR_DEBUG_PRINT( "\n" );

    /** Load instance of OpENer porting module. */
    p_ctrl->p_opener_port_instance = gp_opener_port0;

    /** Set callback function to OpENer porting module  */
    p_ctrl->callback_link_node.p_context = p_ctrl;
    p_ctrl->callback_link_node.p_func = _user_callback_func;
    usr_err = p_ctrl->p_opener_port_instance->p_api->callbackAdd( p_ctrl->p_opener_port_instance->p_ctrl,
                                                                  &p_ctrl->callback_link_node );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_PRINT( "Failed to add callback. \n" );
        USR_DEBUG_BLOCK_CPU();
    }

    /** Start OpENer porting module. */
    usr_err = p_ctrl->p_opener_port_instance->p_api->start( p_ctrl->p_opener_port_instance->p_ctrl );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_PRINT( "Failed to start OpENer port. \n" );
        USR_DEBUG_BLOCK_CPU();
    }

    /** Apply Module Status to Operational. */
    usr_err = p_ctrl->p_opener_port_instance->p_api->moduleStateSet( p_ctrl->p_opener_port_instance->p_ctrl,
                                                                     OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL );
    if( USR_SUCCESS != usr_err )
    {
        USR_DEBUG_PRINT( "Failed to set module state \n" );
        USR_DEBUG_BLOCK_CPU();
    }

    /** End of task. */
    while(1)
    {
        vTaskDelay( portMAX_DELAY );
    }
}

/***********************************************************************************************************************
 * Implementation of OpENer callback API (declared in opener_api.h)
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Callback for the application initialization
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
EipStatus ApplicationInitialization (void)
{
    /** Create Assembly objects to be accessed by implicit connection. */
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_LED_INPUT, g_assembly_data_led_input, sizeof(g_assembly_data_led_input));
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_CONFIG, g_assembly_data_config, sizeof(g_assembly_data_config));
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_SW_INPUT, g_assembly_data_sw_input, sizeof(g_assembly_data_sw_input));
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_LED_OUTPUT, g_assembly_data_led_output, sizeof(g_assembly_data_led_output));
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_HEARTBEAT_FOR_INPUT_ONLY, NULL, 0);
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_HEARTBEAT_FOR_LISTEN_ONLY, NULL, 0);

    /** Create Assembly object to be accessed by explicit connection only. */
    (void) CreateAssemblyObject(DEMO_ASSEMBLY_EXPLICT, g_assembly_data_explicit, sizeof(g_assembly_data_explicit));

    /**
     * Exclusive Owner Connection for LED I/O.
     * Configure Connection Points as Exclusive Owner connection (Vol.1 3-6.5.3 Exclusive Owner)
     *  T->O: application data
     *  O->T: application data
     */
    ConfigureExclusiveOwnerConnectionPoint(DEMO_CONNECTION_LED_EXCLUSIVE_OWNER,
                                           DEMO_ASSEMBLY_LED_OUTPUT,
                                           DEMO_ASSEMBLY_LED_INPUT,
                                           DEMO_ASSEMBLY_CONFIG);

    /**
     * Input Only Connection for SW input
     * Configure Connection Points as Input Only Connection. (Vol.1 3-6.6 Input Only)
     *  T->O: application data
     *  O->T: heart beat
     */
    ConfigureInputOnlyConnectionPoint(DEMO_CONNECTION_SW_INPUT_ONLY,
                                      DEMO_ASSEMBLY_HEARTBEAT_FOR_INPUT_ONLY,
                                      DEMO_ASSEMBLY_SW_INPUT,
                                      DEMO_ASSEMBLY_CONFIG);

    /**
     * Listen Only Connection for LED input of Exclusive Owner Connection.
     * Configure Connection Points as Input Listen Only Connection. (Vol.1 3-6.5 Listen Only)
     *  T->O: application data
     *  O->T: heart beat
     */
    ConfigureListenOnlyConnectionPoint(DEMO_CONNECTION_LED_LISTEN_ONLY,
                                       DEMO_ASSEMBLY_HEARTBEAT_FOR_LISTEN_ONLY,
                                       DEMO_ASSEMBLY_LED_INPUT,
                                       DEMO_ASSEMBLY_CONFIG);

    /** Get pointer to TCP/IP and QoS CIP class  */
    CipClass * p_tcpip_class = GetCipClass(kCipTcpIpInterfaceClassCode);
    CipClass * p_qos_class = GetCipClass(kCipQoSClassCode);

    /** Set callback function of TCP/IP and QoS class to indicate that settable non-volatile data has been accessed by EtherNet/IP controller. */
    InsertGetSetCallback(p_tcpip_class, _callback_tcpip_nvdata_set, kPostSetFunc|kNvDataFunc);
    InsertGetSetCallback(p_qos_class, _callback_qos_nvdata_set, kPostSetFunc|kNvDataFunc);

    return kEipStatusOk;
}

/*******************************************************************************************************************//**
 * @brief Allow the device specific application to perform its execution
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
void HandleApplication (void)
{
    /**
     * This function is called cyclically every kOpenerTimerTickInMilliSeconds.
     */
}

/*******************************************************************************************************************//**
 * @brief Inform the application on changes occurred for a connection
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
void CheckIoConnectionEvent (unsigned int      output_assembly_id,
                             unsigned int      input_assembly_id,
                             IoConnectionEvent io_connection_event)
{
    /** Unused parameter */
    (void) output_assembly_id;
    (void) input_assembly_id;

    /** Switch depending on event of I/O connection. */
    switch (io_connection_event)
    {
    case kIoConnectionEventOpened:
        break;
    case kIoConnectionEventTimedOut:
        break;
    case kIoConnectionEventClosed:
        break;
    }
}

/*******************************************************************************************************************//**
 * @brief Call back function to inform application on received data for an assembly object.
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
EipStatus AfterAssemblyDataReceived (CipInstance * instance)
{
    /**
     * Check the instance number of Assembly object isntance.
     */
    switch (instance->instance_number)
    {
    case DEMO_ASSEMBLY_LED_OUTPUT:
        /** Write received assembly data into LED. */
        _set_pin_array(g_assembly_data_led_output,
                       DEMO_ASSEMBLY_LED_OUTPUT_BYTES,
                       g_led_pin_array,
                       g_led_pin_array_incactive_levels,
                       DEMO_APP_PIN_ARRAY_LED_PIN_NUM);
        break;
    default:
        break;
    }

    /** Return success code. */
    return kEipStatusOk;
}

/*******************************************************************************************************************//**
 * @brief Inform the application that the data of an assembly object will be sent.
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
EipBool8 BeforeAssemblyDataSend (CipInstance * pa_pstInstance)
{
    EipBool8 has_changed = false;

    /** Check instance number */
    switch( pa_pstInstance->instance_number )
    {
    case DEMO_ASSEMBLY_LED_INPUT:

        /** Read LED array data to assembly data */
        _get_pin_array(g_assembly_data_led_input,
                       DEMO_ASSEMBLY_LED_INPUT_BYTES,
                       g_led_pin_array,
                       g_led_pin_array_incactive_levels,
                       DEMO_APP_PIN_ARRAY_LED_PIN_NUM);
        has_changed = true;
        break;

    case DEMO_ASSEMBLY_SW_INPUT:

        /** Ready SW array data to assembly data */
        _get_pin_array(g_assembly_data_sw_input,
                       DEMO_ASSEMBLY_SW_INPUT_BYTES,
                       g_sw_pin_array,
                       g_sw_pin_array_incactive_levels,
                       DEMO_APP_PIN_ARRAY_SW_PIN_NUM);
        has_changed = true;
        break;

    default:
        break;
    }

    return has_changed;
}

/*******************************************************************************************************************//**
 * @brief Emulate as close a possible a power cycle of the device
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
EipStatus ResetDevice (void)
{
    /** Close All connections */
    CloseAllConnections();

    /** Enable Object parameters which should be enabled when Reset Service is required. */
    CipQosUpdateUsedSetQosValues(); ///< QoS DSCP values (#2 ~ #8) of QoS object instance.

    /** Reset Applications */
    memset(g_assembly_data_led_input,  0, DEMO_ASSEMBLY_LED_INPUT_BYTES);
    memset(g_assembly_data_sw_input,   0, DEMO_ASSEMBLY_SW_INPUT_BYTES);
    memset(g_assembly_data_led_output, 0, DEMO_ASSEMBLY_LED_OUTPUT_BYTES);
    memset(g_assembly_data_config,     0, DEMO_ASSEMBLY_CONFIG_BYTES);
    memset(g_assembly_data_explicit,   0, DEMO_ASSEMBLY_EXPLICT_BYTES);

    /** Return success code */
    return kEipStatusOk;
}

/*******************************************************************************************************************//**
 * @brief Reset the device to the initial configuration and emulate as close as possible a power cycle of the device
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
EipStatus ResetDeviceToInitialConfiguration (void)
{
    /** Reset QoS DSCP value (#2 ~ #8) of QoS object instance to default value. */
    CipQosResetAttributesToDefaultValues();

    /** Reset encapsulation inactivity timeout (#13) of TCP/IP interface object instance to default value. */
    g_tcpip.encapsulation_inactivity_timeout = 120;

    /** Perform Type 0: Reset */
    ResetDevice();

    /** Return success code */
    return kEipStatusOk;
}

/*******************************************************************************************************************//**
 * @brief Inform the application that the Run/Idle State has been changed
 *
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
void RunIdleChanged (EipUint32 run_idle_value)
{
    /** Check run/idle state */
    if ((0x0001 & run_idle_value) == 1)
    {
        /** Set device status to run mode */
        CipIdentitySetExtendedDeviceStatus(kAtLeastOneIoConnectionInRunMode);
    }
    else
    {
        /** Set device status to idel mode */
        CipIdentitySetExtendedDeviceStatus(kAtLeastOneIoConnectionEstablishedAllInIdleMode);
    }
}

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Callback function invoked when the CIP TCP/IP interface object non-volatile attribute is accessed by Set_Attribute_Single service.
 **********************************************************************************************************************/
static EipStatus _callback_tcpip_nvdata_set( CipInstance *const p_instance, CipAttributeStruct *const p_attribute, CipByte service )
{
    /** Unused parameter */
    (void) p_instance;
    (void) p_attribute;
    (void) service;

    /** Resolve example application context. */
    user_app_example_ctrl_t * p_ctrl = gp_user_app_example0_ctrl;

    /** For example, save the object instance directly. */
    g_pseudo_nvstorage_for_tcpip_object = g_tcpip;

    /** For example, save the settable nvdata from OpENer port module. */
    opener_port_cip_settable_nvdata_t settable_nvdata;
    settable_nvdata.p_qos_nvdata   = &p_ctrl->pseudo_nvstorage_for_qos_nvdata;
    settable_nvdata.p_tcpip_nvdata = &p_ctrl->pseudo_nvstorage_for_tcpip_nvdata;
    (void) p_ctrl->p_opener_port_instance->p_api->settableNvdataGet( p_ctrl->p_opener_port_instance->p_ctrl, &settable_nvdata );

    /** Return success code */
    return kEipStatusOk;
}

/*******************************************************************************************************************//**
 * @brief Callback function invoked when the CIP QoS object non-volatile attribute is accessed by Set_Attribute_Single service.
 **********************************************************************************************************************/
static EipStatus _callback_qos_nvdata_set( CipInstance *const p_instance, CipAttributeStruct *const p_attribute, CipByte service )
{
    /** Unused parameter */
    (void) p_instance;
    (void) p_attribute;
    (void) service;

    /** Resolve example application context. */
    user_app_example_ctrl_t * p_ctrl = gp_user_app_example0_ctrl;

    /** For example, save the object instance directly. */
    g_pseudo_nvstorage_for_qos_object = g_qos;

    /** For example, save the settable nvdata from OpENer port module. */
    opener_port_cip_settable_nvdata_t settable_nvdata;
    settable_nvdata.p_qos_nvdata   = &p_ctrl->pseudo_nvstorage_for_qos_nvdata;
    settable_nvdata.p_tcpip_nvdata = &p_ctrl->pseudo_nvstorage_for_tcpip_nvdata;
    (void) p_ctrl->p_opener_port_instance->p_api->settableNvdataGet( p_ctrl->p_opener_port_instance->p_ctrl, &settable_nvdata );

    /** Return success code */
    return kEipStatusOk;
}

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Example callback function to handle the notification from OpENer port module.
 * Please don't block in callback function context.
 **********************************************************************************************************************/
static void _user_callback_func( opener_port_callback_args_t * p_callback_args )
{
    /** Check the callback event. */
    switch( p_callback_args->event )
    {
        /** Require user application to change module indication LED on user board. */
        case OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED:
            R_BSP_PinAccessEnable();
            _set_indicator_led( BOARD_PIN_MOD_GREEN, p_callback_args->indicator_status.green );
            _set_indicator_led( BOARD_PIN_MOD_RED,   p_callback_args->indicator_status.red );
            R_BSP_PinAccessDisable();
            break;

        /** Require user application to change network indication LED on user board. */
        case OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED:
            R_BSP_PinAccessEnable();
            _set_indicator_led( BOARD_PIN_NET_GREEN, p_callback_args->indicator_status.green );
            _set_indicator_led( BOARD_PIN_NET_RED,   p_callback_args->indicator_status.red);
            R_BSP_PinAccessDisable();
            break;
    }
}

/*******************************************************************************************************************//**
 * @brief Set the indicator LED pin
 **********************************************************************************************************************/
static usr_err_t _set_indicator_led( bsp_io_port_pin_t io_port_pin,
                                     opener_port_indicator_flash_state_t indicator_state )
{
    /** For checking current I/O level of indicator */
    bsp_io_level_t current_io_level;

    /** If the pin is disabled, */
    if( BOARD_PIN_DISABLED == (int8_t) io_port_pin )
    {
        /** Return error code. */
        return USR_ERR_ABORTED;
    }

    /** Read current I/O level */
    current_io_level = (bsp_io_level_t) R_BSP_PinRead(io_port_pin);

    switch(indicator_state)
    {
    /** Set ON to LED pin. */
    case OPENER_PORT_INDICATOR_STATE_STEADY_ON:
        if( current_io_level != BOARD_IOLEVEL_LED_ON )
        {
            R_BSP_PinWrite( io_port_pin, BOARD_IOLEVEL_LED_ON);
        }
        break;

    /** Set OFF to LED pin. */
    case OPENER_PORT_INDICATOR_STATE_STEADY_OFF:
        if( current_io_level != BOARD_IOLEVEL_LED_OFF )
        {
            R_BSP_PinWrite( io_port_pin, BOARD_IOLEVEL_LED_OFF);
        }
        break;

    /** Toggle the LED pin. */
    case OPENER_PORT_INDICATOR_STATE_FLASHING:
        R_BSP_PinWrite( io_port_pin, (bsp_io_level_t)!current_io_level );
        break;

    /** Unused State */
    default:
        break;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief set array pins to the specified value
 **********************************************************************************************************************/
static usr_err_t _set_pin_array( uint8_t const * const p_byte_array,
                                 size_t const byte_array_size,
                                 bsp_io_port_pin_t const * const p_pin_array,
                                 bsp_io_level_t const * const p_pin_inactive_levels,
                                 size_t const num_pins)
{
    /** For scanning pins and bytes. */
    size_t pin_idx = 0;
    size_t byte_idx = 0;

    /** For bit data to be written to pin. */
    bsp_io_level_t bit_data = BSP_IO_LEVEL_LOW;

    R_BSP_PinAccessEnable();
    for ( pin_idx = 0; pin_idx < num_pins; pin_idx++ )
    {
        /** If the pin is disabled, */
        if( BOARD_PIN_DISABLED == (int8_t) p_pin_array[pin_idx] )
        {
            /** Skip and continue */
            continue;
        }

        /** If byte index is over, */
        byte_idx = pin_idx / 8;
        if ( byte_idx >= byte_array_size )
        {
            /** Return with error */
            return USR_ERR_ABORTED;
        }

        /** Write data */
        bit_data = (bsp_io_level_t) ((p_byte_array[byte_idx] >> (pin_idx % 8)) & BSP_IO_LEVEL_HIGH);
        R_BSP_PinWrite(p_pin_array[pin_idx], (bsp_io_level_t) ((bit_data ^ p_pin_inactive_levels[pin_idx]) & BSP_IO_LEVEL_HIGH));
    }
    R_BSP_PinAccessDisable();

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the values of array pins.
 **********************************************************************************************************************/
static usr_err_t _get_pin_array( uint8_t * const byte_array,
                                 size_t const byte_array_size,
                                 bsp_io_port_pin_t const * const p_pin_array,
                                 bsp_io_level_t const * const p_pin_inactive_levels,
                                 size_t const num_pins)
{
    /** For scanning pins and bytes. */
    size_t pin_idx = 0;
    size_t byte_idx = 0;

    /** For bit data to be written to pin. */
    bsp_io_level_t bit_data = BSP_IO_LEVEL_LOW;

    R_BSP_PinAccessEnable();
    for (pin_idx = 0; pin_idx < num_pins; pin_idx++ )
    {
        /** If the pin is disabled, */
        if( BOARD_PIN_DISABLED == (int8_t) p_pin_array[pin_idx] )
        {
            /** Skip and continue */
            continue;
        }

        /** If byte index is over, */
        byte_idx = pin_idx / 8;
        if ( byte_idx >= byte_array_size )
        {
            /** Return with error */
            return USR_ERR_ABORTED;
        }

        /** Read data */
        bit_data = (bsp_io_level_t)( (R_BSP_PinRead(p_pin_array[pin_idx]) ^ p_pin_inactive_levels[pin_idx]) & BSP_IO_LEVEL_HIGH );

        /** Apply to byte array */
        if (bit_data)
        {
            byte_array[byte_idx] = (byte_array[byte_idx] | (uint8_t)(bit_data << (pin_idx % 8)));
        }
        else
        {
            byte_array[byte_idx] = (byte_array[byte_idx] & (uint8_t)~(!bit_data << (pin_idx % 8)));
        }
    }
    R_BSP_PinAccessDisable();

    /** Return success code */
    return USR_SUCCESS;
}
