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

/** OpENer Encapsulation layer */
#include "encap.h"
#include "endianconv.h"
#include "cipqos.h"
#include "cipconnectionobject.h"

/** Network interface porting header */
#include "socket_timer.h"
#include "opener_error.h"
#include "generic_networkhandler.h"
#include "opener_user_conf.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPENER_PORT_ENCAP_TICK_MS          (1000ULL / configTICK_RATE_HZ)
#define OPENER_PORT_ENCAP_SOCKET_INVALID   (-1)
#define OPENER_PORT_ENCAP_ERROR_MESSAGE_BUFFER_SIZE     (255)

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
static void * gp_callback_context;

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** Export to enet_encap/encap.c. */
SocketTimer g_timestamps[OPENER_NUMBER_OF_SUPPORTED_SESSIONS];  ///< Socket timer for session timeout management.
MilliSeconds g_actual_time;                                     ///< Actual time when polling is called finally.

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
usr_err_t um_opener_port_encap_open( opener_port_encap_ctrl_t * const p_ctrl,
                                     opener_port_tcpip_netif_ctrl_t * const p_tcpip_netif_ctrl )
{
    /** Error status */
    usr_err_t usr_err;

    /** Set the pointer of internal submodule */
    p_ctrl->p_tcpip_netif_ctrl = p_tcpip_netif_ctrl;

    /** Initialize Doubly linked list for Connection objects. */
    (void) DoublyLinkedListInitialize(&connection_list,
                                      CipConnectionObjectListArrayAllocator,
                                      CipConnectionObjectListArrayFree
                                      );

    /** Initialize encapsulation layer here because it accesses the IP address. */
    (void) EncapsulationInit();

    /** Socket session timer */
    SocketTimerArrayInitialize(g_timestamps, OPENER_NUMBER_OF_SUPPORTED_SESSIONS);

    /** Set actual time for encap. */
    usr_err = um_opener_port_encap_set_actual_time(p_ctrl, NULL);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set callback context */
    gp_callback_context = p_ctrl;

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Check if any connections exist.
 *
 * @retval USR_SUCCESS                  Any connections exist.
 * @retval USR_ERR_NOT_FOUND            No connections
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_check_any_connections_exist( opener_port_encap_ctrl_t * const p_ctrl )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Check if any connections exist in connection list. */
    if ( connection_list.first )
    {
       return USR_SUCCESS;
    }

    /** If it is not found, return error code. */
    return USR_ERR_NOT_FOUND;
}

/*******************************************************************************************************************//**
 * @brief Find the socket in connection list.
 *
 * @retval USR_SUCCESS                  Specified socket exist.
 * @retval USR_ERR_NOT_INITIALIZED      No connection related to specified socket.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_find_socket_in_connection_list( opener_port_encap_ctrl_t * const p_ctrl, int32_t target_socket )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** For iterating with connections */
    DoublyLinkedListNode * p_node;
    CipConnectionObject * p_connection_object;

    /** Iterate with list node. */
    for( p_node = connection_list.first ; NULL != p_node ; p_node = p_node->next )
    {
        /** Resolve connection object */
        p_connection_object = ( CipConnectionObject *) p_node->data;

        /** If the target socket is found in connection list */
        if( p_connection_object->socket[kUdpCommuncationDirectionConsuming] == target_socket )
        {
            /** return success code. */
            return USR_SUCCESS;
        }
    }

    /** If it is not found, return error code. */
    return USR_ERR_NOT_FOUND;
}

/*******************************************************************************************************************//**
 * @brief Set actual time.
 *
 * @param[in] p_ctrl            Pointer to the controller
 * @param[out] p_elapsed_time   Pointer to elapsed time calculated by actual time and previous it.
 *
 * @retval USR_SUCCESS          Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_set_actual_time( opener_port_encap_ctrl_t * const p_ctrl, uint32_t * p_elapsed_time_ms )
{
    /** Record previous actual time. */
    p_ctrl->prev_actual_time_ms = g_actual_time;

    /** Set the actual time. */
    g_actual_time = xTaskGetTickCount() * OPENER_PORT_ENCAP_TICK_MS;

    /** Set the elapsed time. */
    if( NULL != p_elapsed_time_ms )
    {
        * p_elapsed_time_ms = (g_actual_time - p_ctrl->prev_actual_time_ms); ///< +1 is rounding.
    }

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Handle the explicit message request received via the UDP unicast/broadcast socket.
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_udp_socket   File descriptor of UDP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_handle_explicit_udp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            int32_t explicit_udp_socket,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address,
                                                            struct sockaddr_in * p_dst_address )
{
    /** Error status */
    usr_err_t usr_err;
    EipStatus status    = kEipStatusOk;

    /** Outgoing message buffer. */
    ENIPMessage * p_outgoing_message;

    /** Size of communication. */
    int32_t sent_size = 0;

    /** Remaining bytes after consumed incoming message. */
    int32_t remaining_bytes = 0;

    /** Check if the message is whether unicast or broadcast. */
    int32_t is_unicast = (p_dst_address->sin_addr.s_addr & (~g_network_status.network_mask)) == (~g_network_status.network_mask) ? false : true;

    /** Allocate outgoing buffer. */
    USR_HEAP_ALLOCATE( p_outgoing_message, sizeof(ENIPMessage) );
    {
        /** Initialize */
        InitializeENIPMessage(p_outgoing_message);

        do
        {
            /** Handle UDP request message and get UDP response message*/
            status = HandleReceivedExplictUdpData(explicit_udp_socket,
                                                  p_src_address,
                                                  (EipUint8*) p_incoming_message_buffer,
                                                  (size_t) recv_size,
                                                  (int *) &remaining_bytes,
                                                  is_unicast,
                                                  p_outgoing_message);
            USR_HEAP_ERROR_RETURN( p_outgoing_message, status == kEipStatusOk || status == kEipStatusOkSend, USR_ERR_ABORTED );

            /** If the response message is need to send, send it. */
            if ( status == kEipStatusOkSend )
            {
                /** Send the message */
                usr_err = um_opener_port_tcpip_netif_send_udp_packet( p_ctrl->p_tcpip_netif_ctrl,
                                                                      explicit_udp_socket,
                                                                      &p_outgoing_message->message_buffer[0],
                                                                      p_outgoing_message->used_message_length,
                                                                      (struct sockaddr *) p_src_address,
                                                                      &sent_size);
                USR_HEAP_ERROR_RETURN( p_outgoing_message, usr_err == USR_SUCCESS, USR_ERR_ABORTED );
            }

            /** Update buffer pointer */
            p_incoming_message_buffer += (recv_size - remaining_bytes);

            /** Update buffer length */
            recv_size = remaining_bytes;

        } while (remaining_bytes > 0);
    }
    /** Release outgoing message buffer. */
    USR_HEAP_RELEASE( p_outgoing_message );

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Handle the explicit message request received via the UDP unicast/broadcast socket.
 *
 * @param[in] p_ctrl                Pointer to the controller.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_handle_implicit_udp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Error status */
    EipStatus status = kEipStatusOk;

    /** Handle the received data* */
    status = HandleReceivedConnectedData( p_incoming_message_buffer, recv_size, p_src_address );
    USR_ERROR_RETURN( status == kEipStatusOk, USR_ERR_ABORTED );

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Handle the explicit message request received via the TCP socket.
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_tcp_socket   File descriptor of TCP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_handle_explicit_tcp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            int32_t explicit_tcp_socket,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address )
{
    /** Error status */
    usr_err_t usr_err;

    /** General variables */
    EipStatus status    = kEipStatusOk;
    int32_t   sent_size = 0;

    /** Outgoing messages */
    ENIPMessage * p_outgoing_message;

    /** For managing message */
    int32_t remained_enip_message = 0;

    /** Save remote address for Forward_Open service. */
    p_ctrl->current_active_peer_address = p_src_address->sin_addr.s_addr;

    /** Allocate outgoing buffer. */
    USR_HEAP_ALLOCATE( p_outgoing_message, sizeof(ENIPMessage) );
    {
        /** Initialize */
        InitializeENIPMessage(p_outgoing_message);

        /** Handle incoming explicit TCP message. */
        status = HandleReceivedExplictTcpData(explicit_tcp_socket,
                                              p_incoming_message_buffer,
                                              (size_t) recv_size,
                                              (int *) &remained_enip_message,
                                              (struct sockaddr *) p_src_address,
                                              p_outgoing_message);

        /** If the status is kEipStatusOkSend, there is an encapsulation response message, so send it. */
        if (kEipStatusOkSend == status)
        {
            usr_err = um_opener_port_tcpip_netif_send_tcp_packet(p_ctrl->p_tcpip_netif_ctrl,
                                                                 explicit_tcp_socket,
                                                                 &p_outgoing_message->message_buffer[0],
                                                                 p_outgoing_message->used_message_length,
                                                                 &sent_size);
            USR_HEAP_ERROR_RETURN( p_outgoing_message, USR_SUCCESS == usr_err && 0 <= sent_size, USR_ERR_ABORTED );
        }
    }
    /** Release outgoing message buffer. */
    USR_HEAP_RELEASE( p_outgoing_message );

    /** Return success. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Update socket timer
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_udp_socket   File descriptor of TCP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_update_socket_timer ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** socket timer */
    SocketTimer * p_socket_timer;

    /** Get socket timer. */
    p_socket_timer = SocketTimerArrayGetSocketTimer(g_timestamps, OPENER_NUMBER_OF_SUPPORTED_SESSIONS, socket);
    USR_ERROR_RETURN( NULL != p_socket_timer, USR_ERR_ABORTED );

    /** Update socket timer if the socket is registered in encapsulation sessions.*/
    (void) SocketTimerSetLastUpdate(p_socket_timer, g_actual_time);

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Update socket timer
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_udp_socket   File descriptor of TCP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_clear_socket_timer ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket )
{
    /** unused */
    (void) p_ctrl;

    /** Socket timer */
    SocketTimer * p_socket_timer;

    /** Get socket timer. */
    p_socket_timer = SocketTimerArrayGetSocketTimer(g_timestamps, OPENER_NUMBER_OF_SUPPORTED_SESSIONS, socket);
    USR_ERROR_RETURN( NULL != p_socket_timer, USR_ERR_ABORTED );

    /** Clear socket timer. */
    (void) SocketTimerClear(p_socket_timer);

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Check socket timer timeout.
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_udp_socket   File descriptor of TCP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_check_socket_timer_timeout ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket, uint32_t timeout_ms )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Elapsed millisecond time from last activity. */
    MilliSeconds elapsed_time_ms = 0;
    SocketTimer * p_socket_timer = NULL;

    /** Get socket timer pointer */
    p_socket_timer = SocketTimerArrayGetSocketTimer( g_timestamps, OPENER_NUMBER_OF_SUPPORTED_SESSIONS, socket );
    USR_ERROR_RETURN( NULL != p_socket_timer, USR_ERR_ABORTED );

    /** Get a difference time (ms) between current time and last update time. */
    elapsed_time_ms = g_actual_time - SocketTimerGetLastUpdate( p_socket_timer );

    /** If the difference time over inactivity timeout, the connections and session is closed. */
    if ( elapsed_time_ms >= timeout_ms )
    {
        /** Indicate timeout */
        return USR_ERR_TIMEOUT;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Remove TCP session
 *
 * @param[in] p_ctrl                Pointer to the controller.
 * @param[in] explicit_tcp_socket   File descriptor of TCP socket for explicit message.
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_remove_tcp_session ( opener_port_encap_ctrl_t * p_ctrl, int32_t explicit_tcp_socket )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Remove Encapsulation Session */
    RemoveSession(explicit_tcp_socket);

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Manage IO connections.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_encap_manage_io_connections( opener_port_encap_ctrl_t * const p_ctrl,  uint32_t elapsed_time_ms )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Call manage_connections() in connection manager every kOpenerTimerTickInMilliSeconds ms */
    ManageConnections( elapsed_time_ms );

    /** Return success */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * OpENer callback API (declared in opener_api.h)
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief create a producing or consuming UDP socket
 * Attention:
 *  For producing point-to-point connection the *pa_pstAddr->sin_addr.s_addr member is set to 0 by OpENer.
 *  The network layer of the application has to set the correct address of the originator.
 * Attention:
 *  For consuming connection the network layer has to set the pa_pstAddr->sin_addr.s_addr to the correct address of the originator.
 * @param communication_direction kUdpCommunicationDirectionProducing or kUdpCommunicationDirectionConsuming
 * @param socket_data pointer to the address holding structure
 * @param qos_for_socket CIP QoS object parameter value
 * @return socket identifier on success -1 on error
 **********************************************************************************************************************/
int CreateUdpSocket (UdpCommuncationDirection communication_direction,
                     struct sockaddr_in     * socket_data,
                     CipUsint                 qos_for_socket)
{
    /** Resolve context */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** Error status */
    usr_err_t usr_err;

    /** New socket */
    int32_t new_socket;

    /** QoS DSCP actual value. */
    uint8_t qos_dscp_value = CipQosGetDscpPriority(qos_for_socket);

    /** Create new socket. */
    switch( communication_direction )
    {
    case  kUdpCommuncationDirectionConsuming:

        /** Create consuming socket. */
        usr_err = um_opener_port_tcpip_netif_create_implicit_udp_socket( p_ctrl->p_tcpip_netif_ctrl,
                                                                         &new_socket,
                                                                         qos_dscp_value,
                                                                         0 );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, OPENER_PORT_ENCAP_SOCKET_INVALID );

        /** Store the originators address (only IP address) */
        socket_data->sin_addr.s_addr = p_ctrl->current_active_peer_address;

        break;

    case kUdpCommuncationDirectionProducing:

        /** Create producing socket */
        usr_err = um_opener_port_tcpip_netif_create_implicit_udp_socket( p_ctrl->p_tcpip_netif_ctrl,
                                                                         &new_socket,
                                                                         qos_dscp_value,
                                                                         socket_data->sin_addr.s_addr );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, OPENER_PORT_ENCAP_SOCKET_INVALID );

        /** If unicast socket, set the peer address. */
        if ( 0 == socket_data->sin_addr.s_addr )
        {
            /** Store the originators address (only IP address) */
            socket_data->sin_addr.s_addr = p_ctrl->current_active_peer_address;
        }

        break;
    }

    return new_socket;
}

/***********************************************************************************************************************
 * @brief Create a producing or consuming UDP socket
 * @param socket_data Pointer to the "send to" address
 * @param socket_handle Socket descriptor to send on
 * @param outgoing message The constructed outgoing message
 * @return kEipStatusOk on success
 **********************************************************************************************************************/
EipStatus SendUdpData (const struct sockaddr_in * const address,
                       const int                  socket_handle,
                       const ENIPMessage * const  outgoing_message)
{
    /** Resolve context */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** Error status */
    usr_err_t usr_err;

    /** Sent size*/
    int32_t sent_size;

    /** Send UDP data */
    usr_err = um_opener_port_tcpip_netif_send_udp_packet( p_ctrl->p_tcpip_netif_ctrl,
                                                          socket_handle,
                                                          (void *) outgoing_message->message_buffer,
                                                          outgoing_message->used_message_length,
                                                          (struct sockaddr *) address,
                                                          &sent_size);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, kEipStatusError );

    if( (uint32_t) sent_size != outgoing_message->used_message_length )
    {
        /** TODO: warning handling */
    }

    /** Return success code */
    return kEipStatusOk;
}

/***********************************************************************************************************************
 * Definition of functions declared by generic_networkhandler.h
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * @brief This function is called for closing UDP socket when class 0/1 connection (I/O connection) sessions are closed.
 *
 * @param socket_handle     socket file descriptor
 **********************************************************************************************************************/
void CloseUdpSocket (int socket_handle)
{
    /** Error status */
    usr_err_t usr_err;

    /** Resolve context */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** Close the socket */
    usr_err = um_opener_port_tcpip_netif_close_socket(p_ctrl->p_tcpip_netif_ctrl, socket_handle);
    if( USR_SUCCESS != usr_err )
    {
        /** Error handling in Void Callback function. */
    }

    return;
}

/**********************************************************************************************************************
 * @brief This function is called for closing TCP server socket when class 3 connection or UCMM session are closed.
 *
 * When this function is called from encap.c, the encapsulation session and class 3 connection are alreday closed,
 * so that this function should close the TCP session and socket timer only.
 *
 * @param socket_handle     TCP session socket handle
 **********************************************************************************************************************/
void CloseTcpSocket (int socket_handle)
{
    /** Status */
    usr_err_t usr_err;

    /** Resolve context */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** Close the socket */
    usr_err = um_opener_port_tcpip_netif_close_socket( p_ctrl->p_tcpip_netif_ctrl, socket_handle );
    if( USR_SUCCESS != usr_err )
    {
        /** Error handling in Void Callback function. */
    }

    /** Remove socket timer when the TCP socket is registered in encapsulation layer. */
    usr_err = um_opener_port_encap_clear_socket_timer( p_ctrl, socket_handle );
    if( USR_SUCCESS != usr_err )
    {
        /** Error handling in Void Callback function. */
    }
}

/***********************************************************************************************************************
 * Definition of functions declared by opener_error.h
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief Gets the error number or equivalent
 *
 * A delegate which implements how to get the error number from the system
 *
 * @return Error number
 ***********************************************************************************************************************/
int GetSocketErrorNumber (void)
{
    /** Resolve the context, but it is not used. */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** But it is not used currently. */
    (void) p_ctrl;

    /** return errno */
    return errno;
}

/***********************************************************************************************************************
 * @brief Returns a human readable message for the given error number
 *
 * Returns a human readable error message to be used in logs and traces.
 * The error message shall not be a shared memory, like the classic strerror function, as such functions are non-reentrant
 * To free the space in which the error message is returned the user shall implement and use the function
 * FreeErrorMessage(char *)
 *
 * @return A human readable error message for the given error number
 ***********************************************************************************************************************/
char * GetErrorMessage (int error_number)
{
    /** Resolve the context */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** But it is not used currently. */
    (void) p_ctrl;

    /** Allocate heap the error message */
    char * error_message = CipCalloc( OPENER_PORT_ENCAP_ERROR_MESSAGE_BUFFER_SIZE, sizeof(char) );

    /** TODO: errno is provided by lwIP, but strerror() function is provided by toolchain, Is this OK? */
    error_message = strerror( error_number );

    /** return the error message */
    return error_message;
}

/***********************************************************************************************************************
 * @brief Frees the space of the error message generated by GetErrorMessage(int)
 *
 * This function shall implement an appropriate method to free the space allocated by GetErrorMessage(int)
 ***********************************************************************************************************************/
void FreeErrorMessage (char * error_message)
{
    /** Resolve the context, but it is not used. */
    opener_port_encap_ctrl_t * const p_ctrl = (opener_port_encap_ctrl_t * const ) gp_callback_context;

    /** But it is not used currently. */
    (void) p_ctrl;

    /** Release the error message */
    CipFree( error_message );
}
