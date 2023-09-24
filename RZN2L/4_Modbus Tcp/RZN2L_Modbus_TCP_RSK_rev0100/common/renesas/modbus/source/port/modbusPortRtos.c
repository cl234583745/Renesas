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
#include <port/modbusPortRtos.h>
#include <tcp/modbusTcp.h>
#include "FreeRTOS.h"
#include "queue.h"

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
#define TASK_TABLE_END           (-1)       /*!< Task table end ID                      */
#define MAILBOX_TABLE_END        (-1)       /*!< Initialize table end mark              */

/*===============================================================================================================================*/
/* T Y P E D E F S                                                                                                               */
/*===============================================================================================================================*/

//-------------------------------------
// Task creation information packet
//-------------------------------------
typedef struct _task_table_param
{
    void *                                    pv_parameter;                /*!< Task extended information              */
    void *                                    pv_task_address;             /*!< Task start address                     */
    UBaseType_t                                t_initial_priority;          /*!< Task initial priority                  */
    configSTACK_DEPTH_TYPE                     t_stack_size;                /*!< Task stack size                        */
    TaskHandle_t                               pt_task_handle;              /*!< Task handler                           */
    uint8_t                                    u8_task_name[20];            /*!< Task name                              */
} task_table_param_t;


//-------------------------------------
// Static task table packet
//-------------------------------------
typedef struct _task_table
{
    int32_t                                    s32_task_id;                 /*!< Task ID                                */
    task_table_param_t                         t_task_table;                /*!< Task creation information packet       */
} task_table_t;

//-------------------------------------
// Mailbox creation infomation packet
//-------------------------------------
typedef struct _mailbox_table_param {
    QueueHandle_t                              pt_queue_handle;             /*!< Mailbox creation handle                                             */
    UBaseType_t                                t_queue_length;              /*!< The maximum number of items the mailbox can hold at any one time.   */
} mailbox_table_param_t;


//-------------------------------------
// Static mailbox table packet
//-------------------------------------
typedef struct _mailbox_table
{
    int32_t                                    s32_queue_id;                /*!< Mailbox ID                             */
    mailbox_table_param_t                      t_mailbox_param;             /*!< Mailbox creation infomation packet     */
}mailbox_table_t;

/*============================================================================*/
/* E X T E R N                                                                */
/*============================================================================*/

/*============================================================================*/
/* S T A T I C  V A R I A B L E S                                             */
/*============================================================================*/
static task_table_t static_task_table[] = {
//CREATE_TASK	{s32task_id,                    {pv_parameter,          pv_task_address,                        t_initial_priority,             t_stack_size,           taskhandle,     taskname}};
            	{MODBUS_TASK_ID_TCP_ACCEPT,     {0,                     (void *)Modbus_tcp_soc_wait_task,       configMAX_PRIORITIES -2,        0x400,                  NULL,	        "MODBUS_CON_SOC"}},
                {MODBUS_TASK_ID_TCP_RECEIVE,	{0,                     (void *)Modbus_tcp_recv_data_task,      configMAX_PRIORITIES -2,        0x400,                  NULL,          "MODBUS_RECV_SOC"}},
                {MODBUS_TASK_ID_TCP_SERVICE,	{0,                     (void *)Modbus_tcp_req_process_task,    configMAX_PRIORITIES -2,        0x400,                  NULL,          "MODBUS_SERV_TASK"}},
                {TASK_TABLE_END,                {0,                     (void *)NULL,                           0,                              0,                      NULL,          "MODBUS_TBL_END"}}
};

static mailbox_table_t static_mailbox_table[] =
{
// CREATE_MAILBOX       {s32_queue_id,                          {queue_handle, 	        queue_length}};
                        {MODBUS_MAILBOX_ID_TCP_RECEIVE,         {NULL,                  MAX_RCV_MBX_SIZE}},
                        {MAILBOX_TABLE_END,                     {NULL,                  0}}
};

/*============================================================================*/
/* S T A T I C   F U N C T I O N   P R O T O T Y P E                          */
/*============================================================================*/
static int32_t Modbus_rtos_check_task_table_index(int32_t s32_task_id);
static int32_t Modbus_rtos_check_mailbox_table_index(uint16_t u16_mailbox_id);
static void * Modbus_rtos_get_mailbox_handle_from_table(uint16_t u16_mailbox_id);
/*============================================================================*/
/* P R O G R A M                                                              */
/*============================================================================*/
/** @brief This function create the task
 *
 *  @param[in]          s32_task_id   			Task id number to be started
 *  @param              pv_stacd   				Task launch code
 *
 *  @retval             ERR_OK    				Success
 *  @retval             ERR_RTOS_INVALID_ID    	Illegal ID number
 */
int32_t Modbus_rtos_create_task(int32_t s32_task_id)
{
    /** The result of xTaskCreate()  */
    BaseType_t t_return;

    /** Arguments of xTaskCreate() */
    TaskHandle_t t_task_handle = NULL;

    /** static_task_table array subscript */
    int32_t s32_index = 0;

    /** Find the static_task_table ID that matches s32_task_id */
    s32_index = Modbus_rtos_check_task_table_index(s32_task_id);

    /** Check if the task_id is static_task_table.id matched. */
    if (TASK_TABLE_END != static_task_table[s32_index].s32_task_id)
    {
        t_return = xTaskCreate ((TaskFunction_t )static_task_table[s32_index].t_task_table.pv_task_address,
                                 (char*) static_task_table[s32_index].t_task_table.u8_task_name,
                                 static_task_table[s32_index].t_task_table.t_stack_size,
                                 static_task_table[s32_index].t_task_table.pv_parameter,
                                 static_task_table[s32_index].t_task_table.t_initial_priority,
                                 &t_task_handle);

        /** Add task handler to static_task_table */
        static_task_table[s32_index].t_task_table.pt_task_handle = t_task_handle;

        /** Check if the task is created. */
        if (t_return == pdPASS)
        {
            t_return = ERR_OK;
        }
        else
        {
        	t_return = ERR_RTOS_ABORTED;
        }
    }
    else
    {
        t_return = ERR_RTOS_INVALID_ID;
    }
    /** Returns whether the task has been generated. */
    return (int32_t) t_return;
}

/** @brief Terminate task
 *
 *  @param[in]          s32_task_id   			Task id number to be terminated
 *
 *  @retval             ERR_OK    				Deleted task
 *  @retval             ERR_RTOS_INVALID_ID    	Illegal ID number
 */
int32_t Modbus_rtos_terminate_task(int32_t s32_task_id)
{
    /** static_task_table array subscript */
    int32_t s32_index = 0;

    BaseType_t t_return;

    /** Find the static_task_table ID that matches task_id */
    s32_index = Modbus_rtos_check_task_table_index(s32_task_id);
    /** Check if the task_id is static_task_table.id matched. */
    if (TASK_TABLE_END != static_task_table[s32_index].s32_task_id)
    {
    	/** Delete the task static_task_table that matches task_id */
        vTaskDelete (static_task_table[s32_index].t_task_table.pt_task_handle);
        t_return = ERR_OK;
    }
    else
    {
        t_return = ERR_RTOS_INVALID_ID;
    }

    /** Returns whether the task has been deleted. */
    return t_return;
}

/** @brief Terminate task
 *
 *  This function terminate task.
 *  Passing NULL will cause the calling task to be deleted.
 *
 */
void Modbus_rtos_exit_task(void)
{
    vTaskDelete (NULL);
}

/** @brief Sleep task with time out
 *
 *  @param[in]          s32_timeout    			Time out time
 *
 *  @retval             ERR_OK     				Success
 *  @retval             ERR_RTOS_TIMEOUT  		Polling failure or timeout
 */
int32_t Modbus_rtos_timeout_sleep_task(int32_t s32_timeout)
{
    uint32_t u32_return;
    TickType_t t_ticks_to_wait;

    /** Convert timeout time to tick time. */
    t_ticks_to_wait = (TickType_t) (s32_timeout * configTICK_RATE_HZ / 1000);

    u32_return = ulTaskNotifyTake(pdTRUE, t_ticks_to_wait);
    if (u32_return != 0)
    {
        return ERR_OK;
    }
    return ERR_RTOS_TIMEOUT;
}

/** @brief Sleep task without timeout
 *
 *  @param              None
 *
 *  @retval             ERR_OK        			Success
 */
int32_t Modbus_rtos_sleep_task(void)
{
    uint32_t u32_return;
    u32_return = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (u32_return != 0)
    {
        /** This section can not be reached */
    }
    return ERR_OK;
}

/** @brief Wake up task
 *
 *  @param              s32_task_id       		Task ID number to wake up
 *
 *  @retval             ERR_OK        			Success
 *  @retval             ERR_RTOS_INVALID_ID     Illegal ID number
 */
int32_t Modbus_rtos_wakeup_task(int32_t s32_task_id)
{
    /** static_task_table array subscript */
    int32_t s32_index = 0;

    for (s32_index = 0; TASK_TABLE_END != static_task_table[s32_index].s32_task_id; s32_index++)
    {
        if (s32_task_id == static_task_table[s32_index].s32_task_id)
        {
            /** Wake up the task specified by task_handle */
            xTaskNotifyGive(static_task_table[s32_index].t_task_table.pt_task_handle);
            return ERR_OK;
        }
    }
    /** Failed to find the specified task ID in task table. */
    return ERR_RTOS_INVALID_ID;
}

/** @brief Check task table index matches the task id.
 *
 *  @param              s32_task_id       		Task ID number
 *
 *  @retval             s_32_index     			If s32_task_id matches static_task_tabl.id, the subscript of static_task_table is returned.
 *  											However, if there is no match, the TASK_TBL_END subscript is returned.
 *
 */
static int32_t Modbus_rtos_check_task_table_index(int32_t s32_task_id)
{
	int32_t s32_index;

	for (s32_index = 0; TASK_TABLE_END != static_task_table[s32_index].s32_task_id; s32_index++)
	{
		if (s32_task_id == static_task_table[s32_index].s32_task_id)
		{
			break;
		}
	}
	return s32_index;
}

/** @brief Create a queue for the mailbox table and add the queue handler to the mailbox table.
 *
 *  @param              u16_mailbox_id      	Mailbox ID number
 *
 *  @return 			ERR_OK					Create queue handler for the mailbox.
 *  @return				ERR_MAILBOX
 *  @return				ERR_RTOS_INVALID_ID		Illegal mailbox ID number
 *
 */
uint32_t Modbus_rtos_create_mailbox(uint16_t u16_mailbox_id)
{
	int32_t s32_index;
	s32_index = Modbus_rtos_check_mailbox_table_index(u16_mailbox_id);
	/** Generate queue handler for mailbox */
	if (MAILBOX_TABLE_END != static_mailbox_table[s32_index].s32_queue_id)
	{
		static_mailbox_table[s32_index].t_mailbox_param.pt_queue_handle = xQueueCreate(static_mailbox_table[s32_index].t_mailbox_param.t_queue_length, sizeof(p_mb_req_mbx_t));
		if (NULL == static_mailbox_table[s32_index].t_mailbox_param.pt_queue_handle)
		{
			return ERR_MAILBOX;
		}
	}
    else
    {
    	 /** Failed to find the specified mailbox ID in mailbox table. */
    	return ERR_RTOS_INVALID_ID;
    }
	return ERR_OK;
}

/** @brief Check mailbox table index matches the mailbox id.
 *
 *  @param              u16_mailbox_id      	Mailbox ID number
 *
 *  @retval             s_32_index      		If u16_mbx_id matches static_mailbox_tabl.id, the subscript of static_mailbox_table is returned.
 *  											However, if there is no match, the MAILBOX_TBL_END subscript is returned.
 *
 */
static int32_t Modbus_rtos_check_mailbox_table_index(uint16_t u16_mailbox_id)
{
	int32_t s32_index;
	for (s32_index = 0; MAILBOX_TABLE_END != static_mailbox_table[s32_index].s32_queue_id; s32_index++)
	{
		if (u16_mailbox_id == static_mailbox_table[s32_index].s32_queue_id)
		{
			break;
		}
	}
	return s32_index;
}

/** @brief Get queue handler of mailbox table.
 *
 *  @param              u16_mailbox_id      	Mailbox ID number
 *
 *  @retval             t_queue_handle  		If u16_mbx_id matches static_mailbox_tabl.id, the queue handler of static_mailbox_table is returned.
 *  											However, if there is no match, the queue handler is NULL.
 *
 */
static void * Modbus_rtos_get_mailbox_handle_from_table(uint16_t u16_mailbox_id)
{
	int32_t s32_index;
	QueueHandle_t t_queue_handle = NULL;

	s32_index = Modbus_rtos_check_mailbox_table_index(u16_mailbox_id);
	if (u16_mailbox_id == static_mailbox_table[s32_index].s32_queue_id)
	{
		t_queue_handle = static_mailbox_table[s32_index].t_mailbox_param.pt_queue_handle;
	}
	return t_queue_handle;
}

/** @brief Send packet to the mailbox.
 *
 *  @param[in]          u16_mailbox_id      	Mailbox ID number
 *  @param[in] 			pt_req_received    		Pointer to the structure containing request information.
 *
 *  @retval[out]        u32_return    			Returns pdTRUE if the data was successfully sent.
 *  											Returns err code if the data was failed sent.
 *
 */
uint32_t Modbus_rtos_send_packet_on_mailbox(uint16_t u16_mailbox_id, void * pt_request_received)
{
	uint32_t u32_return;
	QueueHandle_t  t_queue_handle;
	t_queue_handle = Modbus_rtos_get_mailbox_handle_from_table(u16_mailbox_id);
	if(t_queue_handle != NULL)
	{
		u32_return = (uint32_t)xQueueSend(t_queue_handle, (void* ) &pt_request_received, (TickType_t )0);
	}
	else
	{
		u32_return = ERR_RTOS_INVALID_ID;
	}
	return u32_return;
}

/** @brief receive data from the mailbox.
 *
 *  @param[in]          u16_mailbox_id      	Mailbox ID number
 *  @param[in] 			pt_request_received    	Pointer to the structure containing request information.
 *
 *  @retval[out]        u32_return    			Returns pdTRUE if the data was successfully receive.
 *  											Returns err code if the data was failed receive.
 *
 */
uint32_t Modbus_rtos_receive_packet_on_mailbox(uint16_t u16_mailbox_id, void * pt_request_received)
{
	uint32_t u32_return;
	QueueHandle_t t_queue_handle;

	t_queue_handle = Modbus_rtos_get_mailbox_handle_from_table(u16_mailbox_id);
	if(t_queue_handle != NULL)
	{
    /** Get to data from a queue using FreeRTOS's xQueueReceive API */
		u32_return = (uint32_t)xQueueReceive (t_queue_handle, pt_request_received, portMAX_DELAY);
	}
	else
	{
		u32_return = ERR_RTOS_INVALID_ID;
	}
    return u32_return;
}

/** @brief Delete mailbox.
 *
 *  @param[in]          u16_mailbox_id      	Mailbox ID number
 *
 *  @retval[out]        u32_return    			Returns ERR_OK if delete the mailbox successfully.
 *  											Returns err code if delete the mailbox failed.
 *
 */
uint32_t Modbus_rtos_delete_mailbox(uint16_t u16_mailbox_id)
{
	int32_t s32_index;
	uint32_t u32_return = ERR_OK;

	s32_index = Modbus_rtos_check_mailbox_table_index(u16_mailbox_id);
	if (static_mailbox_table[s32_index].s32_queue_id == u16_mailbox_id)
	{
		vQueueDelete (static_mailbox_table[s32_index].t_mailbox_param.pt_queue_handle);
	}
	else
	{
		u32_return = ERR_RTOS_INVALID_ID;
	}
	return u32_return;
}
