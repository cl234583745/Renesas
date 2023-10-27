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

#ifndef _MODBUSPORTTCP_H_
#define _MODBUSPORTTCP_H_

/*===============================================================================================================================*/
/* I N C L U D E                                                                                                                 */
/*===============================================================================================================================*/
#include <modbusTypedef.h>
#include <port/modbusPortRtos.h>
#include "FreeRTOS.h"
#include "FreeRTOS_Sockets.h"

/*===============================================================================================================================*/
/* M A C R O D I F I N I T I O N S                                                                                               */
/*===============================================================================================================================*/
#define BSD_SOCKET_MAX 10

/*===============================================================================================================================*/
/* T Y P E D E F S                                                                                                               */
/*===============================================================================================================================*/
typedef struct in_addr
{
    uint32_t s_addr;
} in_addr;

typedef struct sockaddr_in
{
    uint8_t sin_len;
    uint8_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    uint8_t sin_zero[8];
} sockaddr_in;

typedef struct sockaddr
{
    uint8_t sa_len;
    uint8_t sa_family;
    uint8_t sa_data[14];
} sockaddr;

/*===============================================================================================================================*/
/* E N U M                                                                                                                       */
/*===============================================================================================================================*/
typedef enum
{
    MODBUS_TCP_SELECT_NO_EVENT = 0x00,
    MODBUS_TCP_SELECT_READ = 0x01,
    MODBUS_TCP_SELECT_EXCEPTION = 0x02,
    MODBUS_TCP_SELECT_INVARID_SOCKET = 0x03
}MODBUS_TCP_SELECT_EVENT;

/*===============================================================================================================================*/
/* P R O T O T Y P E S                                                                                                           */
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
int8_t Modbus_tcp_init_socket(uint16_t u16_port, pint32_t ps32_listen_fd);

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
int32_t Modbus_tcp_send_packet(uint8_t u8_socket_id, puint8_t pu8_mailbox_send_packet, uint32_t u32_send_packet_length);

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
int32_t Modbus_tcp_receive_packet(uint8_t u8_socket_id, puint8_t pu8_mailbox_receive_packet, uint32_t u32_receive_packet_length);

/** @brief  Count readable sockets.
 *
 *  This function finds readable sockets for listen socket.
 *
 *  @param[in] 	s32_maximum_socket_id    Require (the maximum number in socket fds included the following fdsets).
 *  @param[in] 	s32_timeout_ms           The timeout value.
 *
 *  @return The number of readable or disconnected sockets.
 */
int32_t Modbus_tcp_listen_select(int32_t s32_maximum_socket_id, uint32_t u32_timeout_ms);

/** @brief Test if a socket belongs to a socket-set and if so, which event bit(s) are set.
 *
 * @param[in] s32_listen_fd_index    Specify the id of the socket.
 *
 * @return If the socket belongs to the socket set: the event bits, otherwise zero.
 */
int32_t Modbus_tcp_check_listen_socket_acceptable(int32_t s32_listen_fd_index);

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
int32_t Modbus_tcp_accept_socket(int32_t s32_sockfd, struct sockaddr * pt_address, socklen_t * pt_address_length);

/** @brief  Count readable or disconnected sockets.
 *
 *  This function finds readable or disconnected sockets.
 *
 *  @param[in]      s32_maximum_socket_id        Require (the maximum number in socket fds included the following fdsets).
 *  @param[in]      u32_timeout_ms               The timeout value.
 *
 *  @return The number of readable or disconnected sockets.
 */
int32_t Modbus_tcp_receive_select(int32_t s32_maximum_socket_id, uint32_t u32_timeout_ms);

/** @brief Test if a socket belongs to a socket-set and if so, which event bit(s) are set.
 *
 * @param[in] s32_connected_fd_index    Specify the id of the socket.
 *
 * @return If the socket belongs to the socket set: the event bits, otherwise zero.
 */
int32_t Modbus_tcp_check_connected_socket_receptible(int32_t s32_listen_fd_index);

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
int32_t Modbus_tcp_close_connected_socket(int32_t s32_fd_index);

/** @brief This function gets the IP address expressed as a 32-bit number of network byte order.
 *
 *  @param[in] * pu8_ip_address  The character string pointer holding the IP-address in the "W.X.Y.Z"(dotted decimal) format.
 *
 *  @return If the format of the string pointed to by the pu8_ip_address parameter is valid then the same IP address expressed as a 32-bit number in network byte order is returned.
 *  @return In all other cases 0 is returned.
 */
uint32_t Modbus_tcp_inet_addr(const uint8_t * pu8_ip_address);

#endif /* _MODBUSPORTTCP_H_ */
/* End of File */

