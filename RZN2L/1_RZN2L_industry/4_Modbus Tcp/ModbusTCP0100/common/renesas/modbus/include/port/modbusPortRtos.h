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

#ifndef _MODBUSPORTRTOS_H_
#define _MODBUSPORTRTOS_H_

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
#define MODBUS_TASK_ID_TCP_ACCEPT        (51)       /*!< Wait for connection task ID            */
#define MODBUS_TASK_ID_TCP_RECEIVE       (52)       /*!< Receive data task ID                   */
#define MODBUS_TASK_ID_TCP_SERVICE       (53)       /*!< Send data task ID                      */

#define MODBUS_MAILBOX_ID_TCP_RECEIVE     (8)        /*!< Modbus TCP communication mailbox ID    */

/*===============================================================================================================================*/
/* T Y P E D E F S                                                                                                               */
/*===============================================================================================================================*/
typedef void            (*cb_func)(void);    /*!< Processing unit start address (pointer to a function) */

/*===============================================================================================================================*/
/* P R O T O T Y P E S                                                                                                           */
/*===============================================================================================================================*/

/** @brief This function create the task
 *
 *  @param[in]          s32_task_id             Task id number to be started
 *  @param              pv_stacd                Task launch code
 *
 *  @retval             ERR_OK                  Success
 *  @retval             ERR_RTOS_INVALID_ID     Illegal ID number
 */
int32_t Modbus_rtos_create_task(int32_t s32_task_id);

/** @brief Terminate task
 *
 *  @param[in]          s32_task_id             Task id number to be terminated
 *
 *  @retval             ERR_OK                  Deleted task
 *  @retval             ERR_RTOS_INVALID_ID     Illegal ID number
 */
int32_t Modbus_rtos_terminate_task(int32_t s32_task_id);

/** @brief Terminate task
 *
 *  This function terminate task.
 *  Passing NULL will cause the calling task to be deleted.
 *
 */
void Modbus_rtos_exit_task(void);

/** @brief Sleep task with time out
 *
 *  @param[in]          s32_timeout             Time out time
 *
 *  @retval             ERR_OK                  Success
 *  @retval             ERR_RTOS_TIMEOUT        Polling failure or timeout
 */
int32_t Modbus_rtos_timeout_sleep_task(int32_t s32_timeout);

/** @brief Sleep task without timeout
 *
 *  @param              None
 *
 *  @retval             ERR_OK                  Success
 */
int32_t Modbus_rtos_sleep_task(void);

/** @brief Wake up task
 *
 *  @param              s32_task_id             Task ID number to wake up
 *
 *  @retval             ERR_OK                  Success
 *  @retval             ERR_RTOS_INVALID_ID     Illegal ID number
 */
int32_t Modbus_rtos_wakeup_task(int32_t s32_task_id);

/** @brief Create a queue for the mailbox table and add the queue handler to the mailbox table.
 *
 *  @param              u16_mailbox_id          Mailbox ID number
 *
 *  @return             ERR_OK                  Create queue handler for the mailbox.
 *  @return             ERR_MAILBOX
 *  @return             ERR_RTOS_INVALID_ID     Illegal mailbox ID number
 *
 */
uint32_t Modbus_rtos_create_mailbox(uint16_t u16_mailbox_id);

/** @brief Send packet to the mailbox.
 *
 *  @param[in]          u16_mailbox_id          Mailbox ID number
 *  @param[in]          pt_req_received         Pointer to the structure containing request information.
 *
 *  @retval[out]        u32_return              Returns pdTRUE if the data was successfully sent.
 *                                              Returns err code if the data was failed sent.
 *
 */
uint32_t Modbus_rtos_send_packet_on_mailbox(uint16_t u16_mailbox_id, void * pt_request_received);

/** @brief receive data from the mailbox.
 *
 *  @param[in]          u16_mailbox_id          Mailbox ID number
 *  @param[in]          pt_request_received     Pointer to the structure containing request information.
 *
 *  @retval[out]        u32_return              Returns pdTRUE if the data was successfully receive.
 *                                              Returns err code if the data was failed receive.
 *
 */
uint32_t Modbus_rtos_receive_packet_on_mailbox(uint16_t u16_mailbox_id, void * pt_request_received);

/** @brief Delete mailbox.
 *
 *  @param[in]          u16_mailbox_id          Mailbox ID number
 *
 *  @retval[out]        u32_return              Returns ERR_OK if delete the mailbox successfully.
 *                                              Returns err code if delete the mailbox failed.
 *
 */
uint32_t Modbus_rtos_delete_mailbox(uint16_t u16_mailbox_id);

#endif /* _MODBUSPORTRTOS_H_ */
