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

#ifndef _MODBUSTYPEDEF_H_
#define _MODBUSTYPEDEF_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
/**
 * Used to select MODBUS Stack RTU master mode 
 */
#define MODBUS_RTU_MASTER_MODE		1	
/** 
 * Used to select MODBUS Stack RTU slave mode 
 */
#define MODBUS_RTU_SLAVE_MODE		2	
/** 
 * Used to select MODBUS Stack ASCII master mode 
 */
#define MODBUS_ASCII_MASTER_MODE	3	
/** 
 * Used to select MODBUS Stack ASCII slave mode 
 */
#define MODBUS_ASCII_SLAVE_MODE		4
/** 
 * Used to select MODBUS Stack TCP server mode 
 */
#define MODBUS_TCP_SERVER_MODE		5
/** 
 * Used to select MODBUS Stack TCP server mode with gateway 
 */   
#define MODBUS_TCP_GATEWAY_MODE		6
   
/**
 * Maximum data size for coils(discrete output) and discrete inputs
 */
#define MAX_DISCRETE_DATA              251

/**
 * Maximum data size for input and holding registers
 */
#define MAX_REG_DATA                   125

/*===============================================================================================================================*/
/* T Y P E D E F S                                                                                                               */
/*===============================================================================================================================*/
/**
 * For char 
 */
typedef char char_t;

/**
 * For void 
 */
typedef void void_t;

/**
 * For void pointer 
 */
typedef void* pvoid_t;

/**
 * For char pointer 
 */
typedef char* pchar_t;

/**
 * For signed int pointer 
 */
typedef signed int* pint32_t;

/**
 * For unsigned char pointer 
 */
typedef unsigned char* puint8_t;

/**
 * For unsigned short pointer 
 */
typedef unsigned short* puint16_t;

/**
 * For unsigned int pointer 
 */
typedef unsigned int* puint32_t;

/*===============================================================================================================================*/
/* S T R U C T U R E                                                                                                             */
/*===============================================================================================================================*/

/**
 * Request to read coils 
 */
typedef struct _req_read_coils
{
  uint16_t        u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;	          /**< Identification of a remote slave connected */
  uint16_t	  u16_start_addr;             /**< Specifies address of the first coil */
  uint16_t	  u16_num_of_coils;           /**< Specifies the number of coils to be read */
}req_read_coils_t;
typedef struct _req_read_coils* p_req_read_coils_t;

/**
 * Request to read inputs 
 */
typedef struct _req_read_inputs
{
  uint16_t        u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;            /**< Identification of a remote slave connected */
  uint16_t        u16_start_addr;         /**< Specifies address of the first discrete input */
  uint16_t        u16_num_of_inputs;	  /**< Specifies the number of discrete inputs to be read */
}req_read_inputs_t;
typedef struct _req_read_inputs* p_req_read_inputs_t;

/**
 * Request to read holding registers 
 */
typedef struct _req_read_holding_reg
{
  uint16_t    u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t    u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t	  u8_slave_id;            /**< Identification of a remote slave connected */
  uint16_t 	  u16_start_addr;         /**< Specifies address of the first holding register */
  uint16_t 	  u16_num_of_reg;         /**< Specifies the number of registers to be read */
}req_read_holding_reg_t;
typedef struct _req_read_holding_reg* p_req_read_holding_reg_t;

/**
 * Request to read input registers 
 */
typedef struct _req_read_input_reg
{
  uint16_t        u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;            /**< Identification of a remote slave connected */
  uint16_t        u16_start_addr;         /**< Specifies address of the first input register */
  uint16_t        u16_num_of_reg;         /**< Specifies the number of registers to be read */
}req_read_input_reg_t;
typedef struct _req_read_input_reg * p_req_read_input_reg_t;

/**
 * Request to write single coil 
 */
typedef struct _req_write_single_coil
{
  uint16_t        u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;		      /**< Identification of a remote slave connected */
  uint16_t        u16_output_addr;	      /**< Specifies address of the coil */
  uint16_t        u16_output_value;	      /**< Data to be written */
}req_write_single_coil_t;
typedef struct _req_write_single_coil* p_req_write_single_coil_t;

/**
 * Request to write single register 
 */
typedef struct _req_write_single_reg
{
  uint16_t        u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;            /**< Identification of a remote slave connected */
  uint16_t 	      u16_register_addr;      /**< Specifies address of the register */
  uint16_t 	      u16_register_value;     /**< Data to be written */
}req_write_single_reg_t;
typedef struct _req_write_single_reg* p_req_write_single_reg_t;

/**
 * Request to write multiple coils 
 */
typedef struct _req_write_multiple_coils
{
  uint16_t        u16_transaction_id;           /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;              /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;                  /**< Identification of a remote slave connected */
  uint16_t        u16_start_addr;               /**< Specifies address of the first coil */
  uint16_t        u16_num_of_outputs;           /**< Specifies the number of coils to be written */
  uint8_t         u8_num_of_bytes;              /**< Specifies the number of bytes of data*/
  uint8_t         aru8_data[MAX_DISCRETE_DATA]; /**< Data to be written */
}req_write_multiple_coils_t;
typedef struct _req_write_multiple_coils* p_req_write_multiple_coils_t;

/**
 * Request to write multiple registers 
 */
typedef struct _req_write_multiple_reg
{
  uint16_t        u16_transaction_id;       /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;          /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;              /**< Identification of a remote slave connected */
  uint16_t        u16_start_addr;           /**< Specifies address of the first register */
  uint16_t        u16_num_of_reg;           /**< Specifies the number of registers to be written */
  uint8_t         u8_num_of_bytes;          /**< Specifies the number of bytes of data*/
  uint16_t        aru16_data[MAX_REG_DATA]; /**< Data to be written */
}req_write_multiple_reg_t;
typedef struct _req_write_multiple_reg* p_req_write_multiple_reg_t;

/**
 * Request to read and write multiple registers 
 */
typedef struct _req_read_write_multiple_reg
{
  uint16_t        u16_transaction_id;       /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;          /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;              /**< Identification of a remote slave connected */
  uint16_t        u16_read_start_addr;      /**< Specifies address of the first register to be read from */
  uint16_t        u16_num_to_read;          /**< Specifies the number of registers to be read */
  uint16_t        u16_write_start_addr;     /**< Specifies address of the first register to be written to */
  uint16_t        u16_num_to_write;         /**< Specifies the number of registers to be written */
  uint8_t         u8_write_num_of_bytes;    /**< Specifies the number of bytes of data*/
  uint16_t        aru16_data[MAX_REG_DATA]; /**< Data to be written */
}req_read_write_multiple_reg_t;
typedef struct _req_read_write_multiple_reg* p_req_read_write_multiple_reg_t;

/** 
 * Response of read coils 
 */
struct _resp_read_coils
{
  uint16_t        u16_transaction_id;           /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;              /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;                  /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;            /**< Error detected during processing the request. On success the exception code should 
						                             be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t         u8_num_of_bytes;	            /**< Specifies the number of bytes of data*/
  uint8_t         aru8_data[MAX_DISCRETE_DATA]; /**< Data to be read */
};
typedef struct _resp_read_coils resp_read_coils_t;
typedef struct _resp_read_coils* p_resp_read_coils_t;

/** 
 * Response of read inputs 
 */
typedef struct _resp_read_inputs
{
  uint16_t        u16_transaction_id;           /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;              /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;                  /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;            /**< Error detected during processing the request. On success the exception code should 
                                                     be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t         u8_num_of_bytes;              /**< Specifies the number of bytes of data*/
  uint8_t         aru8_data[MAX_DISCRETE_DATA]; /**< Buffer to store the read data */
}resp_read_inputs_t;
typedef struct _resp_read_inputs* p_resp_read_inputs_t;

/** 
 * Response of read holding registers 
 */
typedef struct _resp_read_holding_reg
{
  uint16_t        u16_transaction_id;         /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;            /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;                /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;          /**< Error detected during processing the request. On success the exception code should 
                                                   be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t         u8_num_of_bytes;            /**< Specifies the number of bytes of data*/
  uint16_t        aru16_data[MAX_REG_DATA];   /**< Buffer to store the read data */
}resp_read_holding_reg_t;
typedef struct _resp_read_holding_reg* p_resp_read_holding_reg_t;

/**
 * Response of read input registers 
 */
typedef struct _resp_read_input_reg
{
  uint16_t        u16_transaction_id;        /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;           /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;               /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;         /**< Error detected during processing the request. On success the exception code should 
                                                  be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t         u8_num_of_bytes;           /**< Specifies the number of bytes of data*/
  uint16_t        aru16_data[MAX_REG_DATA];  /**< Buffer to store the read data */
}resp_read_input_reg_t;
typedef struct _resp_read_input_reg* p_resp_read_input_reg_t;

/**
 * Response of write single coil 
 */
typedef struct _resp_write_single_coil
{
  uint16_t      u16_transaction_id;      /**< Specifies the transaction ID */
  uint16_t      u16_protocol_id;         /**< Specifies the protocol ID */
  uint8_t       u8_slave_id;             /**< Identification of a remote slave connected(Own ID) */
  uint8_t       u8_exception_code;       /**< Error detected during processing the request. 
                                              On success the exception code should be zero */
  uint16_t      u16_output_addr;         /**< Specifies address of the coil */
  uint16_t      u16_output_value;        /**< Data to be written */
}resp_write_single_coil_t;
typedef struct _resp_write_single_coil* p_resp_write_single_coil_t;

/**
 * Response of write single register 
 */
typedef struct _resp_write_single_reg
{
  uint16_t        u16_transaction_id;      /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;         /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;             /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;       /**< Error detected during processing the request. 
                                                On success the exception code should be zero */
  uint16_t        u16_register_addr;       /**< Specifies address of the register */
  uint16_t        u16_register_value;      /**< Data to be written */
}resp_write_single_reg_t;
typedef struct _resp_write_single_reg* p_resp_write_single_reg_t;

/**
 * Response of write multiple coils 
 */
typedef struct _resp_write_multiple_coils
{
  uint16_t        u16_transaction_id;      /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;         /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;             /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;       /**< Error detected during processing the request. 
                                                On success the exception code should be zero */
  uint16_t        u16_start_addr;          /**< Specifies address of the first coil */
  uint16_t        u16_num_of_outputs;      /**< Specifies the number of coils to be written */
}resp_write_multiple_coils_t;
typedef struct _resp_write_multiple_coils* p_resp_write_multiple_coils_t;

/**
 * Response of write multiple registers 
 */
typedef struct _resp_write_multiple_reg
{
  uint16_t        u16_transaction_id; /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;    /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;        /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;  /**< Error detected during processing the request. On success the exception code should 
                                           be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint16_t        u16_start_addr;     /**< Specifies address of the first register */
  uint16_t        u16_num_of_reg;     /**< Specifies the number of registers to be written */
}resp_write_multiple_reg_t;
typedef struct _resp_write_multiple_reg* p_resp_write_multiple_reg_t;


/**
 * Response of read and write multiple registers 
 */
typedef struct _resp_read_write_multiple_reg
{
  uint16_t        u16_transaction_id;            /**< Specifies the transaction ID */
  uint16_t        u16_protocol_id;               /**< Specifies the protocol ID */
  uint8_t         u8_slave_id;                   /**< Identification of a remote slave connected(Own ID) */
  uint8_t         u8_exception_code;	         /**< Error detected during processing the request. On success the exception code should 
                                                       be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t         u8_num_of_bytes;                /**< Specifies the number of complete bytes of data*/		
  uint16_t        aru16_read_data[MAX_REG_DATA];  /**< Data to be read */
}resp_read_write_multiple_reg_t;
typedef struct _resp_read_write_multiple_reg* p_resp_read_write_multiple_reg_t;

/**
 * Response of an unsupported function code
 */
typedef struct _resp_invalid_function_code_t
{
  uint16_t    u16_transaction_id;     /**< Specifies the transaction ID */
  uint16_t    u16_protocol_id;        /**< Specifies the protocol ID */
  uint8_t     u8_slave_id;            /**< Identification of a remote slave connected(Own ID) */
  uint8_t     u8_exception_code;      /**< Error detected during processing the request. On success the exception code should 
                                           be zero, if the exception code is non zero the aru8_data[253] will be null */
  uint8_t     u8_num_of_bytes;	      /**< Specifies the number of complete bytes of data*/		
}resp_invalid_function_code_t;
typedef struct _resp_invalid_function_code_t* p_resp_invalid_function_code_t;

/*===============================================================================================================================*/
/* E N U M                                                      	    				                                         */
/*===============================================================================================================================*/

typedef enum
{
  ERR_OK = 0x00,                /**< Specifies success code */          
  ERR_ILLEGAL_FUNCTION,         /**< Specifies the function code received in the request is not an 
                                     allowable action for the server (or slave) or the function code
                                     is not implemented. This value must be a constant, cannot change the value from 0x01 */
  ERR_ILLEGAL_DATA_ADDRESS,     /**< Specifies the data address received in the request is not an allowable address for the server 
                                     (or slave) or the addressed register is not implemented. This value must be a constant, cannot 
                                     change the value from 0x02 */
  ERR_ILLEGAL_DATA_VALUE,       /**< Specifies a value contained in the request data field is not an allowable value for the server 
                                     (or slave).This value must be a constant, cannot change the value from 0x03 */
  ERR_SLAVE_DEVICE_FAILURE,     /**< Specifies an unrecoverable error occurred while the server (or slave) was attempting to perform 
                                     the requested action. This value must be a constant, cannot change the value from 0x04 */
  ERR_STACK_INIT,               /**< In stack initialization failure  */
  ERR_ILLEGAL_SERV_BSY,         /**< Specifies the maximum transaction reached. This value must be a constant, cannot change the 
                                      value from 0x06 */          
  ERR_CRC_CHECK,                 /**< Specifies the CRC check has failed */
  ERR_LRC_CHECK,                 /**< Specifies the LRC check has failed */
  ERR_INVALID_SLAVE_ID,          /**< Specifies the slave ID is greater than 247  */
  ERR_TCP_SND_MBX_FULL,          /**< Specifies that the mailbox is full */
  ERR_STACK_TERM,                /**< In stack termination failure */	
  ERR_TIME_OUT,                  /**< Timeout error added */
  ERR_MEM_ALLOC,                 /**< Memory allocation failure */ 
  ERR_SYSTEM_INTERNAL,           /**< Mailbox send or receive failure */
  ERR_ILLEGAL_NUM_OF_COILS,      /**< Specifies the number of coils provided is not within the specified limit */
  ERR_ILLEGAL_NUM_OF_INPUTS,     /**< Specifies the number of inputs provided is not within the specified limit */ 
  ERR_ILLEGAL_NUM_OF_REG,        /**< Specifies the number of registers provided is not within the specified limit */ 
  ERR_ILLEGAL_OUTPUT_VALUE,      /**< Specifies the value of the registers is invalid */ 
  ERR_ILLEGAL_NUM_OF_OUTPUTS,    /**< Specifies the number of outputs is invalid */ 
  ERR_INVALID_STACK_INIT_PARAMS, /**< Specifies invalid stack init information from user */
  ERR_INVALID_STACK_MODE,        /**< Stack mode specified is invalid */ 
  ERR_FUN_CODE_MISMATCH,         /**< Master receives a response for another function code(not for the requested function code) */
  ERR_SLAVE_ID_MISMATCH,         /**< Master receives a response from another slave (not from the requested slave) */
  ERR_OK_WITH_NO_RESPONSE,       /**< Return status for broadcast requests */
  ERR_MSG_SIZE_OVER,             /**< Request or response data received exceeds the maximum length */
  ERR_RTOS_INVALID_ID,           /**< Specifies the ID is invalid  */
  ERR_RTOS_TIMEOUT,              /**< No response was obtained by the expected time  */
  ERR_RTOS_ABORTED,              /**< There is something wrong with the result  */
  ERR_TCP_IP_TABLE_DISABLED,
  ERR_TCP_IP_TABLE_EMPTY,
  ERR_TCP_IP_TABLE_IP_NOT_FOUND,
  ERR_TCP_IP_TABLE_IP_ALREADY_PRESENT,
  ERR_TCP_IP_TABLE_MAX_CLIENT,

  ERR_TCP_INVALID_SOCKET = -1,
  ERR_TCP_ABORTED = -2
}ERR_CODE;

#endif /** _MODBUSTYPEDEF_H_ */

/* End of file */
