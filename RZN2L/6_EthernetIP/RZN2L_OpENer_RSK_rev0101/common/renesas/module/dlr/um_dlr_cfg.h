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

#ifndef UM_DLR_CFG_H_
#define UM_DLR_CFG_H_

/* Configuration by user */
#define UM_DLR_CFG_PARAM_CHECKING_ENABLE	(1)

#define UM_DLR_CFG_HOST_ENDIAN_LITTLE		(1)

#define UM_DLR_CFG_RECEIVER_TASK_NAME				("DLR Receiver Task")
#define UM_DLR_CFG_RECEIVER_TASK_PRIORITY			(5)
#define UM_DLR_CFG_RECEIVER_TASK_STACK_BTYE_SIZE	(1024)
#define UM_DLR_CFG_RECEIVER_QUEUE_LENGTH			(16)

#define UM_DLR_CFG_EXECUTOR_TASK_NAME				("DLR Executor Task")
#define UM_DLR_CFG_EXECUTOR_TASK_PRIORITY			(5)
#define UM_DLR_CFG_EXECUTOR_TASK_STACK_BTYE_SIZE	(1024)
#define UM_DLR_CFG_EXECUTOR_QUEUE_LENGTH			(16)

#define UM_DLR_CFG_PORT0_ETHER_PORT_NUMBER			(0)
#define UM_DLR_CFG_PORT1_ETHER_PORT_NUMBER			(1)

#endif /* UM_DLR_CFG_H_ */
