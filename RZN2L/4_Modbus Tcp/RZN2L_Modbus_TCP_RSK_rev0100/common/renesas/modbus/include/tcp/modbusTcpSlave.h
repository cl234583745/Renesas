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

#ifndef _MODBUSTCPSLAVE_H_
#define _MODBUSTCPSLAVE_H_

/*============================================================================*/
/* I N C L U D E                                                              */
/*============================================================================*/
#include <slave/modbusSlave.h>

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
/**
 * @brief MODBUS stack gateway slave disable
 *
 */
#define MODBUS_TCP_GW_SLAVE_DISABLE        0

/**
 * @brief MODBUS stack gateway slave enable
 *
 */
#define MODBUS_TCP_GW_SLAVE_ENABLE        1

/** @brief Disable Multiple client Connection
 *
 * by setting this value, multiple client connection is disabled
 */

#define DISABLE_MULTIPLE_CLIENT_CONNECTION	         0

/** @brief Enable Multiple client Connection
 *
 * by setting this value, multiple client connection is enabled
 */

#define ENABLE_MULTIPLE_CLIENT_CONNECTION	         1

/*===============================================================================================================================*/
/* E N U M                                                                                                                       */
/*===============================================================================================================================*/
/** 
 * Host IP List Enable flag
 */
typedef enum
{
    DISABLE = 0, ENABLE
} ENABLE_FLAG;

/** 
 * Mode of host IP List
 */
typedef enum
{
    REJECT = 0, ACCEPT
} TABLE_MODE;

/*===============================================================================================================================*/
/* P R O T O T Y P E S                                          		                                                         */
/*===============================================================================================================================*/
/** @brief MODBUS terminate TCP Server stack API
 *
 *  This API terminate MODBUS TCP stack related task and the mailbox used for the TCP task.
 *
 *  @retval ERR_OK on successful termination
 *  @retval ERR_STACK_TERM if termination failed
 */
uint32_t Modbus_tcp_server_terminate_stack(void_t);

/** @brief MODBUS TCP server (without gateway) stack initialization
 *
 * This API is used to initialize the TCP stack. A task is activated which wait for client connection in default MODBUS port and in
 *  additional port (other than default port 502) if provided by the user. Separate task to read the request and process the request
 *  are activated. A mailbox is created to communicate between this tasks.
 *
 *  @param[in] u32_additional_port     Additional port value configured by user
 *  @param[in] u8_tcp_multiple_client  Status whether multiple client is enabled
 *
 * @retval ERR_OK on successful initialization of the task or mailbox
 *  @retval ERR_STACK_INIT if initialization of the task or mailbox failed
 */
uint32_t Modbus_tcp_server_init_stack(uint32_t u32_additional_port, uint8_t u8_tcp_multiple_client);

/** @brief MODBUS TCP terminate stack API
 *
 *  This API terminate MODBUS stack. Depending upon the stack mode, corresponding APIs are invoked. If the stack mode is 
 *  MODBUS_TCP_SERVER_MODE, Modbus_tcp_terminate_stack() is invoked. If stack mode is gateway, Modbus_tcp_terminate_gateway_stack() 
 *  is invoked.
 *
 *  @retval ERR_OK on successful termination
 *  @retval ERR_STACK_TERM if termination failed
 */
uint32_t Modbus_tcp_terminate_stack(void_t);

/** @brief MODBUS set host IP list properties
 *
 *  This function is used for specifying mode(accept/reject) and to Enable or Disable the list of IP address by the user. 
 *  By default the host IP list is disabled.
 *
 *  @param e_flag          Status is whether the connection table enabled or disabled
 *  @param e_mode          Status indicating the list contain IP to be accepted or rejected
 *
 *  @return void_t
 */
void_t Modbus_tcp_init_ip_table(ENABLE_FLAG e_flag, TABLE_MODE e_mode);

/** @brief MODBUS add an IP to host IP list
 *
 *  This function is used for adding a IP to the host IP list.
 *
 *  @param pu8_add_ip       Host IP address in numbers and dots notation
 *
 *  @return ERR_OK on successful addition.
 *  @return ERR_IP_ALREADY_PRESENT if address already present in list.
 *  @return ERR_MAX_CLIENT if maximum connections reached.
 *  @return ERR_TABLE_DISABLED if list is disabled
 */
uint32_t Modbus_tcp_add_ip_addr(pchar_t pu8_add_ip);

/** @brief MODBUS remove an IP from  host IP list
 *
 *  This function is used for removing a host IP from the list.
 *
 *  @param pu8_del_ip         Host IP address in numbers and dots notation
 *
 *  @return ERR_OK on successful deletion.
 *  @return ERR_IP_NOT_FOUND if IP is not in the list
 *  @return ERR_TABLE_EMPTY if the list is empty
 *  @return ERR_TABLE_DISABLED if the table is disabled, i.e., server accepts connection request from any host
 */
uint32_t Modbus_tcp_delete_ip_addr(pchar_t pu8_del_ip);

/** @brief Wait for connection task
 *
 *  This task waits for a connection from a client in the default MODBUS port(502) and an additional port if configured by user.
 *  Verify whether the IP table is enabled or not. If enabled, verify the list contains the IP list to accepted or rejected. 
 *  Accordingly save the socket descriptor to the connection list.
 *
 *  @retval void
 */
void_t Modbus_tcp_soc_wait_task(void_t);

/** @brief Receive data Task
 *
 *  This task waits for a request received in the selected socket ID. It verify the packet is for MODBUS protocol. If so, write the
 *  request to the receive mailbox. If the mailbox is found full, send an error server busy to the client.
 *
 *  @retval void
 */
void_t Modbus_tcp_recv_data_task(void_t);

/** @brief Request processing task
 *
 *  This task wait for a request in the queue. Verify the slave ID in the request packet to determine the packet is for the TCP 
 *  server or the device connected to it serially. If the packet is for the TCP server process the request read from the queue, 
 *  prepare the response packet and send it to the TCP client. If the packet is for the serial device connected, write the request 
 *  to the gateway mailbox.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_req_process_task(void_t);

/** @brief Gateway task
 *
 *  This task wait for a request in the gateway queue for processing the data in the serial device connected to the TCP server. 
 *  It process the request read  from the queue, prepare the response packet and send it to the TCP client.
 *
 *  @retval void
 */
void_t Modbus_gateway_task(void_t);

#endif /* _MODBUSTCPSLAVE_H_ */
/* End of File */
