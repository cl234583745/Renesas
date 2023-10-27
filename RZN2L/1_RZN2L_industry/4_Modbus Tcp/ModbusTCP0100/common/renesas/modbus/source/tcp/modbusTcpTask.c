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
#include <modbusCommon.h>
#include <port/modbusPortTcp.h>
#include <port/modbusPortMalloc.h>
#include <port/modbusPortRtos.h>
#include <tcp/modbusTcp.h>
#include <tcp/modbusTcpConfig.h>
#include <tcp/modbusTcpSlave.h>
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/*===============================================================================================================================*/
/* G L O B A L   V A R I A B L E S                                                                                               */
/*===============================================================================================================================*/
/** 
 * Structure variable for storing IP list parameters
 */
table_param_t gt_table_para =
{ ENABLE, ACCEPT, 0 };

/** 
 * Array to store host IP List
 */
uint32_t gu32_hostIP_table[MAXIMUM_NUMBER_OF_CLIENTS];

/**
 * Array containing the list of connected socket ID
 */
uint8_t gu8_conn_list[MAXIMUM_NUMBER_OF_CLIENTS + 1] =
{ 0 };

/*===============================================================================================================================*/
/* S T A T I C  V A R I A B L E S                                                                                                */
/*===============================================================================================================================*/
/**
 * Array used to store the received request packet 
 */
static uint8_t su8_tcp_recv_pkt[MAX_TCP_PKT_SIZE];

/**
 * Variable used to store the length of the packet received 
 */
static int32_t ss32_tcp_rcv_pkt_len = 0;

/**
 * Array used to store the response packet to be send 
 */
static uint8_t su8_tcp_snd_pkt[MAX_TCP_PKT_SIZE];

/**
 * Variable used to store the length of the packet to be send 
 */
static uint32_t su32_tcp_snd_pkt_len = 0;

/**
 * Structure variable storing the TCP packet information
 */
static mb_tcp_pkt_info_t st_tcp_info;

/*===============================================================================================================================*/
/* P R O G R A M                                                                                                                 */
/*===============================================================================================================================*/
/** @brief Wait for connection task
 *
 *  This task waits for a connection from a client in the default MODBUS port(502) and an additional port if configured by user.
 *  Verify whether the IP table is enabled or not. If enabled, verify the list contains the IP list to accepted or rejected. 
 *  Accordingly save the socket descriptor to the connection list.
 *
 *  @retval void
 */
void_t Modbus_tcp_soc_wait_task(void_t)
{
    struct sockaddr_in client_addr;
    int32_t s32_listen_fds_config = 0;
    int32_t s32_listen_fds_dflt = 0;
    int32_t s32_return = ERR_OK;
    int32_t s32_conn_fd = 0;
    uint32_t u32_max_conn_read_fd = 0;
    socklen_t u32_client_len = 0;
    uint8_t u8_idx = 0;
    uint32_t u32_select_timeout_ms = 10;

    /** Create a socket listening in default port 502 and a configured port(if provided by user) */
    s32_return = (int32_t) Modbus_tcp_init_socket (TCP_PORT_NUMBER, (pint32_t) &s32_listen_fds_dflt);
    if (ERR_OK == s32_return)
    {
        if (0 != gu32_config_port)
        {
            s32_return = (int32_t) Modbus_tcp_init_socket ((uint16_t) gu32_config_port, (pint32_t) &s32_listen_fds_config);
            if (ERR_OK != s32_return)
            {
                Modbus_rtos_exit_task ();
            }
        }
    }
    else
    {
        Modbus_rtos_exit_task ();
    }

    u32_client_len = sizeof(client_addr);

    while (1)
    {
        u32_max_conn_read_fd = s32_listen_fds_dflt;
        if (s32_listen_fds_config)
        {
            if (s32_listen_fds_config > u32_max_conn_read_fd)
                u32_max_conn_read_fd = s32_listen_fds_config;
        }

        s32_return = Modbus_tcp_listen_select (u32_max_conn_read_fd, u32_select_timeout_ms);
        if (0 < s32_return)
        {
            if (MODBUS_TCP_SELECT_READ == Modbus_tcp_check_listen_socket_acceptable(s32_listen_fds_dflt))
            {
                /** Accept awaiting request */
                s32_conn_fd = Modbus_tcp_accept_socket (s32_listen_fds_dflt, (struct sockaddr*) &client_addr, &u32_client_len);
            }
            else if (MODBUS_TCP_SELECT_READ == Modbus_tcp_check_listen_socket_acceptable(s32_listen_fds_config))
            {
                /** Accept awaiting request */
                s32_conn_fd = Modbus_tcp_accept_socket (s32_listen_fds_config, (struct sockaddr*) &client_addr, &u32_client_len);
            }
            else
            {
                s32_conn_fd = 0;
            }
            if (((DISABLE_MULTIPLE_CLIENT_CONNECTION == gu8_multiple_client_enable)
                    && (0 == gu8_conn_list[LIST_CNT_IDX]))
                    || (ENABLE_MULTIPLE_CLIENT_CONNECTION == gu8_multiple_client_enable))
            {
                /** If connection fd is valid proceed */
                if (0 <= s32_conn_fd)
                {
                    /** Verify whether IP table lookup is enabled */
                    if (ENABLE == gt_table_para.e_enable)
                    {
                        if (ACCEPT == gt_table_para.e_mode)
                        {
                            /** Accept only requests from hostIP's given in table */
                            for (u8_idx = 0; u8_idx < gt_table_para.u8_cnt; u8_idx++)
                            {
                                if (client_addr.sin_addr.s_addr == gu32_hostIP_table[u8_idx])
                                {
                                    /** Update connection list with the new connection */
                                    if ((MAXIMUM_NUMBER_OF_CLIENTS) > gu8_conn_list[LIST_CNT_IDX])
                                    {
                                        Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                                    }
                                    else
                                    {
                                        /** Close the oldest unused socket and add the new connection */
                                        Modbus_tcp_close_connected_socket (gu8_conn_list[OLDEST_CONN_IDX]);
                                        Modbus_tcp_update_conn_list (gu8_conn_list[OLDEST_CONN_IDX], gu8_conn_list,
                                                                     REMOVE_FROM_CONN_LIST);
                                        Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                                    }
                                    break;
                                }
                            }
                            if (gt_table_para.u8_cnt == u8_idx)
                            {
                                DEBUG_PRINT("Connection rejected\n");
                                Modbus_tcp_close_connected_socket (s32_conn_fd);
                            }
                        }
                        else
                        {
                            /** Reject connection request from hostIP's given in table */
                            for (u8_idx = 0; u8_idx < gt_table_para.u8_cnt; u8_idx++)
                            {
                                if (client_addr.sin_addr.s_addr == gu32_hostIP_table[u8_idx])
                                {
                                    DEBUG_PRINT("Connection rejected (host is not allowed to connect)\n");
                                    Modbus_tcp_close_connected_socket (s32_conn_fd);
                                    break;
                                }
                            }
                            if (gt_table_para.u8_cnt == u8_idx)
                            {
                                /** Update connection list with the new connection */
                                if ((MAXIMUM_NUMBER_OF_CLIENTS) > gu8_conn_list[LIST_CNT_IDX])
                                {
                                    Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                                }
                                else
                                {
                                    /** Close the oldest unused socket and add the new connection */
                                    Modbus_tcp_close_connected_socket (gu8_conn_list[OLDEST_CONN_IDX]);
                                    Modbus_tcp_update_conn_list (gu8_conn_list[OLDEST_CONN_IDX], gu8_conn_list,
                                                                 REMOVE_FROM_CONN_LIST);
                                    Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                                }
                            }
                        }
                    }
                    else
                    {
                        /** Table can be enabled or disabled only at the beginning of execution. Once disabled, user must not change it until
                         *   the next execution
                         */
                        if ((MAXIMUM_NUMBER_OF_CLIENTS) > gu8_conn_list[LIST_CNT_IDX])
                        {
                            Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                        }
                        else
                        {
                            /** Close the oldest unused socket and add the new connection */
                            Modbus_tcp_close_connected_socket (gu8_conn_list[OLDEST_CONN_IDX]);
                            Modbus_tcp_update_conn_list (gu8_conn_list[OLDEST_CONN_IDX], gu8_conn_list,
                                                         REMOVE_FROM_CONN_LIST);
                            Modbus_tcp_update_conn_list (s32_conn_fd, gu8_conn_list, ADD_TO_CONN_LIST);
                        }
                    }
                }
                else
                {
                    Modbus_rtos_timeout_sleep_task (10);
                }
            }
            else
            {
                Modbus_tcp_close_connected_socket (s32_conn_fd);
            }
        }
    }

}

/** @brief Receive data Task
 *
 *  This task waits for a request received in the selected socket ID. It verify the packet is for MODBUS protocol. If so, write the 
 *  request to the receive mailbox. If the mailbox is found full, send a error server busy to the client.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_recv_data_task(void_t)
{
    static uint8_t su8_invalid_pkt = 0;
    uint32_t u32_max_read_fd = 0;
    uint32_t u32_soc_id = 0;
    uint32_t u32_conn_idx;
    uint32_t u32_idx;
    uint16_t u16_protocol_id;
    uint16_t u16_recv_pdu_len = 0;
    int32_t s32_remain_pkt_len = 0;
    int32_t s32_fixed_pkt_len;
    mb_req_mbx_t *pt_mb_req_pkt = NULL;
    uint32_t u32_timeout_ms = 20;
    puint8_t pu8_req_pkt;
    uint32_t u32_check_code;

    while (1)
    {
        u32_max_read_fd = 0;

        if (0 != gu8_conn_list[LIST_CNT_IDX])
        {
            for (u32_idx = 0; u32_idx < gu8_conn_list[LIST_CNT_IDX]; u32_idx++)
            {
                if (gu8_conn_list[u32_idx] > u32_max_read_fd)
                {
                    u32_max_read_fd = gu8_conn_list[u32_idx];
                }
            }

            u32_check_code = Modbus_tcp_receive_select (u32_max_read_fd, u32_timeout_ms);
            if (0 < u32_check_code)
            {
                /** Run through the existing connections looking for data to be read */
                for (u32_conn_idx = 1; u32_conn_idx <= u32_max_read_fd; u32_conn_idx++)
                {
                    /** Check if the socket is disconnected. */
                    u32_check_code = Modbus_tcp_check_connected_socket_receptible(u32_conn_idx);
                    if ( MODBUS_TCP_SELECT_EXCEPTION == u32_check_code)
                    {
                        /** Close the socket */
                        Modbus_tcp_close_connected_socket (u32_conn_idx);
                        Modbus_tcp_update_conn_list (u32_conn_idx, gu8_conn_list, REMOVE_FROM_CONN_LIST);
                        continue;
                	}

                    if ( MODBUS_TCP_SELECT_READ != u32_check_code)
                    {
                        continue;
                    }

                    /** Save the socket ID where data received */
                    u32_soc_id = u32_conn_idx;

                    memset (su8_tcp_recv_pkt, 0, MAX_TCP_PKT_SIZE);
                    ss32_tcp_rcv_pkt_len = Modbus_tcp_recv (u32_soc_id, su8_tcp_recv_pkt, MAX_TCP_PKT_SIZE);
                    if (0 >= ss32_tcp_rcv_pkt_len)
                    {
                        /** Connection lose, close the socket */
                        Modbus_tcp_close_connected_socket (u32_soc_id);
                        Modbus_tcp_update_conn_list (u32_soc_id, gu8_conn_list, REMOVE_FROM_CONN_LIST);
                        continue;
                    }

                    if (INVALID_PKT_RECEPTION != su8_invalid_pkt)
                    {
                        u16_recv_pdu_len = UINT8_TO_UINT16(su8_tcp_recv_pkt, RECV_PKT_LEN_IDX) - 1;
                        /* Check if packet greater than MAX_TCP_PKT_SIZE is sent from master */
                        if (u16_recv_pdu_len > (MAX_TCP_PKT_SIZE - MODBUS_TCP_HEADER_LEN))
                        {
                            su8_invalid_pkt = INVALID_PKT_RECEPTION;
                            /* Save the remaining length to be received */
                            s32_remain_pkt_len = u16_recv_pdu_len - (ss32_tcp_rcv_pkt_len - MODBUS_TCP_HEADER_LEN);
                        }
                        else if (ss32_tcp_rcv_pkt_len < (u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN))
                        {
                            ss32_tcp_rcv_pkt_len = 0;
                        }
                    }
                    else
                    {
                        s32_remain_pkt_len = s32_remain_pkt_len - ss32_tcp_rcv_pkt_len;
                        ss32_tcp_rcv_pkt_len = 0;
                        if (s32_remain_pkt_len <= 0)
                        {
                            su8_invalid_pkt = 0;
                        }
                    }

                    /** Verify packet is for MODBUS protocol */
                    if (MODBUS_TCP_HEADER_LEN >= ss32_tcp_rcv_pkt_len)
                    {
                        /** Discard the packet */
                        continue;
                    }
                    u16_protocol_id = UINT8_TO_UINT16(su8_tcp_recv_pkt, TCP_PROTOCOL_IDX);
                    if (MODBUS_PROTOCOL != u16_protocol_id)
                    {
                        /** Discard the packet */
                        continue;
                    }

                    /* Separate the recieve data to the PDU unit */
                    s32_fixed_pkt_len = 0;
                    while ((ss32_tcp_rcv_pkt_len - s32_fixed_pkt_len) > MODBUS_TCP_HEADER_LEN)
                    {
                        pt_mb_req_pkt = (mb_req_mbx_t*) Modbus_malloc (sizeof(mb_req_mbx_t));
                        pu8_req_pkt = Modbus_malloc (u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN);

                        if ((NULL == pt_mb_req_pkt) || (NULL == pu8_req_pkt))
                        {
                            DEBUG_PRINT("\nMemory allocation failed\n");
                        }
                        else
                        {
                            memset (pu8_req_pkt, 0, u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN);
                            for (u32_idx = 0; u32_idx < u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN; u32_idx++)
                            {
                                pu8_req_pkt[u32_idx] = su8_tcp_recv_pkt[s32_fixed_pkt_len + u32_idx];
                            }
                            pt_mb_req_pkt->pu8_req_pkt = pu8_req_pkt;
                            pt_mb_req_pkt->u32_pkt_len = u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN;
                            pt_mb_req_pkt->u32_soc_id = u32_soc_id;

                            /** Update the connection list with the latest transaction */
                            Modbus_tcp_update_conn_list (pt_mb_req_pkt->u32_soc_id, gu8_conn_list, ADD_TO_CONN_LIST);

                            if (ERR_TCP_SND_MBX_FULL == Modbus_post_to_mailbox (MODBUS_MAILBOX_ID_TCP_RECEIVE, pt_mb_req_pkt))
                            {
                                /** Server busy should be returned */
                                for (u32_idx = 0; u32_idx < 4; u32_idx++)
                                {
                                    su8_tcp_snd_pkt[u32_idx] = pt_mb_req_pkt->pu8_req_pkt[u32_idx];
                                }
                                su8_tcp_snd_pkt[u32_idx++] = 0;
                                su8_tcp_snd_pkt[u32_idx++] = EXCEPTION_PDU_LEN + 1; /** PDU length + slave ID (1 byte) */
                                su8_tcp_snd_pkt[u32_idx++] = pt_mb_req_pkt->pu8_req_pkt[TCP_SLAVE_ID_IDX];
                                su8_tcp_snd_pkt[u32_idx++] = (MODBUS_EXCEPTION_CODE
                                        | pt_mb_req_pkt->pu8_req_pkt[FUNCTION_CODE_IDX]);
                                su8_tcp_snd_pkt[u32_idx++] = ERR_ILLEGAL_SERV_BSY;
                                if (ERR_OK != Modbus_tcp_send (u32_soc_id,su8_tcp_snd_pkt, u32_idx))
                                {
                                    /* ignore error */
                                }
                                if (NULL != pt_mb_req_pkt->pu8_req_pkt)
                                {
                                    Modbus_free (pt_mb_req_pkt->pu8_req_pkt);
                                }
                                if (NULL != pt_mb_req_pkt)
                                {
                                    Modbus_free (pt_mb_req_pkt);
                                }
                            }
                        }
                        s32_fixed_pkt_len += (u16_recv_pdu_len + MODBUS_TCP_HEADER_LEN);
                        if (0 < s32_remain_pkt_len)
                        {
                            /* if invalid packet reception, recieve remain data. */
                            break;
                        }
                        /* Next PDU unit */
                        u16_recv_pdu_len = UINT8_TO_UINT16(su8_tcp_recv_pkt, s32_fixed_pkt_len + RECV_PKT_LEN_IDX) - 1;
                    }
                    /** Provide to the task for processing the request */
                    Modbus_rtos_sleep_task ();
                }
            }
        }
        else
        {
            Modbus_rtos_timeout_sleep_task (100);
        }
    }
}

/** @brief Request processing task
 *
 *  This task wait for a request in the queue. Verify the slave ID in the request packet to determine the packet is for the TCP 
 *  server or the device connected to it serially. If the packet is for the TCP server process the request read from the queue, 
 *  prepare the response packet and send it to the TCP client. If the packet is for the serial device connected, write the request 
 *  to the gateway mailbox.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_req_process_task(void_t)
{
    static uint8_t su8_pkt_state = 0;
    uint32_t u32_return;
    mb_req_mbx_t *pt_mb_req;

    while (1)
    {
        if (ERR_OK == Modbus_fetch_from_mailbox (MODBUS_MAILBOX_ID_TCP_RECEIVE, &pt_mb_req))
        {
            su8_pkt_state = 0;
            if (MODBUS_TCP_SLAVE_ID == pt_mb_req->pu8_req_pkt[TCP_SLAVE_ID_IDX])
            {
                if (((MODBUS_TCP_GW_SLAVE_ENABLE == gu8_gw_init) && (MODBUS_TCP_GATEWAY_MODE == gu8_stack_mode))
                        || (MODBUS_TCP_SERVER_MODE == gu8_stack_mode))
                {
                    /** request to tcp server */
                    su8_pkt_state = MODBUS_TCP_PKT;
                }
            }
            else if (0 == pt_mb_req->pu8_req_pkt[TCP_SLAVE_ID_IDX])
            {
                if (MODBUS_TCP_GATEWAY_MODE == gu8_stack_mode)
                {
                    /** request to gateway devices */
                    su8_pkt_state = MODBUS_GW_PKT;
                }
                else
                {
                    /** request to tcp server */
                    su8_pkt_state = MODBUS_TCP_PKT;
                }
            }
            else
            {
                if (MODBUS_TCP_GATEWAY_MODE == gu8_stack_mode)
                {
                    /** request to gateway devices */
                    su8_pkt_state = MODBUS_GW_PKT;
                }
                else
                {
                    /** discard the packet */
                }
            }
            if (MODBUS_GW_PKT == su8_pkt_state)
            {
                /** discard the packet (gateway mode is disabled currently) */
            }
            else if (MODBUS_TCP_PKT == su8_pkt_state)
            {
                /** Parse and validate the frame received */
                u32_return = Modbus_tcp_parse_pkt (pt_mb_req->pu8_req_pkt, pt_mb_req->u32_pkt_len, &st_tcp_info);

                /** Clear the array element in the structure to store the the response packet */
                memset (st_tcp_info.aru8_data_packet, 0, MAX_DATA_SIZE);

                /** If the validation is unsuccessful, save the exception code */
                if (ERR_OK != u32_return)
                {
                    /* Memory allocation error */
                }
                else
                {
                    u32_return = Modbus_tcp_frame_response (pt_mb_req->pu8_req_pkt[FUNCTION_CODE_IDX], &st_tcp_info);
                }

                if (ERR_OK == u32_return)
                {
                    Modbus_tcp_frame_pkt (su8_tcp_snd_pkt, &su32_tcp_snd_pkt_len, &st_tcp_info);

                    if (ERR_OK != Modbus_tcp_send (pt_mb_req->u32_soc_id, su8_tcp_snd_pkt, su32_tcp_snd_pkt_len))
                    {
                        /** If packet sending fails, close the socket and update the connection list */
                        Modbus_tcp_update_conn_list (pt_mb_req->u32_soc_id, gu8_conn_list, REMOVE_FROM_CONN_LIST);
                    }
                    else
                    {
                        if (NULL != pt_mb_req->pu8_req_pkt)
                        {
                            Modbus_free (pt_mb_req->pu8_req_pkt);
                        }
                        if (NULL != pt_mb_req)
                        {
                            Modbus_free (pt_mb_req);
                        }
                    }
                }
            }
            else
            {
                /** discard the packet */
            }
        }
        /* return to receive data task */
        Modbus_rtos_wakeup_task (MODBUS_TASK_ID_TCP_RECEIVE);

    }
}

/* End of File */
