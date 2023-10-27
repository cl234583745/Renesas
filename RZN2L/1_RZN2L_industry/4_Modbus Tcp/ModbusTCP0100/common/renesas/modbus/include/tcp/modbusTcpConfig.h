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

#ifndef _MODBUSTCPCONFIG_H_
#define _MODBUSTCPCONFIG_H_

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/

/** @brief TCP Port number  
 *
 * By default the TCP Port is 502. If another port number is specified  
 * it will wait in both 502 and the specified port number
 */
#define TCP_PORT_NUMBER		                         502

/** @brief Maximum number of clients 
 *
 * The maximum number of clients supported in the MODBUS stack 
 */
#define MAXIMUM_NUMBER_OF_CLIENTS	                 8

/** @brief TCP Connection Timeout 
 *
 * The Timeout is specified in seconds
 */
#define CONNECTION_TIMEOUT_TCP		                 60

/** @brief TCP Retransmission Timeout 
 *
 * The Timeout is specified in seconds
 */
#define RETRANSMISSION_TIMEOUT_TCP	                 1

#endif /* _MODBUSTCPCONFIG_H_ */
/* End of File */
