/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 SCIFA program
* File Name    : sio_char.h
* Version      : 1.4
* Device       : R7S910017
* Abstract     : Serial I/O settings controlling the char.
* Tool-Chain   : IAR Embedded Workbench for ARM
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Serial I/O settings for cntrolling the char.
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 27.02.2015 1.0      First Release
*              : 01.07.2015 1.1      Second Release
*              : 30.10.2015 1.2      Third Release
*                                     - No modification
*              : 31.03.2017 1.3      4th Release
*                                     - No modification
*              : 31.05.2018 1.4      5th Release
*                                     - No modification
*******************************************************************************/
#ifndef _SIO_CHAR_HEADER_
#define _SIO_CHAR_HEADER_

typedef char            char_t;

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define TRANSFER_LENGTH (1)

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t SioWrite(int32_t file_no, const char_t * buffer, uint32_t writing_b);
int32_t SioRead(int32_t file_no, char_t * buffer, uint32_t reading_b);

int32_t IoGetchar(void);
void    IoPutchar(int32_t buffer);


#endif  /* _SIO_CHAR_HEADER_ */

/* End of File */
