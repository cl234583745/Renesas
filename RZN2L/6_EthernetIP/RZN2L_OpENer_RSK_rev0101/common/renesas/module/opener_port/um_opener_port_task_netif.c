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

#include "opener_user_conf.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPENER_PORT_ENCAP_HEADER_SIZE   (24)
#define OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_POS  (2)
#define OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_SIZE (2)
#define OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN (OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_POS + OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_SIZE)

/**
 * NOTE:
 *  Polling interval ( time measurement resolution ) may be generate the error of I/O connection test.
 *  Polling interval should be enough bigger than time measurement resolution to reduce the measurement error.
 */
#define OPENER_PORT_NETIF_POLLING_INTERVAL_MS  (kOpenerTimerTickInMilliSeconds)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _task_code (void * pvParameter);
static usr_err_t _update_ethernetip_network_state ( opener_port_netif_ctrl_t * p_ctrl );
static usr_err_t _handle_ethernetip_network_interface( opener_port_netif_ctrl_t * const p_ctrl );
static usr_err_t _handle_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket );
static usr_err_t _handle_explicit_udp_socket( opener_port_netif_ctrl_t * p_ctrl );
static usr_err_t _handle_implicit_udp_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket );
static usr_err_t _handle_explicit_tcp_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket );
static usr_err_t _accept_new_tcp_session( opener_port_netif_ctrl_t * p_ctrl );
static usr_err_t _close_tcp_session( opener_port_netif_ctrl_t * p_ctrl, int32_t socket );

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
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_netif_open( opener_port_netif_ctrl_t * const p_ctrl,
                                          opener_port_lwip_port_ctrl_t * const p_lwip_port_ctrl,
                                          opener_port_tcpip_netif_ctrl_t * const p_tcpip_netif_ctrl,
                                          opener_port_encap_ctrl_t * const p_encap_ctrl,
                                          opener_port_indicator_ctrl_t * const p_indicator_ctrl )
{
    /** Error status */
    usr_err_t usr_err;
    BaseType_t rtos_err;

    /** Set internal module controller. */
    p_ctrl->p_lwip_port_ctrl    = p_lwip_port_ctrl;
    p_ctrl->p_tcpip_netif_ctrl  = p_tcpip_netif_ctrl;
    p_ctrl->p_encap_ctrl        = p_encap_ctrl;
    p_ctrl->p_indicator_ctrl    = p_indicator_ctrl;

    /** Create Explicit TCP socket */
    usr_err = um_opener_port_tcpip_netif_create_explicit_tcp_socket(p_ctrl->p_tcpip_netif_ctrl, &p_ctrl->explicit_tcp_socket);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Create Explicit UDP socket */
    usr_err = um_opener_port_tcpip_netif_create_explicit_udp_socket(p_ctrl->p_tcpip_netif_ctrl, &p_ctrl->explicit_udp_socket);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Get current task handle as parent task. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Create netif task */
    rtos_err = xTaskCreate( _task_code,
                            UM_OPENER_PORT_CFG_NETIF_TASK_NAME,
                            UM_OPENER_PORT_CFG_NETIF_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_OPENER_PORT_CFG_NETIF_TASK_PRIORITY,
                            &(p_ctrl->p_task_handle) );

    USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_NOT_INITIALIZED );

    /** Wait for notification indicating the created task is initialized. */
    (void) ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    /** Suspend the created task. */
    vTaskSuspend( p_ctrl->p_task_handle );

    /** Return error code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the submodule and its controller.
 *
 * @param[in] p_ctrl                    Pointer to the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_netif_close( opener_port_netif_ctrl_t * const p_ctrl )
{
    (void) p_ctrl;

    /** TODO: Implement close sequence. */

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the task.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_netif_start( opener_port_netif_ctrl_t * const p_ctrl )
{
    vTaskResume(p_ctrl->p_task_handle);
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @retval USR_SUCCESS          Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_netif_stop( opener_port_netif_ctrl_t * const p_ctrl )
{
    vTaskSuspend(p_ctrl->p_task_handle);
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief task code
 **********************************************************************************************************************/
static void _task_code (void * pvParameter)
{
    /** Resolve task parameter */
    opener_port_netif_ctrl_t * const p_ctrl = (opener_port_netif_ctrl_t * const) pvParameter;

    /** Status */
    usr_err_t usr_err;

    /** Notify the parent task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    while(true)
    {
        /** Update network state. */
        usr_err = _update_ethernetip_network_state( p_ctrl );
        if( USR_SUCCESS != usr_err )
        {
            /** TODO: Error handling in task. */
        }

        /** Handle EthenNet/IP network interfaces */
        usr_err = _handle_ethernetip_network_interface( p_ctrl );
        if( USR_SUCCESS != usr_err )
        {
            /** TODO: Error handling in task. */
        }
    }

    /** vTaskDelete(NULL); */
}

/**********************************************************************************************************************
 * @brief Update network state
 **********************************************************************************************************************/
static usr_err_t _update_ethernetip_network_state ( opener_port_netif_ctrl_t * p_ctrl )
{
    /** Error status. */
    usr_err_t usr_err;

    /** New network state. */
    opener_port_network_state_t cur_network_state;
    opener_port_network_state_t new_network_state;

    /** Get current network state */
    usr_err = um_opener_port_task_indicator_get_network_state( p_ctrl->p_indicator_ctrl, &cur_network_state );

    /** Check if lwIP port network interface state. */
    usr_err = um_opener_port_lwip_port_netif_is_up( p_ctrl->p_lwip_port_ctrl );
    if ( USR_SUCCESS == usr_err )
    {
        /** Check if any connection exist */
        usr_err = um_opener_port_encap_check_any_connections_exist( p_ctrl->p_encap_ctrl );
        if ( USR_SUCCESS == usr_err )
        {
            /** Any connection exists */
            new_network_state = OPENER_PORT_NETWORK_STATE_CONNECTED;
        }
        else
        {
            /** No connection exists */
            new_network_state = OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS;
        }
    }
    else
    {
        /** No IP address. */
        new_network_state = OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS;
    }

    /** Update network state */
    if( cur_network_state != new_network_state )
    {
        usr_err = um_opener_port_task_indicator_set_network_state( p_ctrl->p_indicator_ctrl, new_network_state );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }

    /** return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Handle Ethernet/IP sockets.
 **********************************************************************************************************************/
static usr_err_t _handle_ethernetip_network_interface( opener_port_netif_ctrl_t * const p_ctrl )
{
    /** Error status */
    usr_err_t usr_err;

    /** Elapsed time and remained time during the cycle of tick timer. */
    static uint32_t elapsed_time_ms = 0;

    /** For socket scanning */
    int32_t socket = 0;
    int32_t highest_socket = 0;

    /** For checking timeout */
    uint32_t encap_inactivity_timeout_sec;

    /** Freeze highest socket handle */
    usr_err = um_opener_port_tcpip_netif_get_highest_socket(p_ctrl->p_tcpip_netif_ctrl, &highest_socket);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Adjust time */
    uint32_t adjust_time_ms = (OPENER_PORT_NETIF_POLLING_INTERVAL_MS > elapsed_time_ms)
                              ? (OPENER_PORT_NETIF_POLLING_INTERVAL_MS - elapsed_time_ms)
                              : 0;

    /** Call select() function */
    usr_err = um_opener_port_tcpip_netif_check_all_socket_events(p_ctrl->p_tcpip_netif_ctrl, adjust_time_ms );
    if ( USR_SUCCESS == usr_err )
    {
        /** Scan the socket */
        for ( socket = 0; socket <= highest_socket; socket++ )
        {
            /** Check if the socket is readable. */
            usr_err = um_opener_port_tcpip_netif_check_socket_readable(p_ctrl->p_tcpip_netif_ctrl, socket);
            if( USR_SUCCESS == usr_err )
            {
                /** Handle socket */
                usr_err = _handle_socket(p_ctrl, socket);
                USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
            }
        }
    }

    /** Update actual time and elapsed time. */
    usr_err = um_opener_port_encap_set_actual_time( p_ctrl->p_encap_ctrl, &elapsed_time_ms );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Manage IO connections. */
    usr_err = um_opener_port_encap_manage_io_connections( p_ctrl->p_encap_ctrl, elapsed_time_ms );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Get timeout value */
    usr_err = um_opener_port_lwip_port_get_encap_inactivity_timeout( p_ctrl->p_lwip_port_ctrl, &encap_inactivity_timeout_sec );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Check if encap inactivity timeout is enabled. */
    if( encap_inactivity_timeout_sec > 0 )
    {
        /** Scan the socket */
        for ( socket = 0; socket <= highest_socket; socket++ )
        {
            /** Check the timeout of socket timer. */
            usr_err = um_opener_port_encap_check_socket_timer_timeout( p_ctrl->p_encap_ctrl, socket, encap_inactivity_timeout_sec * 1000 /** ms **/ );
            if( usr_err == USR_ERR_TIMEOUT )
            {
                /** Close the socket */
                usr_err = _close_tcp_session(p_ctrl, socket);
                USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
            }
        }
    }

    /** Return success */
    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Handle socket events
 **********************************************************************************************************************/
static usr_err_t _handle_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket )
{
    /** Error status */
    usr_err_t usr_err;

    if ( socket == p_ctrl->explicit_tcp_socket )
    {
        /** Accept new TCP session. */
        usr_err = _accept_new_tcp_session( p_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }
    else if ( socket == p_ctrl->explicit_udp_socket )
    {
        /** Handle explicit UDP socket */
        usr_err = _handle_explicit_udp_socket( p_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }
    else if ( USR_SUCCESS == um_opener_port_encap_find_socket_in_connection_list(p_ctrl->p_encap_ctrl, socket) )
    {
        /** Consuming implicit socket */
        usr_err = _handle_implicit_udp_socket( p_ctrl, socket );
    }
    else
    {
        /** Handle explicit TCP socket. */
        usr_err = _handle_explicit_tcp_socket( p_ctrl, socket );
        if ( USR_SUCCESS != usr_err )
        {
            /** Close the socket */
            usr_err = _close_tcp_session(p_ctrl, socket);
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
        }

        /** Update the last active time of socket timer */
        usr_err = um_opener_port_encap_update_socket_timer( p_ctrl->p_encap_ctrl, socket );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }

    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Handle any connection request coming in the TCP server socket.
 **********************************************************************************************************************/
static usr_err_t _accept_new_tcp_session( opener_port_netif_ctrl_t * p_ctrl )
{
    /** Error status */
    usr_err_t usr_err;

    /** session socket and its peer address */
    int32_t session_socket;
    struct sockaddr peer_address;

    /** Accept session. */
    usr_err = um_opener_port_tcpip_netif_accept_tcp_session( p_ctrl->p_tcpip_netif_ctrl,
                                                             p_ctrl->explicit_tcp_socket,
                                                             &session_socket,
                                                             &peer_address );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Close TCP session
 **********************************************************************************************************************/
static usr_err_t _close_tcp_session( opener_port_netif_ctrl_t * p_ctrl, int32_t socket )
{
    /** Error code */
    usr_err_t usr_err;

    /** Remove TCP socket handle from encapsulation. Internally, class 3 connection will be closed. */
    usr_err = um_opener_port_encap_remove_tcp_session( p_ctrl->p_encap_ctrl, socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Remove socket timer when the TCP socket is registered in encapsulation layer. */
    usr_err = um_opener_port_encap_clear_socket_timer( p_ctrl->p_encap_ctrl, socket );

    /** Close the socket */
    usr_err = um_opener_port_tcpip_netif_close_socket( p_ctrl->p_tcpip_netif_ctrl, socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Handle explicit udp socket
 **********************************************************************************************************************/
static usr_err_t _handle_explicit_udp_socket( opener_port_netif_ctrl_t * p_ctrl )
{
    /** Error status */
    usr_err_t usr_err;

    /** Incoming message buffer */
    uint8_t * p_incoming_message_buffer;

    /** Source & destination address in UDP header */
    struct sockaddr_in src_address;   ///< remote address
    struct sockaddr_in dst_address;   ///< own address or broadcast address.

    /** Size of received packet */
    int32_t recv_size = 0;

    /** Initialize incoming message buffer */
    USR_HEAP_ALLOCATE( p_incoming_message_buffer, PC_OPENER_ETHERNET_BUFFER_SIZE );
    {
        /** Receive frame */
        usr_err = um_opener_port_tcpip_netif_recv_udp_packet( p_ctrl->p_tcpip_netif_ctrl,
                                                              p_ctrl->explicit_udp_socket,
                                                              p_incoming_message_buffer,
                                                              PC_OPENER_ETHERNET_BUFFER_SIZE,
                                                              &recv_size,
                                                              (struct sockaddr *) &src_address,
                                                              (struct sockaddr *) &dst_address );
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

        /** Receive & send explicit UDP packet */
        usr_err = um_opener_port_encap_handle_explicit_udp_packet ( p_ctrl->p_encap_ctrl,
                                                                    p_ctrl->explicit_udp_socket,
                                                                    p_incoming_message_buffer,
                                                                    recv_size,
                                                                    &src_address,
                                                                    &dst_address
                                                                    );
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }
    /** Release incoming message buffer. */
    USR_HEAP_RELEASE( p_incoming_message_buffer );

    /** Return success */
    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Handle implicit udp socket
 **********************************************************************************************************************/
static usr_err_t _handle_implicit_udp_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket )
{
    /** Error status */
    usr_err_t usr_err;

    /** Incoming message buffer */
    uint8_t * p_incoming_message_buffer;

    /** Source & destination address in UDP header */
    struct sockaddr_in src_address;   ///< remote address

    /** Size of received packet */
    int32_t recv_size = 0;

    /** Initialize incoming message buffer */
    USR_HEAP_ALLOCATE( p_incoming_message_buffer, PC_OPENER_ETHERNET_BUFFER_SIZE );
    {
        /** Receive frame */
        usr_err = um_opener_port_tcpip_netif_recv_udp_packet( p_ctrl->p_tcpip_netif_ctrl,
                                                              socket,
                                                              p_incoming_message_buffer,
                                                              PC_OPENER_ETHERNET_BUFFER_SIZE,
                                                              &recv_size,
                                                              (struct sockaddr *) &src_address,
                                                              NULL );
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

        /** Receive & send explicit UDP packet */
        usr_err = um_opener_port_encap_handle_implicit_udp_packet ( p_ctrl->p_encap_ctrl,
                                                                    p_incoming_message_buffer,
                                                                    recv_size,
                                                                    &src_address
                                                                    );
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }
    /** Release incoming message buffer. */
    USR_HEAP_RELEASE( p_incoming_message_buffer );

    /** Return success */
    return USR_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Handle explicit tcp socket
 **********************************************************************************************************************/
static usr_err_t _handle_explicit_tcp_socket( opener_port_netif_ctrl_t * p_ctrl, int32_t socket )
{
    /** Error status */
    usr_err_t usr_err;

    /** Incoming message buffer */
    uint8_t * p_incoming_message_buffer;

    /** Size of received packet */
    int32_t recv_size = 0;

    /** Message size when receiving the packet separately */
    int32_t remained_message_size = 0;

    struct sockaddr_in src_address;

    /** Initialize incoming message buffer */
    USR_HEAP_ALLOCATE( p_incoming_message_buffer, PC_OPENER_ETHERNET_BUFFER_SIZE );
    {
        /** Receive incoming message until data length field (byte 3 and 4) in ENIP message. */
        usr_err = um_opener_port_tcpip_netif_recv_tcp_packet(p_ctrl->p_tcpip_netif_ctrl,
                                                             socket,
                                                             p_incoming_message_buffer,
                                                             OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN,
                                                             &recv_size);
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err && 0 <= recv_size, USR_ERR_ABORTED );

        /** Calculate remained data size */
        remained_message_size = p_incoming_message_buffer[OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_POS  ] |
                                p_incoming_message_buffer[OPENER_PORT_ENCAP_HEADER_MESSAGE_LEN_POS+1] << 8;
        remained_message_size += ( OPENER_PORT_ENCAP_HEADER_SIZE - OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN );

        /** If the entire of the incoming message is greater than buffer size, discard the incoming message by receiving the remained data. */
        if (PC_OPENER_ETHERNET_BUFFER_SIZE < (remained_message_size + OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN))
        {
            /** Discard the remained all packet */
            do
            {
                /** Receive the remained message */
                usr_err = um_opener_port_tcpip_netif_recv_tcp_packet(p_ctrl->p_tcpip_netif_ctrl,
                                                                     socket,
                                                                     p_incoming_message_buffer,
                                                                     PC_OPENER_ETHERNET_BUFFER_SIZE,
                                                                     &recv_size);
                USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err && 0 <= recv_size, USR_ERR_ABORTED );

                /** Calculate remained size */
                remained_message_size -= recv_size;
            } while (0 < remained_message_size);

            /** return with success code*/
            return USR_SUCCESS;
        }

        /** Receive remained incoming message properly. */
        usr_err = um_opener_port_tcpip_netif_recv_tcp_packet(p_ctrl->p_tcpip_netif_ctrl,
                                                             socket,
                                                             &p_incoming_message_buffer[OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN],
                                                             (uint32_t) remained_message_size,
                                                             &recv_size);
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err && 0 <= recv_size, USR_ERR_ABORTED );

        /** Calculate entire size of incoming message. */
        recv_size = recv_size + OPENER_PORT_ENCAP_HEADER_SIZE_UNTIL_MESSAGE_LEN;

        /** Get peer remote address and freeze it as a global variable. */
        memset( &src_address, 0, sizeof(struct sockaddr_in));
        usr_err = um_opener_port_tcpip_netif_get_remote_address(p_ctrl->p_tcpip_netif_ctrl,
                                                                socket,
                                                                (struct sockaddr *) &src_address);
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

        /** Receive & send explicit TCP packet*/
        usr_err = um_opener_port_encap_handle_explicit_tcp_packet ( p_ctrl->p_encap_ctrl,
                                                                    socket,
                                                                    p_incoming_message_buffer,
                                                                    recv_size,
                                                                    &src_address );
        USR_HEAP_ERROR_RETURN( p_incoming_message_buffer, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    }
    /** Release incoming message buffer. */
    USR_HEAP_RELEASE( p_incoming_message_buffer );

    /** Return success code. */
    return USR_SUCCESS;
}
