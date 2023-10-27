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

/*===============================================================================================================================*/
/* I N C L U D E                                                                                                                 */
/*===============================================================================================================================*/
#include <port/modbusPortTcp.h>
#include <tcp/modbusTcp.h>
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/*===============================================================================================================================*/
/* G L O B A L  V A R I A B L E S                                                                                                */
/*===============================================================================================================================*/


/*===============================================================================================================================*/
/* S T A T I C  V A R I A B L E S                                                                                                */
/*===============================================================================================================================*/
static Socket_t spt_modbus_fd[BSD_SOCKET_MAX] = { NULL };
static SocketSet_t spt_listener_socketset = NULL;
static SocketSet_t spt_connected_socketset = NULL;

/*===============================================================================================================================*/
/* S T A T I C  F U N C T I O N   P R O T O T Y P E                                                                             */
/*===============================================================================================================================*/
static int32_t Modbus_fd_set_listener_socketset(int32_t s32_listen_fd_index);
static int32_t Modbus_fd_set_connected_socketset(int32_t s32_listen_fd_index);

/*===============================================================================================================================*/
/* D I F I N E M A C O                                                                                                           */
/*===============================================================================================================================*/
#define INADDR_ANY              ((uint32_t)0U)            /* Auto IP ADDRESS */

/*===============================================================================================================================*/
/* S T A T I C  P R O G R A M                                                                                                                 */
/*===============================================================================================================================*/
/** @brief Add a socket to a set.
 *
 * @param[in] s32_listen_fd_index    Specify the id of the socket.
 *
 * @return ERR_OK                    Success fd_set to listen socket.
 * @return ERR_TCP_INVALID_SOCKET    Socket id is invalid
 */
static int32_t Modbus_fd_set_listener_socketset(int32_t s32_listen_fd_index)
{
    if(spt_modbus_fd[s32_listen_fd_index] == NULL)
    {
        return ERR_TCP_INVALID_SOCKET;
    }
    FreeRTOS_FD_SET(spt_modbus_fd[s32_listen_fd_index],spt_listener_socketset,eSELECT_READ);
    return ERR_OK;
}

/** @brief Add a connected socket to a set.
 *
 * @param[in] s32_listen_fd_index    Specify the id of the socket.
 *
 * @return ERR_OK                    Success fd_set to listen socket.
 * @return ERR_TCP_INVALID_SOCKET    Socket id is invalid
 */
static int32_t Modbus_fd_set_connected_socketset(int32_t s32_listen_fd_index)
{
    if(spt_modbus_fd[s32_listen_fd_index] == NULL)
    {
        return ERR_TCP_INVALID_SOCKET;
    }
    FreeRTOS_FD_SET(spt_modbus_fd[s32_listen_fd_index],spt_connected_socketset,eSELECT_READ);
    FreeRTOS_FD_SET(spt_modbus_fd[s32_listen_fd_index],spt_connected_socketset,eSELECT_EXCEPT);
    return ERR_OK;
}
/*===============================================================================================================================*/
/* P R O G R A M                                                                                                                 */
/*===============================================================================================================================*/
/** @brief Function for creating server socket
 *
 *  This function is used for creating the server socket and turns the server to accept mode for monitoring client connections.
 *
 *  @param  u16_port        Port number to which socket is to be binded.
 *  @param  ps32_listen_fd  Socket descriptor binded
 * 
 *  @return ERR_OK                  Create socket is success
 *  @return ERR_TCP_INVALID_SOCKET  If socket creation fails
 *  @return ERR_TCP_ABORTED         Created socket is not found
 */
int8_t Modbus_tcp_init_socket(uint16_t u16_port, pint32_t ps32_listen_fd)
{
    struct freertos_sockaddr t_serv_addr;
    int32_t s32_return = ERR_OK;
    Socket_t pt_socket;
    BaseType_t t_err_code;
    int32_t s32_fd_index;

    if(NULL == spt_listener_socketset)
    {
        spt_listener_socketset = FreeRTOS_CreateSocketSet();
    }
    if(NULL == spt_connected_socketset)
    {
        spt_connected_socketset = FreeRTOS_CreateSocketSet();
    }
    pt_socket = FreeRTOS_socket (FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP);
    if (pt_socket == FREERTOS_INVALID_SOCKET)
    {
        DEBUG_PRINT("\nError in socket creation\n");
        return ERR_TCP_INVALID_SOCKET;
    }
    memset (&t_serv_addr, '0', sizeof(t_serv_addr));

    t_serv_addr.sin_family = FREERTOS_AF_INET;
    t_serv_addr.sin_addr = FreeRTOS_htonl(INADDR_ANY);
    t_serv_addr.sin_port = FreeRTOS_htons(u16_port);
    t_err_code = FreeRTOS_bind (pt_socket, &t_serv_addr, sizeof(t_serv_addr));
    if (0 > t_err_code)
    {
        DEBUG_PRINT("\nSocket bind error\n");
        FreeRTOS_closesocket(pt_socket);
        return ERR_TCP_INVALID_SOCKET;
    }
    t_err_code = FreeRTOS_listen (pt_socket, BSD_SOCKET_MAX);
    if (0 > t_err_code)
    {
        DEBUG_PRINT("\nSocket listen failed\n");
        FreeRTOS_closesocket(pt_socket);
        return ERR_TCP_INVALID_SOCKET;
    }
    for (s32_fd_index = 0; s32_fd_index < BSD_SOCKET_MAX; s32_fd_index++)
    {
        if (spt_modbus_fd[s32_fd_index] == NULL)
        {
            spt_modbus_fd[s32_fd_index] = pt_socket;
            s32_return = Modbus_fd_set_listener_socketset(s32_fd_index);
            break;
        }
    }
    if (spt_modbus_fd[s32_fd_index] != pt_socket)
    {
        DEBUG_PRINT("\nError in socket creation\n");
        return ERR_TCP_ABORTED;
    }
    *(ps32_listen_fd) = s32_fd_index;

    return (int8_t) s32_return;
}

/** @brief Modbus TCP send packet
 *
 *  This function sends the prepared packet to lower layer (transport layer). 
 *
 *  @param[in] u8_socket_id                 Socket ID to which the data is to be transmitted
 *  @param[in] pu8_mailbox_send_packet      Pointer to the MODBUS send packet array.
 *  @param[in] u32_send_packet_length       Length of the MODBUS send packet
 *
 *  @return ERR_OK                          On successfully send the packet
 *  @return ERR_TCP_INVALID_SOCKET          The specified socket does not exist
 *  @return ERR_TCP_ABORTED                 If packet sending failed
 */
int32_t Modbus_tcp_send_packet(uint8_t u8_socket_id, puint8_t pu8_mailbox_send_packet, uint32_t u32_send_packet_length)
{
    int32_t s32_return = ERR_OK;
    int32_t s32_snd_length = 0;
    if(NULL == spt_modbus_fd[u8_socket_id])
    {
        return ERR_TCP_INVALID_SOCKET;
    }
    s32_snd_length = FreeRTOS_send (spt_modbus_fd[u8_socket_id], pu8_mailbox_send_packet, u32_send_packet_length, 0);
    if (0 > s32_snd_length)
    {
        s32_return = ERR_TCP_ABORTED;
    }
    return s32_return;
}

/** @brief Modbus TCP receive packet
 *
 *  This function receive the prepared packet to lower layer (transport layer). 
 *
 *  @param[in] u8_socket_id                             Socket ID
 *  @param[in] pu8_mailbox_receive_packet               Pointer to the MODBUS receive packet array.
 *  @param[in] uint32_t u32_receive_packet_length       Length of the MODBUS receive packet
 *
 *  @return packet receive data length                  Receive packet data length
 *  @return ERR_TCP_INVALID_SOCKET                      Socket is not found
 *  @return ERR_TCP_ABORTED                             If packet receiving failed
 */
int32_t Modbus_tcp_receive_packet(uint8_t u8_socket_id, puint8_t pu8_mailbox_receive_packet, uint32_t u32_receive_packet_length)
{
    int32_t s32_receive_length = 0;
    if(NULL == spt_modbus_fd[u8_socket_id])
    {
        return ERR_TCP_INVALID_SOCKET;
    }
    s32_receive_length = FreeRTOS_recv (spt_modbus_fd[u8_socket_id], pu8_mailbox_receive_packet, u32_receive_packet_length, 0);
    if(0 > s32_receive_length)
    {
        return ERR_TCP_ABORTED;
    }
    return s32_receive_length;
}

/** @brief  Count readable sockets.
 *
 *  This function finds readable sockets for listen socket.
 *
 *  @param[in] 	s32_maximum_socket_id    Require (the maximum number in socket fds included the following fdsets).
 *  @param[in] 	s32_timeout_ms           The timeout value.
 *
 *  @return The number of readable or disconnected sockets.
 */
int32_t Modbus_tcp_listen_select(int32_t s32_maximum_socket_id, uint32_t u32_timeout_ms)
{
    /** The result of FreeRTOS_select() . */
    BaseType_t t_result;

    TickType_t t_block_time_ticks = 0;

    /** The counter of the number of readable or disconnected sockets. */
    int32_t s32_cnt = 0;

    /** File descriptor index in iteration. */
    int32_t s32_fd_index;

    /** Convert the type of timeout value for FreeRTOS_select() */
    t_block_time_ticks += (TickType_t) (u32_timeout_ms * configTICK_RATE_HZ / 1000);

    /** Call FreeRTOS_select() */
    t_result = FreeRTOS_select (spt_listener_socketset, t_block_time_ticks);

    /** If the FreeRTOS_select detects any events. */
    if (t_result != 0)
    {
        for (s32_fd_index = 0; s32_fd_index < s32_maximum_socket_id + 1; s32_fd_index++)
        {
            if (NULL == spt_modbus_fd[s32_fd_index])
            {
                continue;
            }
            if(FreeRTOS_FD_ISSET (spt_modbus_fd[s32_fd_index], spt_listener_socketset))
            {
                s32_cnt++;
            }
        }
    }
    /** Return the number of readable or disconnected socket. */
    return s32_cnt;
}

/** @brief Test if a socket belongs to a socket-set and if so, which event bit(s) are set.
 *
 * @param[in] s32_listen_fd_index    Specify the id of the socket.
 *
 * @return If the socket belongs to the socket set: the event bits, otherwise zero.
 */
int32_t Modbus_tcp_check_listen_socket_acceptable(int32_t s32_listen_fd_index)
{
    EventBits_t rtos_event = 0;
    if(spt_modbus_fd[s32_listen_fd_index] == NULL)
    {
        return MODBUS_TCP_SELECT_INVARID_SOCKET;
    }
    rtos_event = FreeRTOS_FD_ISSET(spt_modbus_fd[s32_listen_fd_index],spt_listener_socketset);

    if (eSELECT_READ == (eSELECT_READ & rtos_event))
    {
        return MODBUS_TCP_SELECT_READ;
    }
    return MODBUS_TCP_SELECT_NO_EVENT;
}

/** @brief Accept the connection on a TCP socket.
 *
 *  This function accepts the connection from clients.
 *
 *  @param[in]      s32_sockfd              Socket file descriptor index
 *  @param[in, out] * pt_address            A pointer that will be filled  with the IP address and port number of the socket from which a connection was accepted.
 *  @param[in, out] * pt_address_length     Not currently used
 *
 *  @return The number of connected sockets id.
 *  @return ERR_TCP_INVALID_SOCKET
 */
int32_t Modbus_tcp_accept_socket(int32_t s32_sockfd, struct sockaddr * pt_address, socklen_t * pt_address_length)
{
    /** File descriptor index in iteration. */
    int32_t s32_fd_index;

    /** The handle of the socket where connections request was accepted. */
    Socket_t pt_connected_socket;

    /** Call FreeRTOS_accept() */
    pt_connected_socket = FreeRTOS_accept (spt_modbus_fd[s32_sockfd], (struct freertos_sockaddr*) pt_address, pt_address_length);
    if (pt_connected_socket == FREERTOS_INVALID_SOCKET)
    {
        return ERR_TCP_INVALID_SOCKET;
    }

    for (s32_fd_index = 0; s32_fd_index < BSD_SOCKET_MAX; s32_fd_index++)
    {
        /** Store connected sockets in an array */
        if (NULL == spt_modbus_fd[s32_fd_index])
        {
            spt_modbus_fd[s32_fd_index] = pt_connected_socket;
            if(ERR_TCP_INVALID_SOCKET == Modbus_fd_set_connected_socketset(s32_fd_index))
            {
                return ERR_TCP_INVALID_SOCKET;
            }
            break;
        }
    }
    return s32_fd_index;
}

/** @brief  Count readable or disconnected sockets.
 *
 *  This function finds readable or disconnected sockets.
 *
 *  @param[in]      s32_maximum_socket_id        Require (the maximum number in socket fds included the following fdsets).
 *  @param[in]      u32_timeout_ms               The timeout value.
 *
 *  @return The number of readable or disconnected sockets.
 */
int32_t Modbus_tcp_receive_select(int32_t s32_maximum_socket_id, uint32_t u32_timeout_ms)
{
    /** The result of FreeRTOS_select() . */
    BaseType_t t_result;
    TickType_t t_block_time_ticks = 0;
    /** The counter of the number of readable or disconnected sockets. */
    int32_t s32_cnt = 0;
    /** File descriptor index in iteration. */
    int32_t s32_fd_index;

    t_block_time_ticks += (TickType_t) (u32_timeout_ms * configTICK_RATE_HZ / 1000);

    /** Call FreeRTOS_select() */
    t_result = FreeRTOS_select (spt_connected_socketset, t_block_time_ticks);
    /** If the FreeRTOS_select detects any events. */
    if (t_result != 0)
    {
        for (s32_fd_index = 0; s32_fd_index < s32_maximum_socket_id + 1; s32_fd_index++)
        {
            if (NULL == spt_modbus_fd[s32_fd_index])
            {
                continue;
            }
            if(Modbus_tcp_check_connected_socket_receptible (s32_fd_index))
            {
                s32_cnt++;
            }
        }
    }
    return s32_cnt;
}

/** @brief Test if a socket belongs to a socket-set and if so, which event bit(s) are set.
 *
 * @param[in] s32_connected_fd_index    Specify the id of the socket.
 *
 * @return If the socket belongs to the socket set: the event bits, otherwise zero.
 */
int32_t Modbus_tcp_check_connected_socket_receptible(int32_t s32_connected_fd_index)
{
    EventBits_t rtos_event = 0;
    if(NULL == spt_modbus_fd[s32_connected_fd_index])
    {
        return MODBUS_TCP_SELECT_INVARID_SOCKET;
    }
    rtos_event = FreeRTOS_FD_ISSET(spt_modbus_fd[s32_connected_fd_index],spt_connected_socketset);
    if (eSELECT_EXCEPT == (eSELECT_EXCEPT & rtos_event))
    {
        return MODBUS_TCP_SELECT_EXCEPTION;
    }
    if (eSELECT_READ == (eSELECT_READ & rtos_event))
    {
        return MODBUS_TCP_SELECT_READ;
    }
    return MODBUS_TCP_SELECT_NO_EVENT;
}

/** @brief Close the socket.
 *
 *  This function close the socket.
 *
 *  @param[in] s32_fd_index The socket fd will be closed.
 *
 *  @return ERR_TCP_ABORTED             The socket fd is invalid
 *  @return ERR_TCP_INVALID_SOCKET      The socket is not found
 *  @return ERR_OK                      Successfully close the socket.
 */
int32_t Modbus_tcp_close_connected_socket(int32_t s32_fd_index)
{
    /** The error code of FreeRTOS. */
    BaseType_t t_result;

    if (s32_fd_index >= BSD_SOCKET_MAX)
    {
        return ERR_TCP_ABORTED;
    }
    /** Check if the socket fd is valid. */
    if(spt_modbus_fd[s32_fd_index] == NULL)
    {
        return ERR_TCP_INVALID_SOCKET;
    }
    FreeRTOS_FD_CLR(spt_modbus_fd[s32_fd_index],spt_connected_socketset,eSELECT_ALL);
    /** Close the socket. */
    t_result = FreeRTOS_closesocket (spt_modbus_fd[s32_fd_index]);
    if (t_result == 0)
    {
        /** Return error code. */
        return ERR_TCP_ABORTED;
    }

    /** Delete the pointer of the socket. */
    spt_modbus_fd[s32_fd_index] = NULL;

    /** Return success code. */
    return ERR_OK;
}

/** @brief This function gets the IP address expressed as a 32-bit number of network byte order.
 *
 *  @param[in] * pu8_ip_address  The character string pointer holding the IP-address in the "W.X.Y.Z"(dotted decimal) format.
 *
 *  @return If the format of the string pointed to by the pu8_ip_address parameter is valid then the same IP address expressed as a 32-bit number in network byte order is returned.
 *  @return In all other cases 0 is returned.
 */
uint32_t Modbus_tcp_inet_addr(const uint8_t * pu8_ip_address)
{
    return FreeRTOS_inet_addr ((const char *) pu8_ip_address);
}

/* End of File */
