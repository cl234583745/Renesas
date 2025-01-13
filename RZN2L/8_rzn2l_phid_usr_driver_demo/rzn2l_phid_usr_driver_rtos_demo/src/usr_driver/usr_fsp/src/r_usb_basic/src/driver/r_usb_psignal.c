/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/******************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <r_usb_basic.h>
#include <r_usb_basic_api.h>
#include <r_usb_basic_cfg.h>

#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

/******************************************************************************
 * Exported global variables (to be accessed by other files)
 ******************************************************************************/
 #if USB_CFG_BC == USB_CFG_ENABLE
extern uint16_t g_usb_bc_detect;

 #endif                                /* USB_CFG_BC == USB_CFG_ENABLE */

/******************************************************************************
 * Renesas Abstracted Peripheral signal control functions
 ******************************************************************************/

/******************************************************************************
 * Function Name   : usb_pstd_bus_reset
 * Description     : A USB bus reset was issued by the host. Execute relevant pro-
 *               : cessing.
 * Arguments       : none
 * Return value    : none
 ******************************************************************************/
void usb_pstd_bus_reset (usb_utr_t * p_utr)
{
    uint16_t connect_info;

    /* Bus Reset */
    usb_pstd_busreset_function();

    /* Memory clear */
    usb_pstd_clr_mem();
    connect_info = usb_cstd_port_speed(p_utr);

    /* Callback */
    if (USB_NULL != g_usb_pstd_driver.devdefault)
    {
 #if USB_CFG_BC == USB_CFG_ENABLE
        (*g_usb_pstd_driver.devdefault)(p_utr, connect_info, g_usb_bc_detect);
 #else                                 /* USB_CFG_BC == USB_CFG_ENABLE */
        (*g_usb_pstd_driver.devdefault)(p_utr, connect_info, USB_NULL);
 #endif /* USB_CFG_BC == USB_CFG_ENABLE */
    }

    /* DCP configuration register  (0x5C) */
    hw_usb_write_dcpcfg(p_utr, 0);

    /* DCP maxpacket size register (0x5E) */
    hw_usb_write_dcpmxps(p_utr, g_usb_pstd_driver.p_devicetbl[USB_DEV_MAX_PKT_SIZE]);
}

/******************************************************************************
 * End of function usb_pstd_bus_reset
 ******************************************************************************/

/******************************************************************************
 * Function Name   : usb_pstd_attach_process
 * Description     : USB attach setting.
 * Arguments       : none
 * Return value    : none
 ******************************************************************************/
void usb_pstd_attach_process (usb_utr_t * p_utr)
{
    usb_cpu_delay_xms((uint16_t) 10);
 #if USB_CFG_BC == USB_CFG_ENABLE
    usb_pstd_bc_detect_process(p_utr);
 #endif                                /* USB_CFG_BC == USB_CFG_ENABLE */
    hw_usb_pset_dprpu(p_utr->ip);
}

/******************************************************************************
 * End of function usb_pstd_attach_process
 ******************************************************************************/

/******************************************************************************
 * Function Name   : usb_pstd_detach_process
 * Description     : Initialize USB registers for detaching, and call the callback
 *               : function that completes the detach.
 * Arguments       : none
 * Return value    : none
 ******************************************************************************/
void usb_pstd_detach_process (usb_utr_t * p_utr)
{
    uint16_t i;

 #if defined(BSP_MCU_GROUP_RA6M3) || defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZT2L) || defined(BSP_MCU_GROUP_RZN2L)
    hw_usb_clear_cnen(p_utr);
 #endif                                /* defined(BSP_MCU_GROUP_RA6M3) || defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZT2L) */
    /* Pull-up disable */
    hw_usb_pclear_dprpu(p_utr->ip);

    /* Configuration number */
    g_usb_pstd_config_num = 0;

    /* Remote wakeup enable flag */
    g_usb_pstd_remote_wakeup = USB_FALSE;

    /* WAIT_LOOP */
    for (i = USB_MIN_PIPE_NO; i < (USB_MAXPIPE_NUM + 1); i++)
    {
        if (USB_TRUE == g_usb_pipe_table[p_utr->ip][i].use_flag)
        {
            usb_pstd_forced_termination(i, (uint16_t) USB_DATA_STOP, p_utr);
            usb_cstd_clr_pipe_cnfg(p_utr, i);
        }
    }

    /* Callback */
    if (USB_NULL != g_usb_pstd_driver.devdetach)
    {
        (*g_usb_pstd_driver.devdetach)(p_utr, USB_NO_ARG, USB_NULL);
    }

    usb_pstd_stop_clock(p_utr->ip);
}

/******************************************************************************
 * End of function usb_pstd_detach_process
 ******************************************************************************/

/******************************************************************************
 * Function Name   : usb_pstd_suspend_process
 * Description     : Perform a USB peripheral suspend.
 * Arguments       : none
 * Return value    : none
 ******************************************************************************/
void usb_pstd_suspend_process (usb_utr_t * p_utr)
{
    uint16_t intsts0;
    uint16_t buf;

    /* Resume interrupt enable */
    hw_usb_pset_enb_rsme(p_utr->ip);

    intsts0 = hw_usb_read_intsts(p_utr->ip);
    buf     = hw_usb_read_syssts(p_utr);
    if (((uint16_t) 0 != (intsts0 & USB_DS_SUSP)) && (USB_FS_JSTS == (buf & USB_LNST)))
    {
        /* Suspend */
        usb_pstd_stop_clock(p_utr->ip);
        usb_pstd_suspend_function();

        /* Callback */
        if (USB_NULL != g_usb_pstd_driver.devsuspend)
        {
            (*g_usb_pstd_driver.devsuspend)(p_utr, g_usb_pstd_remote_wakeup, USB_NULL);
        }
    }

    /* --- SUSPEND -> RESUME --- */
    else
    {
        /* RESM status clear */
        hw_usb_pclear_sts_resm(p_utr->ip);

        /* RESM interrupt disable */
        hw_usb_pclear_enb_rsme(p_utr->ip);
    }
}

/******************************************************************************
 * End of function usb_pstd_suspend_process
 ******************************************************************************/
#endif                                 /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 * End  Of File
 ******************************************************************************/
