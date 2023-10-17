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

/** lwIP socket */
#include "lwip/sockets.h"

/** OpENer libraries */
#include "cipqos.h"                 ///< For management of QoS value
#include "cipconnectionobject.h"    ///< For getting current QoS DSCP value (s_active_dscp)

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPENER_PORT_SOCKET_INVALID                (-1)
#define OPENER_PORT_SOCKET_ERROR                  (-1)
#define OPENER_PORT_SOCKET_NUM_LISTNER_SOCKET     (32)
#define OPENER_PORT_SOCKET_MESSAGE_CONTROL_BUFFER (512)

#define OPENER_PORT_EIP_EXPLICIT_PORT   (0xAF12)
#define OPENER_PORT_EIP_IMPLICIT_PORT   (0x08AE)

#define QOS_DSCP_DEFAULT_EVENT          (59U)
#define QOS_DSCP_DEFAULT_GENERAL        (47U)
#define QOS_DSCP_DEFAULT_URGENT         (55U)
#define QOS_DSCP_DEFAULT_SCHEDULED      (47U)
#define QOS_DSCP_DEFAULT_HIGH           (43U)
#define QOS_DSCP_DEFAULT_LOW            (31U)
#define QOS_DSCP_DEFAULT_EXPLICIT       (27U)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/** For CIP QoS object initialization */
static usr_err_t _open_cip_qos_instance( opener_port_tcpip_netif_ctrl_t * const p_ctrl );
static usr_err_t _init_cip_qos_instance_attributes( opener_port_tcpip_netif_ctrl_t * const p_ctrl );
static usr_err_t _set_cip_qos_dscp_values( opener_port_tcpip_netif_ctrl_t * const p_ctrl, opener_port_cip_qos_nvdata_t const * const p_cfg );

/** For manage socket set */
static usr_err_t _add_into_socket_set( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t const socket );
static usr_err_t _remove_from_socket_set( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t const socket );

/** Socket utilities */
static usr_err_t _enable_non_blocking( int32_t socket );
static usr_err_t _enable_reuse_address( int32_t socket );
static usr_err_t _set_qos_value( int32_t socket, uint8_t qos_value );

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
usr_err_t um_opener_port_tcpip_netif_open( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                           opener_port_lwip_port_ctrl_t * const p_lwip_port_ctrl,
                                           opener_port_cip_settable_nvdata_t const * const p_cip_nvdata )
{
    /** Error code */
    usr_err_t usr_err;

    /** Set the pointer to cip configuration. */
    p_ctrl->p_cip_nvdata = p_cip_nvdata;

    /** Set the pointer to internal submodule. */
    p_ctrl->p_lwip_port_ctrl = p_lwip_port_ctrl;

    /** Clear socket set */
    FD_ZERO( &p_ctrl->socket_set );
    FD_ZERO( &p_ctrl->readable_socket_set );

    /** Clear highest socket. */
    p_ctrl->highest_socket = OPENER_PORT_SOCKET_INVALID;

    /** Open CIP QoS instance. */
    usr_err = _open_cip_qos_instance( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Initialize CIP QoS instance attributes. */
    usr_err = _init_cip_qos_instance_attributes( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** return success. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Create TCP socket for explicit messaging
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED  Fail to create socket.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_create_explicit_tcp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * const p_socket )
{
    /** Error status */
    usr_err_t usr_err;
    int32_t sock_err;

    /** Network interface configuration. */
    lwip_port_netif_cfg_t netif_cfg;

    /** Get network interface configuration */
    usr_err = um_opener_port_lwip_port_get_netif_config(p_ctrl->p_lwip_port_ctrl, &netif_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Socket address */
    struct sockaddr_in socket_address;
    socket_address.sin_addr.s_addr = netif_cfg.ip_address;
    socket_address.sin_port = PP_HTONS(OPENER_PORT_EIP_EXPLICIT_PORT);
    socket_address.sin_family = AF_INET;

    /** Create TCP socket */
    *p_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_INVALID != *p_socket, USR_ERR_NOT_INITIALIZED );

    /** Set the socket to non blocking mode */
    usr_err = _enable_non_blocking( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set reuse option */
    usr_err = _enable_reuse_address( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Bind the address and port. */
    sock_err = lwip_bind( *p_socket, (struct sockaddr*) &socket_address, sizeof(struct sockaddr) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Start listening. */
    sock_err = lwip_listen( *p_socket, OPENER_PORT_SOCKET_NUM_LISTNER_SOCKET );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Set Qos option */
    usr_err = _set_qos_value( *p_socket, CipQosGetDscpPriority(kConnectionObjectPriorityExplicit) );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Add socket set */
    usr_err = _add_into_socket_set( p_ctrl, *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Create UDP socket for explicit messaging
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED  Fail to create socket.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_create_explicit_udp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * const p_socket )
{
    /** Error status */
    usr_err_t usr_err;
    int32_t sock_err;

    /** Network interface configuration. */
    lwip_port_netif_cfg_t netif_cfg;

    /** Get network interface configuration */
    usr_err = um_opener_port_lwip_port_get_netif_config(p_ctrl->p_lwip_port_ctrl, &netif_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Socket address */
    struct sockaddr_in socket_address;
    socket_address.sin_addr.s_addr = netif_cfg.ip_address;
    socket_address.sin_port = PP_HTONS(OPENER_PORT_EIP_EXPLICIT_PORT);
    socket_address.sin_family = AF_INET;

    /** option value for enabling to get the packet information */
    uint32_t one = 1;

    /** Create UDP socket */
    *p_socket = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_INVALID != *p_socket, USR_ERR_NOT_INITIALIZED );

    /** Set the socket to non blocking mode */
    usr_err = _enable_non_blocking( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set reuse option */
    usr_err = _enable_reuse_address( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Enable to get packet information */
    sock_err = lwip_setsockopt( *p_socket, IPPROTO_IP, IP_PKTINFO, &one, sizeof(one) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Bind the address and port. */
    sock_err = lwip_bind( *p_socket, (struct sockaddr*) &socket_address, sizeof(struct sockaddr) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Set Qos option */
    usr_err = _set_qos_value( *p_socket, CipQosGetDscpPriority(kConnectionObjectPriorityExplicit) );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Add socket set */
    usr_err = _add_into_socket_set( p_ctrl, *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Create UDP socket for implicit messaging
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED  Fail to create socket.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_create_implicit_udp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                                 int32_t * const p_socket,
                                                                 uint8_t const qos_dscp_value,
                                                                 uint32_t multicast_address )
{
    /** Error status */
    usr_err_t usr_err;
    int32_t sock_err;

    /** Network interface configuration. */
    lwip_port_netif_cfg_t netif_cfg;

    /** For multicast information */
    uint32_t mcast_starting_address;
    uint8_t mcast_ttl_value;

    /** Get network interface configuration */
    usr_err = um_opener_port_lwip_port_get_netif_config(p_ctrl->p_lwip_port_ctrl, &netif_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Socket address */
    struct sockaddr_in socket_address;
    socket_address.sin_addr.s_addr = netif_cfg.ip_address;
    socket_address.sin_port = PP_HTONS(OPENER_PORT_EIP_IMPLICIT_PORT);
    socket_address.sin_family = AF_INET;

    /** option value for enabling to get the packet information */
    uint32_t one = 1;

    /** Create UDP socket */
    *p_socket = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_INVALID != *p_socket, USR_ERR_NOT_INITIALIZED );

    /** Set the socket to non blocking mode */
    usr_err = _enable_non_blocking( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set reuse option */
    usr_err = _enable_reuse_address( *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Enable to get packet information */
    sock_err = lwip_setsockopt( *p_socket, IPPROTO_IP, IP_PKTINFO, &one, sizeof(one) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Bind the address and port. */
    sock_err = lwip_bind( *p_socket, (struct sockaddr*) &socket_address, sizeof(struct sockaddr) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );

    /** Set QoS option */
    usr_err = _set_qos_value( *p_socket, qos_dscp_value );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Get ttl value from tcpip interface object. */
    usr_err = um_opener_port_lwip_port_get_mcast_address_and_ttl_value( p_ctrl->p_lwip_port_ctrl, &mcast_starting_address, &mcast_ttl_value );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** If the socket data has multicast address, set the ttl value. */
    if( multicast_address == mcast_starting_address )
    {
        /** Set ttl value of socket */
        sock_err = lwip_setsockopt( *p_socket, IPPROTO_IP, IP_MULTICAST_TTL, &mcast_ttl_value, sizeof(mcast_ttl_value) );
        USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_NOT_INITIALIZED );
    }

    /** Add socket set */
    usr_err = _add_into_socket_set( p_ctrl, *p_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close UDP socket
 *
 * @retval USR_SUCCESS              Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_close_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t socket )
{
    /** Status */
    usr_err_t usr_err;
    int32_t sock_err;

    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Update socket set */
    usr_err = _remove_from_socket_set( p_ctrl, socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

#if 0   /// Unnecessary ?
    /** Shutdown the socket. */
    sock_err = lwip_shutdown(socket, SHUT_RDWR);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err , USR_ERR_ABORTED );
#endif

    /** Close socket. */
    sock_err = lwip_close(socket);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err , USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Check the readable socket
 *
 * @param[in] p_ctrl                Pointer to the controller
 *
 * @retval USR_SUCCESS              Some sockets detect any events.
 * @retval USR_ERR_NOT_ENABLED      No events.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_check_all_socket_events( opener_port_tcpip_netif_ctrl_t * const p_ctrl, uint32_t timeout_ms )
{
    /** number of ready socket */
    int num_ready = 0;

    /** timeout value */
    struct timeval timeout;

    /** Set timeout */
    timeout.tv_sec = (long) timeout_ms / 1000;
    timeout.tv_usec = (long) ( ( timeout_ms % 1000 ) * 1000 );

    /** Copy socket set to check if sockets is readable. */
    memcpy( &p_ctrl->readable_socket_set, &p_ctrl->socket_set, sizeof(struct fd_set) );

    /** Select */
    num_ready = lwip_select(p_ctrl->highest_socket + 1, &p_ctrl->readable_socket_set, NULL, NULL, &timeout);
    USR_ERROR_RETURN( 0 < num_ready , USR_ERR_NOT_ENABLED );

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the highest socket number
 *
 * @param[in] p_ctrl                Pointer to the controller
 *
 * @retval USR_SUCCESS              Some sockets detect any events.
 * @retval USR_ERR_NOT_ENABLED      No events.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_get_highest_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * p_highest_socket )
{
    /** Copy highest socket. */
    *p_highest_socket = p_ctrl->highest_socket;

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the highest socket number
 *
 * @param[in] p_ctrl                Pointer to the controller
 *
 * @retval USR_SUCCESS              The socket is readable
 * @retval USR_ERR_NOT_ENABLED      Ths socket is not readable
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_check_socket_readable( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t socket )
{
    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Check if the socket is readable */
    if ( FD_ISSET( socket, &p_ctrl->readable_socket_set ) )
    {
        /** return success code. */
        return USR_SUCCESS;
    }

    /** Return error code. */
    return USR_ERR_NOT_ENABLED;
}

/*******************************************************************************************************************//**
 * @brief Accept TCP session
 *
 * @param[in] p_ctrl                    Pointer to the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_ABORTED              Process has been aborted.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_accept_tcp_session ( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                          int32_t const listner_socket,
                                                          int32_t * const session_socket,
                                                          struct sockaddr * peer_address )
{
    /** Error Status */
    usr_err_t   usr_err;

    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( listner_socket, &p_ctrl->socket_set ) );

    /** Accept client socket */
    *session_socket = lwip_accept( listner_socket, (struct sockaddr *) peer_address, NULL);
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_INVALID != *session_socket , USR_ERR_ABORTED );

    /** Set Qos option */
    usr_err = _set_qos_value( *session_socket, CipQosGetDscpPriority(kConnectionObjectPriorityExplicit) );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Add socket set */
    usr_err = _add_into_socket_set( p_ctrl, *session_socket );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_ABORTED );

    /** Return success code */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * @brief Send TCP payload by using network platform.
 *
 * @retval USR_SUCCESS        Process has been done successfully.
 * @retval USR_ERR_ABORTED    Fail to send the frame
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_send_tcp_packet ( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                       int32_t           socket,
                                                       const void      * p_buffer,
                                                       uint32_t          buffer_size,
                                                       int32_t         * p_sent_size)
{
    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Sent size or error code. */
    int32_t sent_size;

    /** Send data */
    sent_size = lwip_send( socket, p_buffer, buffer_size, 0);
    USR_ERROR_RETURN( 0 <= sent_size, USR_ERR_ABORTED );

    /** Return sent size. */
    if( p_sent_size )
    {
        * p_sent_size = sent_size;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * @brief Receive TCP payload by using network platform.
 *
 * @retval USR_SUCCESS        Process has been done successfully.
 * @retval USR_ERR_ABORTED    Fail to send the frame
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_recv_tcp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t socket,
                                                       void * p_buffer,
                                                       uint32_t buffer_size,
                                                       int32_t * p_recv_size)
{
    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Sent size or error code. */
    int32_t recv_size;

    /** Send data */
    recv_size = lwip_recv( socket, p_buffer, buffer_size, 0 );
    USR_ERROR_RETURN( 0 <= recv_size, USR_ERR_ABORTED );

    /** Return sent size. */
    if( p_recv_size )
    {
        * p_recv_size = recv_size;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * @brief Send UDP payload by using network platform.
 *
 * @retval USR_SUCCESS        Process has been done successfully.
 * @retval USR_ERR_ABORTED    Fail to send the frame
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_send_udp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t           socket,
                                                       const void      * p_buffer,
                                                       uint32_t          buffer_size,
                                                       struct sockaddr * p_address,
                                                       int32_t         * p_sent_size)
{
    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Sent size or error code. */
    int32_t sent_size;

    /** Send data */
    sent_size = lwip_sendto(socket, p_buffer, buffer_size, 0, (struct sockaddr *) p_address, sizeof(struct sockaddr));
    USR_ERROR_RETURN( 0 <= sent_size, USR_ERR_ABORTED );

    /** Return sent size. */
    if( p_sent_size )
    {
        * p_sent_size = sent_size;
    }

    /** Return success code. */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * @brief Receive UDP payload by using network platform.
 *
 * @retval USR_SUCCESS        Process has been done successfully.
 * @retval USR_ERR_ABORTED    Fail to send the frame
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_recv_udp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t socket,
                                                       void * p_buffer,
                                                       uint32_t buffer_size,
                                                       int32_t * p_recv_size,
                                                       struct sockaddr * p_src_addr,
                                                       struct sockaddr * p_dst_addr)
{
    /** Message control buffer For getting destination src_addr */
    void * p_message_control_buffer;

    /** Message control handler */
    struct cmsghdr * p_message_control_handler;

    /** Packet information */
    struct in_pktinfo * p_packet_info;

    /** Resolve sockaddr type */
    struct sockaddr_in * p_dst_addr_in = (struct sockaddr_in *) p_dst_addr;

    /** Message buffer */
    struct iovec message_buffer_iovec =
    {
        .iov_base = p_buffer,
        .iov_len  = buffer_size,
    };

    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** Get message buffer. */
    USR_HEAP_ALLOCATE( p_message_control_buffer, OPENER_PORT_SOCKET_MESSAGE_CONTROL_BUFFER );
    {
        /** Message handler structure. */
        struct msghdr message_handler =
        {
            .msg_name       = p_src_addr,
            .msg_namelen    = sizeof(struct sockaddr),
            .msg_iov        = &message_buffer_iovec,
            .msg_iovlen     = 1,
            .msg_control    = p_message_control_buffer,
            .msg_controllen = OPENER_PORT_SOCKET_MESSAGE_CONTROL_BUFFER,
            .msg_flags      = 0,
        };

        /** Receive UDP data. */
        *p_recv_size = lwip_recvmsg(socket, &message_handler, 0);
        USR_HEAP_ERROR_RETURN( p_message_control_buffer, 0 <= *p_recv_size, USR_ERR_ABORTED );

        /** Scan message control buffer with message controller handler  */
        for( p_message_control_handler = CMSG_FIRSTHDR(&message_handler) ;
             p_message_control_handler != NULL ;
             p_message_control_handler = CMSG_NXTHDR(&message_handler, p_message_control_handler))
        {
            /** Check the protocol type */
            if( (p_message_control_handler->cmsg_level == IPPROTO_IP) &&
                (p_message_control_handler->cmsg_type  == IP_PKTINFO) )
            {
                /** Get packet information */
                p_packet_info = (struct in_pktinfo *) CMSG_DATA(p_message_control_handler);
                break;
            }
        }

        /** Get destination address from the packet information. */
        if( p_dst_addr_in )
        {
            p_dst_addr_in->sin_addr = p_packet_info->ipi_addr;
        }
    }
    /** Release heap */
    USR_HEAP_RELEASE( p_message_control_buffer );

    /** Return success */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * @brief Get the remote address of a specified socket handle.
 *
 * @retval USR_SUCCESS        Process has been done successfully.
 * @retval USR_ERR_ABORTED    Fail to process.
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_get_remote_address ( opener_port_tcpip_netif_ctrl_t * p_ctrl, int32_t socket, struct sockaddr * address )
{
    /** Check if the socket is in control. */
    USR_ASSERT( FD_ISSET( socket, &p_ctrl->socket_set ) );

    /** error status */
    int32_t status;

    /** address length */
    socklen_t address_len;

    /** Get remote address */
    status = lwip_getpeername( socket, (struct sockaddr *) address, &address_len);
    USR_ERROR_RETURN( status == 0, USR_ERR_ABORTED );

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get QoS settable non-volatile configuration.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ASSERTION                Assertion error
 **********************************************************************************************************************/
usr_err_t um_opener_port_tcpip_netif_get_cip_qos_nvdata( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                          opener_port_cip_qos_nvdata_t * const p_cip_qos_cfg )
{
    /** Resolve type */
    CipQosObject * p_qos_cip_instance = p_ctrl->p_qos_cip_instance;
    (void) p_qos_cip_instance;

    /** Check output argument pointer */
    USR_ASSERT( p_cip_qos_cfg );

    /** Copy the DSCP values. */
    p_cip_qos_cfg->dscp_explict   = CipQosGetDscpPriority(kConnectionObjectPriorityExplicit);
    p_cip_qos_cfg->dscp_high      = CipQosGetDscpPriority(kConnectionObjectPriorityHigh);
    p_cip_qos_cfg->dscp_low       = CipQosGetDscpPriority(kConnectionObjectPriorityLow);
    p_cip_qos_cfg->dscp_scheduled = CipQosGetDscpPriority(kConnectionObjectPriorityScheduled);
    p_cip_qos_cfg->dscp_urgent    = CipQosGetDscpPriority(kConnectionObjectPriorityUrgent);

    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Open CIP QoS interface object instance
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _open_cip_qos_instance( opener_port_tcpip_netif_ctrl_t * const p_ctrl )
{
    /** Set QoS values */
    p_ctrl->p_qos_cip_instance = &g_qos;

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Initialize CIP QoS interface object instance attributes
 *
 * NOTE:
 *  This function initialize all attributes even if the attributes have already initialized on definition or on CipStackInit() function.
 *  This function should be called after the CIP stack initialization to overwrite attributes by the CIP initialization.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _init_cip_qos_instance_attributes( opener_port_tcpip_netif_ctrl_t * const p_ctrl )
{
    /** Error code */
    usr_err_t usr_err;

    /** Resolve type */
    CipQosObject * p_qos_cip_instance = p_ctrl->p_qos_cip_instance;

    /** Disable 802.1Q tagging (#1). */
    p_qos_cip_instance->q_frames_enable = false;

    if( p_ctrl->p_cip_nvdata->p_qos_nvdata )
    {
        /** Set PTP DSCP values (#2 ~ #3) to default value. This attribute is NOT settable in this stack. */
        p_qos_cip_instance->dscp.event = QOS_DSCP_DEFAULT_EVENT;
        p_qos_cip_instance->dscp.general = QOS_DSCP_DEFAULT_GENERAL;

        /** Set DSCP values (#4 ~ #8) to configuration value. */
        usr_err = _set_cip_qos_dscp_values( p_ctrl, p_ctrl->p_cip_nvdata->p_qos_nvdata );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED )
    }
    else
    {
        /** Set DSCP values (#2 ~ #8) to default value.*/
        (void) CipQosResetAttributesToDefaultValues();
    }

    /** Apply the object value to actual values. */
    (void) CipQosUpdateUsedSetQosValues();

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set QoS DSCP values
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _set_cip_qos_dscp_values( opener_port_tcpip_netif_ctrl_t * const p_ctrl, opener_port_cip_qos_nvdata_t const * const p_cfg  )
{
    /** Resolve type */
    CipQosObject * p_qos_cip_instance = p_ctrl->p_qos_cip_instance;

    /** Copy the QoS values*/
    p_qos_cip_instance->dscp.explicit_msg = p_cfg->dscp_explict;
    p_qos_cip_instance->dscp.high         = p_cfg->dscp_high;
    p_qos_cip_instance->dscp.low          = p_cfg->dscp_low;
    p_qos_cip_instance->dscp.scheduled    = p_cfg->dscp_scheduled;
    p_qos_cip_instance->dscp.urgent       = p_cfg->dscp_urgent;

    /** Return success */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set the socket to non-blocking mode
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 * @retval USR_ERR_ABORTED  Fail to create socket.
 **********************************************************************************************************************/
static usr_err_t _enable_non_blocking( int32_t socket )
{
    /** Error status */
    int32_t sock_err;

    /** Option parameter */
    struct timeval timeval;
    timeval.tv_sec = 10;
    timeval.tv_usec = 0;

    /** Set the timeout for receiving */
    sock_err = lwip_setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, &timeval, sizeof(timeval) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_ABORTED );

    /** Set the timeout for sending */
    sock_err = lwip_setsockopt( socket, SOL_SOCKET, SO_SNDTIMEO, &timeval, sizeof(timeval) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_ABORTED );

    /** return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set QoS DSCP value
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 * @retval USR_ERR_ABORTED  Fail to create socket.
 **********************************************************************************************************************/
static usr_err_t _set_qos_value( int32_t socket, uint8_t qos_value )
{
    /** Error status */
    int32_t sock_err;

    /** Qos value shifted for DSCP field */
    uint32_t set_tos = ((uint32_t) qos_value << 2);

    /** Set the qos value*/
    sock_err = lwip_setsockopt(socket, IPPROTO_IP, IP_TOS, &set_tos, sizeof(set_tos) );
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_ABORTED );

    /** return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Set the socket option to reuse address
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 * @retval USR_ERR_ABORTED  Fail to create socket.
 **********************************************************************************************************************/
static usr_err_t _enable_reuse_address( int32_t socket )
{
    /** Error status */
    int32_t sock_err;

    /** Option parameter */
    int32_t reuse_addr = 1;

    /** Set the option */
    sock_err = lwip_setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    USR_ERROR_RETURN( OPENER_PORT_SOCKET_ERROR != sock_err, USR_ERR_ABORTED );

    /** retunc success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Add socket into socket set
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _add_into_socket_set( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t const socket )
{
    /** Add socket into set */
    FD_SET( socket, &p_ctrl->socket_set );

    /** Update highest socket. */
    if ( p_ctrl->highest_socket < socket )
    {
        p_ctrl->highest_socket = socket;
    }

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Remove socket from socket set
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _remove_from_socket_set( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t const socket )
{
    /** Clear socket into set */
    FD_CLR( socket, &p_ctrl->socket_set );

    /** Update the highest socket. */
    if( socket == p_ctrl->highest_socket )
    {
        /** Search highest socket */
        for( ; p_ctrl->highest_socket > OPENER_PORT_SOCKET_INVALID; p_ctrl->highest_socket-- )
        {
            /** Find highest socket */
            if ( FD_ISSET( p_ctrl->highest_socket, &p_ctrl->socket_set) )
            {
                break;
            }
        }
    }

    return USR_SUCCESS;
}
