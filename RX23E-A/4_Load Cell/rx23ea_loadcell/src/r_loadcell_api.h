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
 * File Name    : r_loadcell_api.h
 * Version      : 1.0
 * Description  : Load cell control header file
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
#ifndef R_PRV_LOADCELL_API_H
    #define R_PRV_LOADCELL_API_H

    #define D_LC_PGA_GAIN           (128.0F)
    #define D_LC_CODE_FS            (16777216)//(16777216)//16762400
    #define D_LC_VREF               (5.0F)
    #define D_LC_VCC                (5.0F)
    #define D_LC_RO                 (0.001F)//(0.0009F)
    #define D_LC_MMAX               (1000.0F)//(600.0F)
    #define D_LC_DF_GAIN            (0.677626F) /** Oversampling ratio 50000                            */

    #define D_LC_COEFFICIENT_A      ((D_LC_MMAX * D_LC_VREF * 2) / \
                                        (D_LC_RO * D_LC_VCC * D_LC_PGA_GAIN * D_LC_CODE_FS * D_LC_DF_GAIN))
    #define D_LC_COEFFICIENT_B      (0.0F)

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
 * Function Name: R_LC_DsadToWeight
 * Description  : This function inputs the A/D value and returns the weight.
 * Arguments    : float dsad -
 *                    A/D value.
 *              : float coef_a -
 *                    weight conversion coefficient A.
 *              : float coef_b -
 *                    weight conversion coefficient B.
 * Return Value : float
 *                    weight[g].
 *********************************************************************************************************************/
float R_LC_DsadToWeight (float dsad, float coef_a, float coef_b);

#endif /* R_PRV_LOADCELL_API_H */
