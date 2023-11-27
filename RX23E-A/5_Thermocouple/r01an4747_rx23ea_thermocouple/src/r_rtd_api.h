/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_rtd_api.h
 * Version      : 1.0
 * Description  : RTD control header file
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.08.2019 1.00     First Release
 *         : 20.07.2020 1.10     Update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_PRV_RTD_API_H
    #define R_PRV_RTD_API_H

    #define D_RTD_RREF                  (5100.0F)   /** REFOUT voltage [V]                                  */
    #define D_RTD_PGA_GAIN              (32.0F)     /** PGA gain for rtd measurement                        */
    #define D_RTD_CODE_FS               (8388608)   /** 2^23                                                */
    #define D_RTD_DF_GAIN               (0.677626F) /** Oversampling ratio 50000                            */

    /** Coefficient to convert A/D conversion value into RTD resistance  [ohm]                              */
    #define D_RTD_GAIN                  (D_RTD_RREF/(D_RTD_CODE_FS * D_RTD_PGA_GAIN * D_RTD_DF_GAIN))
    #define D_RTD_OFFSET                (0.0F)      /** RTD offset resistance [ohm]                         */
    #define D_RTD_TABLE_TOP_TEMPARATURE (-40.0F)    /** First data of table variable is -40 degrees         */
    #define D_RTD_TABLE_SIZE            (127)       /** Table variable range is -40 degrees to 86 degrees   */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RTD_DsadToTemp
 * Description  : RTD physical quantity calculation function. Convert DSAD value to temperature.
 * Arguments    : float dsad -
 *                    24bit dsad value
 * Return Value : float
 *                    temperature[degree]
 *********************************************************************************************************************/
float R_RTD_DsadToTemp (float dsad);

#endif /* R_PRV_RTD_API_H */
