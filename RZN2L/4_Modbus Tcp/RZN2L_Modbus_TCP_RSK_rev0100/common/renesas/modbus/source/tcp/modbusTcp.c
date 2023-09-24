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
#include "modbusTypedef.h"
#include "modbusCommon.h"
#include <port/modbusPortTcp.h>
#include <port/modbusPortRtos.h>
#include <port/modbusPortMalloc.h>
#include <tcp/modbusTcp.h>

/*===============================================================================================================================*/
/* D I F I N E M A C O                                                                                                           */
/*===============================================================================================================================*/
#define BUFFER_SIZE 256

/*===============================================================================================================================*/
/* G L O B A L  V A R I A B L E S                                                                                                */
/*===============================================================================================================================*/

/**
 * Used to indicate whether the multiple connections are enabled or not
 */
uint8_t gu8_multiple_client_enable = DISABLE_MULTIPLE_CLIENT_CONNECTION;

/**
 * Used to store the port configured by user if provided
 */
uint32_t gu32_config_port;

/*
 * Used to indicate whether the gateway device act as TCP server
 */
uint8_t gu8_gw_init = MODBUS_TCP_GW_SLAVE_DISABLE;


/*===============================================================================================================================*/
/* S T A T I C  V A R I A B L E S                                                                                                */
/*===============================================================================================================================*/
static p_req_read_coils_t spt_tcp_req_read_coils = NULL;
static p_req_read_inputs_t spt_tcp_req_read_inputs = NULL;
static p_req_read_holding_reg_t spt_tcp_req_read_holding_reg = NULL;
static p_req_read_input_reg_t spt_tcp_req_read_input_reg = NULL;
static p_req_write_single_coil_t spt_tcp_req_write_single_coil = NULL;
static p_req_write_single_reg_t spt_tcp_req_write_single_reg = NULL;
static p_req_write_multiple_coils_t spt_tcp_req_write_multiple_coils = NULL;
static p_req_write_multiple_reg_t spt_tcp_req_write_multiple_reg = NULL;
static p_req_read_write_multiple_reg_t spt_tcp_req_read_write_multiple_reg = NULL;

static p_resp_read_coils_t spt_tcp_resp_read_coils = NULL;
static p_resp_read_inputs_t spt_tcp_resp_read_inputs = NULL;
static p_resp_read_holding_reg_t spt_tcp_resp_read_holding_reg = NULL;
static p_resp_read_input_reg_t spt_tcp_resp_read_input_reg = NULL;
static p_resp_write_single_coil_t spt_tcp_resp_write_single_coil = NULL;
static p_resp_write_single_reg_t spt_tcp_resp_write_single_reg = NULL;
static p_resp_write_multiple_coils_t spt_tcp_resp_write_multiple_coils = NULL;
static p_resp_write_multiple_reg_t spt_tcp_resp_write_multiple_reg = NULL;
static p_resp_read_write_multiple_reg_t spt_tcp_resp_read_write_multiple_reg = NULL;
static p_resp_invalid_function_code_t spt_tcp_resp_invalid_function_code = NULL;

/* 
 * Used to store the stack mode TCP server or TCP gateway mode
 */
uint8_t gu8_stack_mode;

/**
 * Static variable storing the number of elements in receive mailbox 
 */
static uint8_t su8_rcv_mbx_used = 0;

/**
 * Static variable storing the number of elements in gateway mailbox 
 */
static uint8_t su8_gw_mbx_used = 0;

/*===============================================================================================================================*/
/* P R O G R A M                                                                                                                 */
/*===============================================================================================================================*/
/** @brief Modbus TCP server (without gateway)stack initialization
 *
 *  This API is used to initialize the TCP stack. A task is activated which wait for client connection in default MODBUS port and in
 *  additional port (other than default port 502) if provided by the user. Separate task to read the request and process the request
 *  are activated. A mailbox is created to communicate between this tasks.
 *
 *  @param[in] u32_additional_port     Additional port value configured by user
 *  @param[in] u8_tcp_multiple_client  Status whether multiple client is enabled 
 *
 *  @retval ERR_OK on successful initialization of the task or mailbox 
 *  @retval ERR_STACK_INIT if initialization of the task or mailbox failed
 */
uint32_t Modbus_tcp_server_init_stack(uint32_t u32_additional_port, uint8_t u8_tcp_multiple_client)
{
    uint32_t u32_return = ERR_OK;
    gu8_stack_mode = MODBUS_TCP_SERVER_MODE;
	/** Activate the task which wait for socket connection in port 502 */
	do
	{
		if (NULL == pt_slave_map_init)
		{
			u32_return = ERR_STACK_INIT;
			DEBUG_PRINT("\nSlave map initialisation not done\n");
			break;
		}
	    u32_return = Modbus_rtos_create_mailbox(MODBUS_MAILBOX_ID_TCP_RECEIVE);
	    if(ERR_OK != u32_return)
	    {
	    	DEBUG_PRINT("\n Mailbox create function failed\n");
	    	break;
	    }
		gu32_config_port = u32_additional_port;
		u32_return = (uint32_t) Modbus_rtos_create_task (MODBUS_TASK_ID_TCP_ACCEPT);
		if (ERR_OK != u32_return)
		{
			DEBUG_PRINT("\nMODBUS Connection task activation failed\n");
			break;
		}
		/** Activate the task which wait for  a packet in the received socket */
		u32_return = (uint32_t) Modbus_rtos_create_task (MODBUS_TASK_ID_TCP_RECEIVE);
		if (ERR_OK != u32_return)
		{
			DEBUG_PRINT("\nMODBUS Receive data task activation failed\n");
			break;
		}
		/** Activate the task in which the server functioning is carried out */
		u32_return = (uint32_t) Modbus_rtos_create_task (MODBUS_TASK_ID_TCP_SERVICE);
		if (ERR_OK != u32_return)
		{
			DEBUG_PRINT("\nMODBUS Request process task activation failed\n");
			break;
		}
		gu8_multiple_client_enable = u8_tcp_multiple_client;
	}
	while (0);
    if (ERR_OK != u32_return)
    {
        u32_return = ERR_STACK_INIT;
    }
    return u32_return;
}

/** @brief MODBUS terminate TCP Server stack API
 *
 *  This API terminate MODBUS TCP stack related task and the mailbox used for the TCP task. 
 *
 *  @retval ERR_OK on successful termination
 *  @retval ERR_STACK_TERM if termination failed
 */
uint32_t Modbus_tcp_server_terminate_stack(void_t)
{
    uint32_t u32_return = ERR_OK;

    do
    {
        u32_return = (uint32_t) Modbus_rtos_terminate_task (MODBUS_TASK_ID_TCP_ACCEPT);
        if (ERR_OK != u32_return)
        {
            DEBUG_PRINT("\nMODBUS Connection task termination failed\n");
            break;
        }
        u32_return = (uint32_t) Modbus_rtos_terminate_task (MODBUS_TASK_ID_TCP_RECEIVE);
        if (ERR_OK != u32_return)
        {
            DEBUG_PRINT("\nMODBUS Receive data task termination failed\n");
            break;
        }
        u32_return = (uint32_t) Modbus_rtos_terminate_task (MODBUS_TASK_ID_TCP_SERVICE);
        if (ERR_OK != u32_return)
        {
            DEBUG_PRINT("\nMODBUS Request process task termination failed\n");
            break;
        }
        u32_return = Modbus_delete_mailbox (MODBUS_MAILBOX_ID_TCP_RECEIVE);
        if (ERR_OK != u32_return)
        {
            DEBUG_PRINT("\nMODBUS Receive mailbox deletion failed\n");
        }
    }
    while (0);

    if (ERR_OK != u32_return)
    {
        u32_return = ERR_STACK_TERM;
    }

    return u32_return;
}

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
 *  @retval ERR_TCP_SND_MBX_FULL if write to mailbox is full
 */
uint32_t Modbus_post_to_mailbox(uint16_t u16_mbx_id, p_mb_req_mbx_t pt_req_recvd)
{
    uint32_t u32_rslt;

    u32_rslt = Modbus_check_mailbox (u16_mbx_id);

    /** Get queue handler for u16_mbx_id */
    if (u32_rslt != ERR_TCP_SND_MBX_FULL)
    {
		/** Send data to a queue using the FreeRTOS's xQueue Send API */
		u32_rslt = Modbus_rtos_send_packet_on_mailbox(u16_mbx_id,pt_req_recvd);
		if (pdTRUE == u32_rslt)
		{
			if (MODBUS_MAILBOX_ID_TCP_RECEIVE == u16_mbx_id)
			{
				su8_rcv_mbx_used++;
			}
			else
			{
				su8_gw_mbx_used++;
			}
			u32_rslt = ERR_OK;
		}
		else
		{
			u32_rslt = ERR_MAILBOX;
		}
    }
    return u32_rslt;
}
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
uint32_t Modbus_fetch_from_mailbox(uint16_t u16_mbx_id, p_mb_req_mbx_t *pt_req_recvd)
{
    uint32_t u32_e_rslt = pdFALSE;
	u32_e_rslt = Modbus_rtos_receive_packet_on_mailbox(u16_mbx_id, pt_req_recvd);
    if (pdTRUE == u32_e_rslt)
    {
        u32_e_rslt = ERR_OK;
        if (MODBUS_MAILBOX_ID_TCP_RECEIVE == u16_mbx_id)
        {
            su8_rcv_mbx_used--;
        }
        else
        {
            su8_gw_mbx_used--;
        }
    }
    else
    {
        u32_e_rslt = ERR_MAILBOX;
    }

    return u32_e_rslt;
}

/** @brief MODBUS verify the number of elements in mailbox.
 *
 *  This API is used to verify the number of elements in mailbox.
 *
 *  @param[in] u16_mbx_id       Variable containing the mailbox ID from which 
 *                              request is read. 
 *
 *  @return ERR_TCP_SND_MBX_FULL if mailbox is full
 *  @return number of mailbox used
 */
uint32_t Modbus_check_mailbox(uint16_t u16_mbx_id)
{
    if (MODBUS_MAILBOX_ID_TCP_RECEIVE == u16_mbx_id)
    {
        if (MAX_RCV_MBX_SIZE <= su8_rcv_mbx_used)
        {
            return ERR_TCP_SND_MBX_FULL;
        }
        else
        {
            return su8_rcv_mbx_used;
        }
    }
    else
    {
        if (MAX_GW_MBX_SIZE <= su8_gw_mbx_used)
        {
            return ERR_TCP_SND_MBX_FULL;
        }
        else
        {
            return su8_gw_mbx_used;
        }
    }
}

/** @brief MODBUS Delete mailbox
 *
 *  This API is used to delete a mailbox specified by the mailbox ID.
 *
 *  @param[in] u16_mbx_id        Variable storing the mailbox ID.
 *
 *  @retval ERR_OK  on successful deletion of mailbox
 *  @retval ERR_MAILBOX  if deletion of mailbox failed
 */
uint32_t Modbus_delete_mailbox(uint16_t u16_mbx_id)
{
	return Modbus_rtos_delete_mailbox(u16_mbx_id);
}

/** @brief MODBUS shift TCP connection list 
 *
 *  This API is used to shift the connection list according to the latest active connection.
 *
 *  @param[in] pu8_conn_list     Pointer to array containing the connection list.
 *  @param[in] pu8_conn_idx      Index from which the socket ID is to be shifted.
 *
 *  @retval void_t
 */
void_t Modbus_tcp_shift_conn_list(puint8_t pu8_conn_list, puint8_t pu8_conn_idx)
{
    uint8_t u8_idx;
    uint8_t u8_conn_cnt = pu8_conn_list[LIST_CNT_IDX];

    for (u8_idx = *(pu8_conn_idx); u8_idx < u8_conn_cnt; u8_idx++)
    {
        pu8_conn_list[u8_idx] = pu8_conn_list[u8_idx + 1];
    }
}

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
void_t Modbus_tcp_remove_from_conn_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list)
{
    uint32_t u32_return = ERR_OK;
    uint8_t u8_soc_loc;
    uint8_t u8_conn_cnt;

    /** Verify whether the socket ID mentioned is present in the list */
    u32_return = Modbus_tcp_get_loc_from_list (pu8_soc_id, pu8_conn_list, &u8_soc_loc);

    if (ERR_OK == u32_return)
    {
        pu8_conn_list[LIST_CNT_IDX]--;
        Modbus_tcp_shift_conn_list (pu8_conn_list, &u8_soc_loc);
        u8_conn_cnt = pu8_conn_list[LIST_CNT_IDX];
        pu8_conn_list[u8_conn_cnt] = 0;
    }
}

/** @brief MODBUS add a connection to MODBUS TCP connection list 
 *
 * This API is used to add a new connection received by the TCP server to the connection list kept by the server. Append the new 
 *  connection to the array one by one.
 *
 *  @param[in] pu8_soc_id         Variable storing the socket ID.
 *  @param[in] pu8_conn_list      Pointer to array containing the connection list.
 *
 *  @retval void_t
 */
void Modbus_tcp_add_to_conn_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list)
{
    uint32_t u32_return = ERR_OK;
    uint8_t u8_soc_loc;
    uint8_t u8_soc_idx = pu8_conn_list[LIST_CNT_IDX];

    /** Verify whether the socket ID mentioned is present in the list */
    u32_return = Modbus_tcp_get_loc_from_list (pu8_soc_id, pu8_conn_list, &u8_soc_loc);
    if (ERR_OK == u32_return)
    {
        /** If the socket ID is present in latest index no need to shift the list */
        if (u8_soc_loc == (u8_soc_idx - 1))
        {
            /** Do nothing */
        }
        else
        {
            /** Shift the location */
            Modbus_tcp_shift_conn_list (pu8_conn_list, &u8_soc_loc);
            /** Append the socket ID to the connection list */
            pu8_conn_list[u8_soc_idx - 1] = *(pu8_soc_id);
        }
    }
    else
    {
        /** Append the new socket ID to the connection list */
        pu8_conn_list[u8_soc_idx] = *(pu8_soc_id);
        pu8_conn_list[LIST_CNT_IDX]++;
    }
}

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
int32_t Modbus_tcp_get_loc_from_list(puint8_t pu8_soc_id, puint8_t pu8_conn_list, puint8_t pu8_soc_loc)
{
    int32_t s32_return = ERR_OK;
    uint8_t u8_idx;

    /** Get the location of the socket ID specified in the connection list */
    for (u8_idx = 0; u8_idx < LIST_CNT_IDX; u8_idx++)
    {
        if (*(pu8_soc_id) == pu8_conn_list[u8_idx])
        {
            *(pu8_soc_loc) = u8_idx;
            break;
        }
    }
    if (LIST_CNT_IDX == u8_idx)
    {
        s32_return = ERR_TCP_INVALID_SOCKET;
    }

    return s32_return;
}

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
void Modbus_tcp_update_conn_list(uint8_t u8_soc_id, puint8_t pu8_conn_list, uint8_t u8_add_remove)
{
    if (ADD_TO_CONN_LIST == u8_add_remove)
    {
        Modbus_tcp_add_to_conn_list (&u8_soc_id, pu8_conn_list);
    }
    else
    {
        Modbus_tcp_remove_from_conn_list (&u8_soc_id, pu8_conn_list);
    }
}

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
uint32_t Modbus_tcp_search_ip_addr(pchar_t pu8_search_IP, puint8_t pu8_ip_idx)
{
    uint32_t u32_return = ERR_TCP_IP_TABLE_IP_NOT_FOUND;
    uint32_t u32_search_IP;
    uint8_t u8_idx;

    /** Check if the table is enabled */
    if (ENABLE == gt_table_para.e_enable)
    {
        /** Check any IP is present in the global array */
        if (!gt_table_para.u8_cnt)
        {
            u32_return = ERR_TCP_IP_TABLE_EMPTY;
        }
        else
        {
            /** Search for the IP in the host IP table */
            u32_search_IP = Modbus_tcp_inet_addr (pu8_search_IP);
            for (u8_idx = 0; u8_idx < gt_table_para.u8_cnt; u8_idx++)
            {
                if (u32_search_IP == gu32_hostIP_table[u8_idx])
                {
                    /** Save the index */
                    *(pu8_ip_idx) = u8_idx;
                    u32_return = ERR_OK;
                    break;
                }
            }
        }
    }
    else
    {
        u32_return = ERR_TCP_IP_TABLE_DISABLED;
    }

    return u32_return;
}

/** @brief MODBUS set host IP list properties
 *
 *  This function is used for specifying mode(accept/reject) and to Enable or Disable the list of IP address by the user. 
 *  By default the host IP list is disabled.
 *
 *  @param e_flag          Status is whether the connection table enabled or disabled
 *  @param e_mode          Status indicating the list contains IP to be accepted or rejected
 *
 *  @return void_t
 */
void Modbus_tcp_init_ip_table(ENABLE_FLAG e_flag, TABLE_MODE e_mode)
{
    gt_table_para.e_enable = e_flag;
    gt_table_para.e_mode = e_mode;
    gt_table_para.u8_cnt = 0;
}

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
uint32_t Modbus_tcp_add_ip_addr(pchar_t pu8_add_ip)
{
    uint32_t u32_return = ERR_OK;
    uint32_t u32_host_ip;
    uint8_t u8_ip_idx;

    u32_return = Modbus_tcp_search_ip_addr (pu8_add_ip, &u8_ip_idx);

    if (ERR_OK == u32_return)
    {
        u32_return = ERR_TCP_IP_TABLE_IP_ALREADY_PRESENT;
    }
    else if ((ERR_TCP_IP_TABLE_IP_NOT_FOUND == u32_return) || (ERR_TCP_IP_TABLE_EMPTY == u32_return))
    {
        /** Check IP is present in the list */
        if (MAXIMUM_NUMBER_OF_CLIENTS > gt_table_para.u8_cnt)
        {
            u32_host_ip = Modbus_tcp_inet_addr (pu8_add_ip);
            gu32_hostIP_table[gt_table_para.u8_cnt++] = u32_host_ip;
            u32_return = ERR_OK;
        }
        else
        {
            /** If maximum client count reached */
            u32_return = ERR_TCP_IP_TABLE_MAX_CLIENT;
        }
    }

    return u32_return;
}

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
uint32_t Modbus_tcp_delete_ip_addr(pchar_t pu8_del_ip)
{
    uint32_t u32_return = ERR_OK;
    uint8_t u8_ip_idx;
    uint8_t u8_idx;

    /** Search IP present in the host IP connection list */
    u32_return = Modbus_tcp_search_ip_addr (pu8_del_ip, &u8_ip_idx);

    if (ERR_OK == u32_return)
    {
        for (u8_idx = u8_ip_idx; u8_idx < gt_table_para.u8_cnt; u8_idx++)
        {
            gu32_hostIP_table[u8_idx] = gu32_hostIP_table[u8_idx + 1];
        }
        gt_table_para.u8_cnt -= 1;
    }

    return u32_return;
}

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
uint32_t Modbus_tcp_send(uint8_t u8_soc_id, puint8_t pu8_mb_snd_pkt, uint32_t u32_snd_pkt_len)
{
    uint32_t u32_return = ERR_OK;

    u32_return = Modbus_tcp_send_packet (u8_soc_id, pu8_mb_snd_pkt, u32_snd_pkt_len);

    return u32_return;
}

/** @brief MODBUS TCP receive packet
 *
 *  This function receive the prepared packet to lower layer (transport layer). 
 *
 *  @param[in] pu8_mb_recv_pkt          Pointer to the MODBUS receive packet array. 
 *  @param[in] u32_recv_pkt_len         Length of the MODBUS receive packet
 *  @param[in] u8_soc_id                Socket ID
 *
 *  @retval packet receive data length
 */
int32_t Modbus_tcp_recv(uint8_t u8_soc_id,puint8_t pu8_mb_recv_pkt, uint32_t u32_recv_pkt_len)
{
    int32_t rcv_pkt_len = 0;

    rcv_pkt_len = Modbus_tcp_receive_packet (u8_soc_id, pu8_mb_recv_pkt, u32_recv_pkt_len);

    return rcv_pkt_len;
}

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
uint32_t Modbus_tcp_frame_response(uint8_t u8_fn_code, p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info)
{
    uint32_t u32_status = ERR_OK;
    uint32_t u32_idx1 = 0;
    uint32_t u32_idx2 = 0;

    switch (u8_fn_code)
    {
        case FUNC_CODE_READ_COIL:
        {
            if (0 != spt_tcp_resp_read_coils->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_READ_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_read_coils->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_READ_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_coils->u8_num_of_bytes;

                for (u32_idx2 = 0; u32_idx2 < spt_tcp_resp_read_coils->u8_num_of_bytes; u32_idx2++)
                {
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_coils->aru8_data[u32_idx2];
                }
                /** Save the PDU length = function code (1 byte) + byte count (1 byte) + coil status */
                pt_mb_tcp_pkt_info->u16_num_of_bytes = RESP_PDU_SIZE_CONST + (spt_tcp_resp_read_coils->u8_num_of_bytes);
            }
            if (NULL == spt_tcp_resp_read_coils)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_read_coils);
            }
        }
        break;

        case FUNC_CODE_READ_INPUT:
        {
            if (0 != spt_tcp_resp_read_inputs->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_READ_INPUT;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_read_inputs->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_READ_INPUT;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_inputs->u8_num_of_bytes;

                for (u32_idx2 = 0; u32_idx2 < spt_tcp_resp_read_inputs->u8_num_of_bytes; u32_idx2++)
                {
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_inputs->aru8_data[u32_idx2];
                }
                /** Save the PDU length = function code (1 byte) + byte count (1 byte) + input status */
                pt_mb_tcp_pkt_info->u16_num_of_bytes = RESP_PDU_SIZE_CONST
                        + (spt_tcp_resp_read_inputs->u8_num_of_bytes);
            }
            if (NULL == spt_tcp_resp_read_inputs)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_read_inputs);
            }
        }
        break;

        case FUNC_CODE_READ_HOLD_REG:
        {
            if (0 != spt_tcp_resp_read_holding_reg->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_READ_HOLD_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_read_holding_reg->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_READ_HOLD_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_holding_reg->u8_num_of_bytes;

                for (u32_idx2 = 0; u32_idx2 < (spt_tcp_resp_read_holding_reg->u8_num_of_bytes / 2); u32_idx2++)
                {
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                            spt_tcp_resp_read_holding_reg->aru16_data[u32_idx2]);
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                            spt_tcp_resp_read_holding_reg->aru16_data[u32_idx2]);
                }
                /** Save the PDU length = function code (1 byte) + byte count (1 byte) + 2 byte register value */
                pt_mb_tcp_pkt_info->u16_num_of_bytes = RESP_PDU_SIZE_CONST
                        + spt_tcp_resp_read_holding_reg->u8_num_of_bytes;
            }
            if (NULL == spt_tcp_resp_read_holding_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_read_holding_reg);
            }
        }
        break;

        case FUNC_CODE_READ_INPUT_REG:
        {
            if (0 != spt_tcp_resp_read_input_reg->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_READ_INPUT_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_read_input_reg->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_READ_INPUT_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = spt_tcp_resp_read_input_reg->u8_num_of_bytes;

                for (u32_idx2 = 0; u32_idx2 < (spt_tcp_resp_read_input_reg->u8_num_of_bytes / 2); u32_idx2++)
                {
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                            spt_tcp_resp_read_input_reg->aru16_data[u32_idx2]);
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                            spt_tcp_resp_read_input_reg->aru16_data[u32_idx2]);
                }
                /** Save the PDU length = function code (1 byte) + byte count (1 byte) + register value */
                pt_mb_tcp_pkt_info->u16_num_of_bytes = RESP_PDU_SIZE_CONST
                        + spt_tcp_resp_read_input_reg->u8_num_of_bytes;
            }
            if (NULL == spt_tcp_resp_read_input_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_read_input_reg);
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_COIL:
        {
            if (0 != spt_tcp_resp_write_single_coil->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_WRITE_SINGLE_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_write_single_coil->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_WRITE_SINGLE_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_single_coil->u16_output_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                        spt_tcp_resp_write_single_coil->u16_output_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_single_coil->u16_output_value);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = GET_LSB(
                        spt_tcp_resp_write_single_coil->u16_output_value);
                pt_mb_tcp_pkt_info->u16_num_of_bytes = FUNC_CODE_WRITE_RESP_PDU_LEN; /** Save the PDU length */
            }
            if (NULL == spt_tcp_resp_write_single_coil)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_write_single_coil);
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_REG:
        {
            if (0 != spt_tcp_resp_write_single_reg->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_WRITE_SINGLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_write_single_reg->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_WRITE_SINGLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_single_reg->u16_register_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                        spt_tcp_resp_write_single_reg->u16_register_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_single_reg->u16_register_value);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = GET_LSB(
                        spt_tcp_resp_write_single_reg->u16_register_value);
                pt_mb_tcp_pkt_info->u16_num_of_bytes = FUNC_CODE_WRITE_RESP_PDU_LEN;
            }
            if (NULL == spt_tcp_resp_write_single_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_write_single_reg);
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_COIL:
        {
            if (0 != spt_tcp_resp_write_multiple_coils->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_WRITE_MULTIPLE_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_write_multiple_coils->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_WRITE_MULTIPLE_COIL;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_multiple_coils->u16_start_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                        spt_tcp_resp_write_multiple_coils->u16_start_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_multiple_coils->u16_num_of_outputs);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = GET_LSB(
                        spt_tcp_resp_write_multiple_coils->u16_num_of_outputs);
                pt_mb_tcp_pkt_info->u16_num_of_bytes = FUNC_CODE_WRITE_RESP_PDU_LEN;
            }
            if (NULL == spt_tcp_resp_write_multiple_coils)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_write_multiple_coils);
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_REG:
        {
            if (0 != spt_tcp_resp_write_multiple_reg->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_WRITE_MULTIPLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_write_multiple_reg->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_WRITE_MULTIPLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_multiple_reg->u16_start_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                        spt_tcp_resp_write_multiple_reg->u16_start_addr);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                        spt_tcp_resp_write_multiple_reg->u16_num_of_reg);
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = GET_LSB(
                        spt_tcp_resp_write_multiple_reg->u16_num_of_reg);
                pt_mb_tcp_pkt_info->u16_num_of_bytes = FUNC_CODE_WRITE_RESP_PDU_LEN;
            }
            if (NULL == spt_tcp_resp_write_multiple_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_write_multiple_reg);
            }
        }
        break;

        case FUNC_CODE_READ_WRITE_MULTIPLE_REG:
        {
            if (0 != spt_tcp_resp_read_write_multiple_reg->u8_exception_code)
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = EXP_CODE_READ_WRITE_MULTIPLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] =
                        spt_tcp_resp_read_write_multiple_reg->u8_exception_code;
                pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            }
            else
            {
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = FUNC_CODE_READ_WRITE_MULTIPLE_REG;
                pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] =
                        spt_tcp_resp_read_write_multiple_reg->u8_num_of_bytes;

                for (u32_idx2 = 0; u32_idx2 < (spt_tcp_resp_read_write_multiple_reg->u8_num_of_bytes / 2); u32_idx2++)
                {
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_MSB(
                            spt_tcp_resp_read_write_multiple_reg->aru16_read_data[u32_idx2]);
                    pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = GET_LSB(
                            spt_tcp_resp_read_write_multiple_reg->aru16_read_data[u32_idx2]);
                }
                /** Save the PDU length = function code (1 byte) + byte count (1 byte) + register value */
                pt_mb_tcp_pkt_info->u16_num_of_bytes = RESP_PDU_SIZE_CONST
                        + spt_tcp_resp_read_write_multiple_reg->u8_num_of_bytes;
            }
            if (NULL == spt_tcp_resp_read_write_multiple_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_read_write_multiple_reg);
            }
        }
        break;

        default:
        {
            pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++] = (MODBUS_EXCEPTION_CODE | u8_fn_code);
            pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = spt_tcp_resp_invalid_function_code->u8_exception_code;
            pt_mb_tcp_pkt_info->u16_num_of_bytes = EXCEPTION_PDU_LEN;
            if (NULL == spt_tcp_resp_invalid_function_code)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_resp_invalid_function_code);
            }
        }
    }

    return u32_status;
}

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
void Modbus_tcp_frame_pkt(puint8_t pu8_mb_snd_pkt, puint32_t pu32_snd_pkt_len, p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info)
{
    uint32_t u32_snd_pkt_len = 0;
    uint32_t u32_idx1 = 0;
    uint32_t u32_idx2 = 0;

    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_MSB(pt_mb_tcp_pkt_info->u16_transaction_id);
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_LSB(pt_mb_tcp_pkt_info->u16_transaction_id);
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_MSB(pt_mb_tcp_pkt_info->u16_protocol_id);
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_LSB(pt_mb_tcp_pkt_info->u16_protocol_id);

    /** Save the PDU + slave ID length */
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_MSB(pt_mb_tcp_pkt_info->u16_num_of_bytes + 1);
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = GET_LSB(pt_mb_tcp_pkt_info->u16_num_of_bytes + 1);
    pu8_mb_snd_pkt[u32_snd_pkt_len++] = pt_mb_tcp_pkt_info->u8_slave_id;

    for (u32_idx1 = 0, u32_idx2 = u32_snd_pkt_len; u32_idx1 < (pt_mb_tcp_pkt_info->u16_num_of_bytes); u32_idx2++)
    {
        pu8_mb_snd_pkt[u32_idx2] = pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1++];
    }

    *pu32_snd_pkt_len = u32_idx2;
}

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
        p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info)
{
    uint32_t u32_return = ERR_OK;
    uint32_t u32_idx1;
    uint32_t u32_idx2;
    uint32_t u32_pdu_len;
    uint8_t u8_fn_code;

    pt_mb_tcp_pkt_info->u16_transaction_id = UINT8_TO_UINT16(pu8_mb_rcv_pkt, TCP_TRANSACTION_IDX);
    pt_mb_tcp_pkt_info->u16_protocol_id = UINT8_TO_UINT16(pu8_mb_rcv_pkt, TCP_PROTOCOL_IDX);
    pt_mb_tcp_pkt_info->u16_num_of_bytes = UINT8_TO_UINT16(pu8_mb_rcv_pkt, RECV_PKT_LEN_IDX);
    pt_mb_tcp_pkt_info->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];

    u32_pdu_len = (u32_recv_pkt_len - MODBUS_TCP_HEADER_LEN);

    for (u32_idx1 = 0, u32_idx2 = FUNCTION_CODE_IDX; u32_idx1 < MAX_DATA_SIZE; u32_idx1++)
    {
        pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1] = pu8_mb_rcv_pkt[u32_idx2++];
    }

    u32_return = Modbus_tcp_validate_pkt (pt_mb_tcp_pkt_info, u32_pdu_len);

    u8_fn_code = pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_FUNC_CODE_IDX];

    switch (u8_fn_code)
    {
        case FUNC_CODE_READ_COIL:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_read_coils = Modbus_malloc (sizeof(req_read_coils_t));
            if (NULL == spt_tcp_req_read_coils)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_read_coils, 0, sizeof(req_read_coils_t));
            }
            spt_tcp_resp_read_coils = Modbus_malloc (sizeof(resp_read_coils_t));
            if (NULL == spt_tcp_resp_read_coils)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_read_coils, 0, sizeof(resp_read_coils_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_read_coils->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_read_coils->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_read_coils->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_read_coils->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                         TCP_PDU_READ_START_ADDR_IDX);
                spt_tcp_req_read_coils->u16_num_of_coils = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                           TCP_PDU_READ_CNT_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code1 (spt_tcp_req_read_coils, spt_tcp_resp_read_coils);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_read_coils->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_read_coils)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_read_coils);
            }
        }
        break;

        case FUNC_CODE_READ_INPUT:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_read_inputs = Modbus_malloc (sizeof(req_read_inputs_t));
            if (NULL == spt_tcp_req_read_inputs)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_read_inputs, 0, sizeof(req_read_inputs_t));
            }
            spt_tcp_resp_read_inputs = Modbus_malloc (sizeof(resp_read_inputs_t));
            if (NULL == spt_tcp_resp_read_inputs)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_read_inputs, 0, sizeof(resp_read_inputs_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_read_inputs->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_read_inputs->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_read_inputs->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_read_inputs->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                          TCP_PDU_READ_START_ADDR_IDX);
                spt_tcp_req_read_inputs->u16_num_of_inputs = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                             TCP_PDU_READ_CNT_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code2 (spt_tcp_req_read_inputs, spt_tcp_resp_read_inputs);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_read_inputs->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_read_inputs)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_read_inputs);
            }
        }
        break;

        case FUNC_CODE_READ_HOLD_REG:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_read_holding_reg = Modbus_malloc (sizeof(req_read_holding_reg_t));
            if (NULL == spt_tcp_req_read_holding_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_read_holding_reg, 0, sizeof(req_read_holding_reg_t));
            }
            spt_tcp_resp_read_holding_reg = Modbus_malloc (sizeof(resp_read_holding_reg_t));
            if (NULL == spt_tcp_resp_read_holding_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_read_holding_reg, 0, sizeof(resp_read_holding_reg_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_read_holding_reg->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_read_holding_reg->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_read_holding_reg->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_read_holding_reg->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                               TCP_PDU_READ_START_ADDR_IDX);
                spt_tcp_req_read_holding_reg->u16_num_of_reg = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                               TCP_PDU_READ_CNT_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code3 (spt_tcp_req_read_holding_reg, spt_tcp_resp_read_holding_reg);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_read_holding_reg->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_read_holding_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_read_holding_reg);
            }
        }
        break;

        case FUNC_CODE_READ_INPUT_REG:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_read_input_reg = Modbus_malloc (sizeof(req_read_input_reg_t));
            if (NULL == spt_tcp_req_read_input_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_read_input_reg, 0, sizeof(req_read_input_reg_t));
            }
            spt_tcp_resp_read_input_reg = Modbus_malloc (sizeof(resp_read_input_reg_t));
            if (NULL == spt_tcp_resp_read_input_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_read_input_reg, 0, sizeof(resp_read_input_reg_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_read_input_reg->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_read_input_reg->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_read_input_reg->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_read_input_reg->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                             TCP_PDU_READ_START_ADDR_IDX);
                spt_tcp_req_read_input_reg->u16_num_of_reg = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                             TCP_PDU_READ_CNT_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code4 (spt_tcp_req_read_input_reg, spt_tcp_resp_read_input_reg);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_read_input_reg->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_read_input_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_read_input_reg);
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_COIL:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_write_single_coil = Modbus_malloc (sizeof(req_write_single_coil_t));
            if (NULL == spt_tcp_req_write_single_coil)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_write_single_coil, 0, sizeof(req_write_single_coil_t));
            }
            spt_tcp_resp_write_single_coil = Modbus_malloc (sizeof(resp_write_single_coil_t));
            if (NULL == spt_tcp_resp_write_single_coil)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_write_single_coil, 0, sizeof(resp_write_single_coil_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_write_single_coil->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_write_single_coil->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_write_single_coil->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_write_single_coil->u16_output_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                 TCP_PDU_WRITE_ADDR_IDX);
                spt_tcp_req_write_single_coil->u16_output_value = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                  TCP_PDU_WRITE_VALUE_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code5 (spt_tcp_req_write_single_coil, spt_tcp_resp_write_single_coil);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_write_single_coil->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_write_single_coil)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_write_single_coil);
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_REG:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_write_single_reg = Modbus_malloc (sizeof(req_write_single_reg_t));
            if (NULL == spt_tcp_req_write_single_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_write_single_reg, 0, sizeof(req_write_single_reg_t));
            }
            spt_tcp_resp_write_single_reg = Modbus_malloc (sizeof(resp_write_single_reg_t));
            if (NULL == spt_tcp_resp_write_single_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_write_single_reg, 0, sizeof(resp_write_single_reg_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_write_single_reg->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_write_single_reg->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_write_single_reg->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_write_single_reg->u16_register_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                  TCP_PDU_WRITE_ADDR_IDX);
                spt_tcp_req_write_single_reg->u16_register_value = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                   TCP_PDU_WRITE_VALUE_IDX);
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code6 (spt_tcp_req_write_single_reg, spt_tcp_resp_write_single_reg);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_write_single_reg->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_write_single_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_write_single_reg);
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_COIL:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_write_multiple_coils = Modbus_malloc (sizeof(req_write_multiple_coils_t));
            if (NULL == spt_tcp_req_write_multiple_coils)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_write_multiple_coils, 0, sizeof(req_write_multiple_coils_t));
            }
            spt_tcp_resp_write_multiple_coils = Modbus_malloc (sizeof(resp_write_multiple_coils_t));
            if (NULL == spt_tcp_resp_write_multiple_coils)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_write_multiple_coils, 0, sizeof(resp_write_multiple_coils_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_write_multiple_coils->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_write_multiple_coils->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_write_multiple_coils->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_write_multiple_coils->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                   TCP_PDU_MULTIPLE_WRITE_ADDR_IDX);
                spt_tcp_req_write_multiple_coils->u16_num_of_outputs = UINT8_TO_UINT16(
                        pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_MULTIPLE_WRITE_CNT_IDX);
                spt_tcp_req_write_multiple_coils->u8_num_of_bytes =
                        pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX];
                for (u32_idx1 = TCP_PDU_MULTIPLE_WRITE_VALUE_IDX, u32_idx2 = 0;
                        u32_idx2 < spt_tcp_req_write_multiple_coils->u8_num_of_bytes; u32_idx1++, u32_idx2++)
                {
                    spt_tcp_req_write_multiple_coils->aru8_data[u32_idx2] =
                            pt_mb_tcp_pkt_info->aru8_data_packet[u32_idx1];
                }
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code15 (spt_tcp_req_write_multiple_coils,
                                                       spt_tcp_resp_write_multiple_coils);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_write_multiple_coils->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_write_multiple_coils)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_write_multiple_coils);
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_REG:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_write_multiple_reg = Modbus_malloc (sizeof(req_write_multiple_reg_t));
            if (NULL == spt_tcp_req_write_multiple_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_write_multiple_reg, 0, sizeof(req_write_multiple_reg_t));
            }
            spt_tcp_resp_write_multiple_reg = Modbus_malloc (sizeof(resp_write_multiple_reg_t));
            if (NULL == spt_tcp_resp_write_multiple_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_write_multiple_reg, 0, sizeof(resp_write_multiple_reg_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_write_multiple_reg->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_write_multiple_reg->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_write_multiple_reg->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_write_multiple_reg->u16_start_addr = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                 TCP_PDU_MULTIPLE_WRITE_ADDR_IDX);
                spt_tcp_req_write_multiple_reg->u16_num_of_reg = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet,
                                                                                 TCP_PDU_MULTIPLE_WRITE_CNT_IDX);
                spt_tcp_req_write_multiple_reg->u8_num_of_bytes =
                        pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX];
                for (u32_idx1 = TCP_PDU_MULTIPLE_WRITE_VALUE_IDX, u32_idx2 = 0;
                        u32_idx2 < (spt_tcp_req_write_multiple_reg->u8_num_of_bytes / 2); u32_idx2++, u32_idx1 +=
                        MB_REGISTER_SIZE)
                {
                    spt_tcp_req_write_multiple_reg->aru16_data[u32_idx2] = UINT8_TO_UINT16(
                            pt_mb_tcp_pkt_info->aru8_data_packet, u32_idx1);
                }
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code16 (spt_tcp_req_write_multiple_reg, spt_tcp_resp_write_multiple_reg);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_write_multiple_reg->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_write_multiple_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_write_multiple_reg);
            }
        }
        break;

        case FUNC_CODE_READ_WRITE_MULTIPLE_REG:
        {
            /** Allocate memory for both request and response structure */
            spt_tcp_req_read_write_multiple_reg = Modbus_malloc (sizeof(req_read_write_multiple_reg_t));
            if (NULL == spt_tcp_req_read_write_multiple_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_req_read_write_multiple_reg, 0, sizeof(req_read_write_multiple_reg_t));
            }
            spt_tcp_resp_read_write_multiple_reg = Modbus_malloc (sizeof(resp_read_write_multiple_reg_t));
            if (NULL == spt_tcp_resp_read_write_multiple_reg)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_read_write_multiple_reg, 0, sizeof(resp_read_write_multiple_reg_t));
            }
            if (ERR_OK == u32_return)
            {
                spt_tcp_req_read_write_multiple_reg->u16_transaction_id = pt_mb_tcp_pkt_info->u16_transaction_id;
                spt_tcp_req_read_write_multiple_reg->u16_protocol_id = pt_mb_tcp_pkt_info->u16_protocol_id;
                spt_tcp_req_read_write_multiple_reg->u8_slave_id = pu8_mb_rcv_pkt[TCP_SLAVE_ID_IDX];
                spt_tcp_req_read_write_multiple_reg->u16_read_start_addr = UINT8_TO_UINT16(
                        pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_START_ADDR_IDX);
                spt_tcp_req_read_write_multiple_reg->u16_num_to_read = UINT8_TO_UINT16(
                        pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                spt_tcp_req_read_write_multiple_reg->u16_write_start_addr = UINT8_TO_UINT16(
                        pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_WRITE_ADDR_IDX);
                spt_tcp_req_read_write_multiple_reg->u16_num_to_write = UINT8_TO_UINT16(
                        pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_WRITE_VALUE_IDX);
                spt_tcp_req_read_write_multiple_reg->u8_write_num_of_bytes =
                        pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_READ_WRITE_BYTE_CNT_IDX];
                for (u32_idx1 = TCP_PDU_READ_WRITE_REG_VALUE_IDX, u32_idx2 = 0;
                        u32_idx2 < (spt_tcp_req_read_write_multiple_reg->u8_write_num_of_bytes / 2);
                        u32_idx2++, u32_idx1 += MB_REGISTER_SIZE)
                {
                    spt_tcp_req_read_write_multiple_reg->aru16_data[u32_idx2] = UINT8_TO_UINT16(
                            pt_mb_tcp_pkt_info->aru8_data_packet, u32_idx1);
                }
                /** Invoke the user function to process the request */
                pt_slave_map_init->fp_function_code23 (spt_tcp_req_read_write_multiple_reg,
                                                       spt_tcp_resp_read_write_multiple_reg);
            }
            else if ((ERR_ILLEGAL_FUNCTION == u32_return) || (ERR_ILLEGAL_DATA_VALUE == u32_return))
            {
                spt_tcp_resp_read_write_multiple_reg->u8_exception_code = u32_return;
                u32_return = ERR_OK;
            }
            else
            {
                /** Discard the packet */
            }
            if (NULL == spt_tcp_req_read_write_multiple_reg)
            {
                DEBUG_PRINT("\nMemory already freed\n");
            }
            else
            {
                Modbus_free (spt_tcp_req_read_write_multiple_reg);
            }
        }
        break;

        default:
        {
            spt_tcp_resp_invalid_function_code = Modbus_malloc (sizeof(resp_invalid_function_code_t));
            if (NULL == spt_tcp_resp_invalid_function_code)
            {
                u32_return = ERR_MEM_ALLOC;
                DEBUG_PRINT("\nMemory allocation failed\n");
            }
            else
            {
                memset (spt_tcp_resp_invalid_function_code, 0, sizeof(resp_invalid_function_code_t));
                spt_tcp_resp_invalid_function_code->u8_exception_code = ERR_ILLEGAL_FUNCTION;
                u32_return = ERR_OK;
            }
        }
        break;
    }

    return u32_return;
}

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
uint32_t Modbus_tcp_validate_pkt(p_mb_tcp_pkt_info_t pt_mb_tcp_pkt_info, uint32_t u32_pdu_len)
{
    uint32_t u32_return = ERR_OK;
    uint16_t u16_cnt;
    uint8_t u8_byte_cnt;
    uint8_t u8_temp;

    switch (pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_FUNC_CODE_IDX])
    {
        case FUNC_CODE_READ_COIL:
        {
            if (READ_COIL_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                if (NULL == pt_slave_map_init->fp_function_code1)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_READ_DISCRETE_CNT > u16_cnt) || (MAX_READ_DISCRETE_CNT < u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_READ_INPUT:
        {
            if (READ_DISCRETE_INPUT_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                if (NULL == pt_slave_map_init->fp_function_code2)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_READ_DISCRETE_CNT > u16_cnt) || (MAX_READ_DISCRETE_CNT < u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_READ_HOLD_REG:
        {
            if (READ_HOLDING_REG_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                if (NULL == pt_slave_map_init->fp_function_code3)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_READ_REG_CNT > u16_cnt) || (MAX_READ_REG_CNT < u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_READ_INPUT_REG:
        {
            if (READ_INPUT_REG_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                if (NULL == pt_slave_map_init->fp_function_code4)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_READ_REG_CNT > u16_cnt) || (MAX_READ_REG_CNT < u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_COIL:
        {
            if (WRITE_SINGLE_COIL_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_WRITE_VALUE_IDX);
                if (NULL == pt_slave_map_init->fp_function_code5)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_WRITE_COIL_VALUE != u16_cnt) && (MAX_WRITE_COIL_VALUE != u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_WRITE_SINGLE_REG:
        {
            if (WRITE_SINGLE_REG_PDU_SIZE_CONST != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                if (NULL == pt_slave_map_init->fp_function_code6)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_COIL:
        {
            u8_byte_cnt = pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX];

            if ((WRITE_MULTIPLE_COIL_PDU_SIZE_CONST + u8_byte_cnt) != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_MULTIPLE_WRITE_CNT_IDX);

                if (0 != (u16_cnt % 8))
                {
                    u8_temp = (u16_cnt / 8) + 1;
                }
                else
                {
                    u8_temp = (u16_cnt / 8);
                }

                if (NULL == pt_slave_map_init->fp_function_code15)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_MULTIPLE_DISCRETE_WRITE_CNT > u16_cnt) || (MAX_MULTIPLE_DISCRETE_WRITE_CNT < u16_cnt)
                        || (u8_byte_cnt != u8_temp))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_WRITE_MULTIPLE_REG:
        {
            u8_byte_cnt = pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_MULTIPLE_WRITE_BYTE_CNT_IDX];

            if ((WRITE_MULTIPLE_REG_PDU_SIZE_CONST + u8_byte_cnt) != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_MULTIPLE_WRITE_CNT_IDX);

                if (NULL == pt_slave_map_init->fp_function_code16)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_MULTIPLE_REG_WRITE_CNT > u16_cnt) || (MAX_MULTIPLE_REG_WRITE_CNT < u16_cnt)
                        || (u8_byte_cnt != (2 * u16_cnt)))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
            }
        }
        break;

        case FUNC_CODE_READ_WRITE_MULTIPLE_REG:
        {
            u8_byte_cnt = pt_mb_tcp_pkt_info->aru8_data_packet[TCP_PDU_READ_WRITE_BYTE_CNT_IDX];

            if ((READ_WRITE_MULTIPLE_REG_PDU_SIZE_CONST + u8_byte_cnt) != u32_pdu_len)
            {
                u32_return = ERR_ILLEGAL_DATA_VALUE;
            }
            else
            {
                u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_CNT_IDX);
                if (NULL == pt_slave_map_init->fp_function_code23)
                {
                    u32_return = ERR_ILLEGAL_FUNCTION;
                }
                else if ((MIN_READ_REG_CNT > u16_cnt) || (MAX_READ_REG_CNT < u16_cnt))
                {
                    u32_return = ERR_ILLEGAL_DATA_VALUE;
                }
                else
                {
                    u16_cnt = UINT8_TO_UINT16(pt_mb_tcp_pkt_info->aru8_data_packet, TCP_PDU_READ_WRITE_VALUE_IDX);

                    if ((MIN_MULTIPLE_READ_WRITE_CNT > u16_cnt) || (MAX_MULTIPLE_READ_WRITE_CNT < u16_cnt)
                            || (u8_byte_cnt != (2 * u16_cnt)))
                    {
                        u32_return = ERR_ILLEGAL_DATA_VALUE;
                    }
                }
            }
        }
        break;

        default:
            u32_return = ERR_ILLEGAL_FUNCTION;
    }

    return u32_return;
}
/* End of File */
