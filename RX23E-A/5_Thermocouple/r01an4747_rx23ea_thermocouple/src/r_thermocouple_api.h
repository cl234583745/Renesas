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
 * File Name    : r_thermocouple_api.h
 * Version      : 1.0
 * Description  : thermocouple control header file
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.08.2019 1.00     First Release
 *         : 20.07.2020 1.10     update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_PRV_THERMOCOUPLE_API_H
    #define R_PRV_THERMOCOUPLE_API_H

    #define D_TC_REFOUT                 (2.5F)      /** REFOUT voltage [V]                                  */
    #define D_TC_PGA_GAIN               (128.0F)    /** PGA gain for thermocouple measurement               */
    #define D_TC_CODE_FS                (8388608)   /** 2^23                                                */
    #define D_TC_DF_GAIN                (0.677626F) /** Oversampling ratio 50000                            */

    /** Coefficient to convert A/D conversion value into thermoelectromotive force [uV]                     */
    #define D_TC_GAIN                   (D_TC_REFOUT/(D_TC_CODE_FS * D_TC_PGA_GAIN * D_TC_DF_GAIN) * 1000000)
    #define D_TC_OFFSET                 (0.0F)      /** Thermoelectric offset voltage [uV]                  */
    #define D_TC_TABLE_TOP_TEMPARATURE  (-75.0F)    /** First data of table variable is -75 degrees         */
    #define D_TC_TABLE_SIZE             (327)       /** Table variable range is -75 degrees to 251 degrees  */

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 *********************************************************************************************************************/
/*********************************************************************************************************************
 * Function Name: R_TC_TempToEmf
 * Description  : Convert temperature to Thermal electromotive force
 * Arguments    : float temp -
 *                    temperature
 * Return Value : float
 *                    eThermal electromotive
 ********************************************************************************************************************/
float R_TC_TempToEmf (float temp);

/**********************************************************************************************************************
 * Function Name: R_TC_DsadToEmf
 * Description  : Convert dsad to Thermal electromotive force
 * Arguments    : float dsad -
 *                    24bit DSAD value
 * Return Value : float
 *                    thermal electromotive
 *********************************************************************************************************************/
float R_TC_EmfToTemp (float emf);

/**********************************************************************************************************************
 * Function Name: R_TC_EmfToTemp
 * Description  : Convert Thermal electromotive force to temperature
 * Arguments    : float emf -
 *                    Thermal electromotive force
 * Return Value : float
 *                    Temperature
 *********************************************************************************************************************/
float R_TC_DsadToEmf (float dsad);

#endif /* R_PRV_THERMOCOUPLE_API_H */
