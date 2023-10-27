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

#ifndef _MODBUSTCP_H_
#define _MODBUSTCP_H_

#include <modbusCommon.h>
#include <modbusTypedef.h>
#include <tcp/modbusTcpConfig.h>
#include <tcp/modbusTcpSlave.h>
/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
/**
 * Error for mailbox 
 */
#define ERR_MAILBOX                      1

/**
 * Define used to indicate the maximum receive mailbox elements 
 */
#define MAX_RCV_MBX_SIZE               8

/**
 * Define used to indicate the maximum gateway mailbox elements 
 */
#define MAX_GW_MBX_SIZE                4

/**
 * Define used to indicate the maximum size of the TCP ADU size 
 */
#define MAX_TCP_PKT_SIZE                      260 

/**
 * Define to indicate the tcp header size
 */
#define MODBUS_TCP_HEADER_LEN                 7

/**
 * Define used to indicate the new connection is to be added to active connection list
 */
#define ADD_TO_CONN_LIST                      0

/**
 * Define used to indicate the new connection is to be removed from active connection list
 */
#define REMOVE_FROM_CONN_LIST                 1

/**
 * Index at which the oldest connection is placed in the connection list 
 */
#define OLDEST_CONN_IDX                       0

/**
 * Define used to indicate index at which count of connections placed in the list
 */
#define LIST_CNT_IDX                          MAXIMUM_NUMBER_OF_CLIENTS

/**
 * Define used to indicate the protocol ID of MODBUS 
 */
#define MODBUS_PROTOCOL                       0

/**
 * Slave ID of the MODBUS TCP device
 */
#define MODBUS_TCP_SLAVE_ID                   0xFF

/**
 * Define used to specify the timeout for socket select function
 */
#define SELECT_FUNC_TIMEOUT_US                100

/**
 * Define used to specify the exception code
 */
#define MODBUS_EXCEPTION_CODE                 0x80

/**
 * Define specifying the length of PDU in case of exception
 */
#define EXCEPTION_PDU_LEN                     2   

/**
 * Length of the PDU in the response packet of write functions 
 */
#define FUNC_CODE_WRITE_RESP_PDU_LEN          5

/**
 * Define used to indicate the reception of invalid packet
 */
#define INVALID_PKT_RECEPTION                 1

/**
 * Define used to indicate the packet is for the TCP server 
 */
#define MODBUS_TCP_PKT                        1

/**
 * Define used to indicate the packet is for the TCP server 
 */
#define MODBUS_GW_PKT                         2

/** 
 * Define used to indicate the index of different fields in the MODBUS TCP frame 
 */
#define TCP_TRANSACTION_IDX                   (0)
#define TCP_PROTOCOL_IDX                      (TCP_TRANSACTION_IDX + 2) 
#define RECV_PKT_LEN_IDX                      (TCP_PROTOCOL_IDX + 2) 
#define TCP_SLAVE_ID_IDX                      (RECV_PKT_LEN_IDX + 2) 
#define FUNCTION_CODE_IDX                     (TCP_SLAVE_ID_IDX + 1) 

#define FUNC_CODE_READ_START_ADDR_IDX         (FUNCTION_CODE_IDX + 1)
#define FUNC_CODE_READ_CNT_IDX                (FUNC_CODE_READ_START_ADDR_IDX + 2)

#define FUNC_CODE_WRITE_ADDR_IDX              (FUNCTION_CODE_IDX + 1)
#define FUNC_CODE_WRITE_VALUE_IDX             (FUNC_CODE_WRITE_ADDR_IDX + 2) 

#define FUNC_CODE_MULTIPLE_WRITE_CNT_IDX      (FUNC_CODE_WRITE_VALUE_IDX)
#define FUNC_CODE_MULTIPLE_WRITE_BYTE_CNT_IDX (FUNC_CODE_MULTIPLE_WRITE_CNT_IDX + 2) 
#define FUNC_CODE_MULTIPLE_WRITE_VALUE_IDX    (FUNC_CODE_MULTIPLE_WRITE_BYTE_CNT_IDX + 1)

#define FUNC_CODE_READ_WRITE_ADDR_IDX         (FUNC_CODE_READ_CNT_IDX + 2)
#define FUNC_CODE_READ_WRITE_VALUE_IDX        (FUNC_CODE_READ_WRITE_ADDR_IDX + 2)
#define FUNC_CODE_READ_WRITE_BYTE_CNT_IDX     (FUNC_CODE_READ_WRITE_VALUE_IDX + 2)
#define FUNC_CODE_READ_WRITE_REG_VALUE_IDX    (FUNC_CODE_READ_WRITE_BYTE_CNT_IDX + 1)

/** 
 * Define used to indicate the index of different fields in the MODBUS TCP PDU 
 */
#define TCP_PDU_FUNC_CODE_IDX                 (0)
#define TCP_PDU_READ_START_ADDR_IDX           (TCP_PDU_FUNC_CODE_IDX + 1)
#define TCP_PDU_READ_CNT_IDX                  (TCP_PDU_READ_START_ADDR_IDX + 2)

#define TCP_PDU_WRITE_ADDR_IDX                (TCP_PDU_READ_START_ADDR_IDX)
#define TCP_PDU_WRITE_VALUE_IDX               (TCP_PDU_READ_CNT_IDX)

#define TCP_PDU_MULTIPLE_WRITE_ADDR_IDX       (TCP_PDU_WRITE_ADDR_IDX)
#define TCP_PDU_MULTIPLE_WRITE_CNT_IDX        (TCP_PDU_WRITE_VALUE_IDX)
#define TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX   (TCP_PDU_MULTIPLE_WRITE_CNT_IDX + 2)
#define TCP_PDU_MULTIPLE_WRITE_VALUE_IDX      (TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX + 1)

#define TCP_PDU_READ_WRITE_ADDR_IDX           (TCP_PDU_READ_CNT_IDX + 2)
#define TCP_PDU_READ_WRITE_VALUE_IDX          (TCP_PDU_READ_WRITE_ADDR_IDX + 2)
#define TCP_PDU_READ_WRITE_BYTE_CNT_IDX       (TCP_PDU_READ_WRITE_VALUE_IDX + 2)
#define TCP_PDU_READ_WRITE_REG_VALUE_IDX      (TCP_PDU_READ_WRITE_BYTE_CNT_IDX + 1)

/**
 * Defines indicating the fixed size of the request PDU 
 */
#define READ_COIL_PDU_SIZE_CONST               (5)
#define READ_DISCRETE_INPUT_PDU_SIZE_CONST     (5)
#define READ_HOLDING_REG_PDU_SIZE_CONST        (5)
#define READ_INPUT_REG_PDU_SIZE_CONST          (5)
#define WRITE_SINGLE_COIL_PDU_SIZE_CONST       (5)
#define WRITE_SINGLE_REG_PDU_SIZE_CONST        (5)
#define WRITE_MULTIPLE_COIL_PDU_SIZE_CONST     (6)           
#define WRITE_MULTIPLE_REG_PDU_SIZE_CONST      (6)
#define READ_WRITE_MULTIPLE_REG_PDU_SIZE_CONST (10)

/**
 * Defines indicating the fixed size of the response PDU 
 */
#define RESP_PDU_SIZE_CONST                    (2)

/*===============================================================================================================================*/
/* E N U M                                                                                                                       */
/*===============================================================================================================================*/

/*===============================================================================================================================*/
/* S T R U C T U R E                                                                                                             */
/*===============================================================================================================================*/
/**
 * Structure defined for mailbox queue 
 */
typedef struct _req_mbx
{
    uint32_t u32_soc_id; /**< Socket ID at which the request arrived */
    puint8_t pu8_req_pkt; /**< Pointer to the requested packet  */
    uint32_t u32_pkt_len; /**< Packet length */
} mb_req_mbx_t;
typedef struct _req_mbx *p_mb_req_mbx_t;

/** 
 * List parameters
 */
typedef struct _table_param
{
    ENABLE_FLAG e_enable;
    TABLE_MODE e_mode;
    uint8_t u8_cnt;
} table_param_t;

/**
 * Structure contain the information required to form a response packet which is sent to server
 */
typedef struct _mb_tcp_pkt_info
{
    puint8_t pu8_output_response; /**< pointer to the response structure from the API */
    uint16_t u16_transaction_id; /**< specifies the transaction identifier */
    uint16_t u16_protocol_id; /**< specifies the protocol ID */
    uint8_t u8_slave_id; /**< specifies the slave ID of the device */
    uint16_t u16_num_of_bytes; /**< specifies the number of bytes in the data packet PDU field */
    uint8_t aru8_data_packet[MAX_DATA_SIZE];/**< contains the function and data  */
} mb_tcp_pkt_info_t;
typedef struct _mb_tcp_pkt_info *p_mb_tcp_pkt_info_t;

/*===============================================================================================================================*/
/* G L O B A L   V A R I A B L E S                                                                                               */
/*===============================================================================================================================*/
extern uint8_t gu8_stack_mode;

extern uint32_t gu32_config_port;

extern uint8_t gu8_gw_init;

extern uint8_t gu8_multiple_client_enable;

extern table_param_t gt_table_para;

extern uint32_t gu32_hostIP_table[MAXIMUM_NUMBER_OF_CLIENTS];

/*===============================================================================================================================*/
/* P R O T O T Y P E S                                          		                                                         */
/*===============================================================================================================================*/
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

/** @brief MODBUS shift TCP connection list 
 *
 *  This API is used to shift the connection list according to the latest active connection.
 *
 *  @param[in] pu8_conn_list     Pointer to array containing the connection list.
 *  @param[in] pu8_conn_idx      Index from which the socket ID is to be shifted.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_shift_conn_list(puint8_t pu8_conn_list, puint8_t pu8_conn_idx);

/** @brief MODBUS TCP remove from connection list 
 *
 *  This API is used to remove a connection established, from the connection list kept by the server. Verify the location at which 
 *  the socket ID is specified and shift the all connection by one.
 *
 *  @param[in] pu8_soc_id        Variable storing the socket ID.
 *  @param[in] pu8_conn_list     Pointer to array containing the connection list. 
 *
 *  @retval void_t
 */
void_t Modbus_tcp_remove_from_conn_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list);

/** @brief MODBUS add a connection to MODBUS TCP connection list 
 *
 *  This API is used to add a new connection received by the TCP server to the connection list kept by the server. Append the new 
 *  connection to the array one by one.
 *
 *  @param[in] pu8_soc_id         Variable storing the socket ID.
 *  @param[in] pu8_conn_list      Pointer to array containing the connection list.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_add_to_conn_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list);

/** @brief MODBUS get location of a connection
 *
 *  This API is used to obtain the location of a socket ID referenced from the MODBUS TCP connection list.
 * 
 *  @param[in] pu8_soc_id        Pointer variable storing the socket ID.
 *  @param[in] pu8_conn_list     Pointer to array containing the connection list.
 *  @param[out] pu8_soc_loc      Pointer variable storing the socket location referenced in the connection list. 
 *
 *  @retval ERR_OK  If the location of the socket ID obtained 
 *  @retval ERR_SOC_NOT_FOUND if referenced socket not present in the connection array list
 */
int32_t Modbus_tcp_get_loc_from_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list, puint8_t pu8_soc_loc);

/** @brief MODBUS update TCP connection list 
 *
 *  This API is used to update the connection list with the latest connection. The latest connection should be placed at the last 
 *  of the array and the oldest connection is placed initially.
 *
 *  @param[in] u8_soc_id        Variable storing the socket ID.
 *  @param[in] pu8_conn_list    Pointer to array containing the connection list.
 *  @param[in] u8_add_remove    Variable storing the status to determine whether the given socket ID is to be added or removed 
 *                              from the connection list.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_update_conn_list(uint8_t u8_soc_id, puint8_t pu8_conn_list, uint8_t u8_add_remove);

/** @brief MODBUS search a host IP 
 *
 *  This function is used for search an IP from the host IP list given.
 *
 *  @param[in]  pu8_search_IP         IP address to be searched
 *  @param[out] pu8_ip_idx            Index at which IP address is placed
 *
 *  @return ERR_OK on successful search.
 *  @return ERR_IP_NOT_FOUND if IP is not in the list
 *  @return ERR_TABLE_EMPTY if the list is empty
 *  @return ERR_TABLE_DISABLED if the table is disabled, i.e., server accepts connection request from any host
 */
uint32_t Modbus_tcp_search_ip_addr(pchar_t pu8_search_IP, puint8_t pu8_ip_idx);

/** @brief MODBUS TCP send packet
 *
 *  This function sends the prepared packet to lower layer (transport layer). 
 *
 *  @param[in] pu8_mb_snd_pkt		Pointer to the MODBUS send packet array. 
 *  @param[in] u32_snd_pkt_len		Length of the MODBUS send packet
 *  @param[in] u8_soc_id                Socket ID to which the data is to be transmitted
 *
 *  @retval ERR_OK on successfully send the packet
 *  @retval ERR_SEND_FAIL if packet sending failed
 */
uint32_t Modbus_tcp_send(uint8_t u8_soc_id, puint8_t pu8_mb_snd_pkt, uint32_t u32_snd_pkt_len);

/** @brief MODBUS TCP recieve packet
 *
 *  This function received the prepared packet to lower layer (transport layer). 
 *
 *  @param[in] pu8_mb_recv_pkt          Pointer to the MODBUS receive packet array. 
 *  @param[in] u32_recv_pkt_len         Length of the MODBUS receive packet
 *  @param[in] u8_soc_id                Socket ID
 *
 *  @retval packet receive data length
 */
int32_t Modbus_tcp_recv(uint8_t u8_soc_id,puint8_t pu8_mb_recv_pkt, uint32_t u32_recv_pkt_len);

/** @brief MODBUS TCP frame response
 *
 *  This function is used to update TCP packet information structure from response structure provided by the user application.
 *
 *  @param[in]  u8_fn_code                  Variable storing the function code
 *  @param[out] pt_mb_tcp_pkt_info          Structure pointer containing user information
 *
 *  @retval ERR_OK on framing packet is successful
 *  @retval ERR_ILLEGAL_FUNCTION if invalid function code received
 */
uint32_t Modbus_tcp_frame_response(uint8_t u8_fn_code, p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info);

/** @brief MODBUS TCP frame packet
 *
 *  This function frames a response packet for TCP device with the information provided by the user application.
 *
 *  @param[in] pu8_mb_snd_pkt          Pointer to the array storing packet to be send.
 *  @param[out] pu32_snd_pkt_len       Length of the packet framed
 *  @param[in] pt_mb_tcp_pkt_info      Structure pointer containing response information
 *
 *  @retval void_t
 */
void_t Modbus_tcp_frame_pkt(puint8_t pu8_mb_snd_pkt, puint32_t pu32_snd_pkt_len, p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info);

/** @brief MODBUS TCP parse packet
 *
 *  This function parse request packet received by TCP device.
 *
 *  @param[in]  pu8_mb_rcv_pkt          Pointer to the array storing the received packet.
 *  @param[in]  u32_recv_pkt_len        Length of the MODBUS receive packet
 *  @param[out] pt_mb_tcp_pkt_info      Structure pointer containing request information
 *
 *  @retval ERR_OK on successful parsing the packet
 *  @retval ERR_MEM_ALLOC if memory allocation fails
 */
uint32_t Modbus_tcp_parse_pkt(puint8_t pu8_mb_rcv_pkt, uint32_t u32_recv_pkt_len,
        p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info);

/** @brief MODBUS TCP validate packet
 *
 *  This function validates a packet received by the TCP device.
 *
 *  @param[in] pt_mb_tcp_pkt_info     Structure pointer containing request information
 *  @param[in] u32_pdu_len            Length of the PDU received
 *
 *  @retval ERR_OK on successful validation
 *  @retval ERR_ILLEGAL_DATA_VALUE if data value is not in the valid range
 *  @retval ERR_ILLEGAL_FUNCTION if function code is not supported or enabled 
 */
uint32_t Modbus_tcp_validate_pkt(p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info, uint32_t u32_pdu_len);

/** @brief MODBUS post a request to the mailbox
 *
 *  This API is used to send the request received from the client to the 
 *  receive mailbox or gateway mailbox. Increment the number of elements in 
 *  mailbox if the request sent successfully.
 *
 *  @param[in] pt_req_recvd        Pointer to the structure containing request 
 *                                 information.
 *  @param[in] u16_mbx_id          Variable containing the mailbox ID to which 
 *                                 request is to be posted.
 *
 *  @retval ERR_OK  on successful write to mailbox
 *  @retval ERR_MAILBOX  if write to mailbox failed
 */
uint32_t Modbus_post_to_mailbox(uint16_t u16_mbx_id, p_mb_req_mbx_t pt_req_recvd);

/** @brief MODBUS read a request from the mailbox.
 *
 *  This API is used to read the request posted to the receive mailbox or 
 *  gateway mailbox depending upon the mailbox ID specified. Decrement the  
 *  number of elements in mailbox if the request read successfully.
 *
 *  @param[in] u16_mbx_id          Variable containing the mailbox ID from which 
 *                                 request is read.
 *  @param[in] pt_req_recvd        Pointer to the structure containing request 
 *                                 information.
 *
 *  @retval ERR_OK  on successful read from mailbox
 *  @retval ERR_MAILBOX  if read from mailbox failed
 */
uint32_t Modbus_fetch_from_mailbox(uint16_t u16_mbx_id, p_mb_req_mbx_t *pt_req_recvd);

/** @brief MODBUS verify the number of elements in mailbox.
 *
 *  This API is used to verify the number of elements in mailbox.
 *
 *  @param[in] u16_mbx_id       Variable containing the mailbox ID from which 
 *                              request is read. 
 *
 *  @retval ERR_CODE if mailbox is full
 *  @retval number of mailbox used
 */
uint32_t Modbus_check_mailbox(uint16_t u16_mbx_id);

/** @brief MODBUS Delete mailbox
 *
 *  This API is used to delete a mailbox specified by the mailbox ID.
 *
 *  @param[in] u16_mbx_id        Variable storing the mailbox ID.
 *
 *  @retval ERR_OK  on successful deletion of mailbox
 *  @retval ERR_MAILBOX  if deletion of mailbox failed
 */
uint32_t Modbus_delete_mailbox(uint16_t u16_mbx_id);

#endif /* _MODBUSTCP_H_ */
/* End of File */
