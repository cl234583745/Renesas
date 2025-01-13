#ifndef USR_HAL_DATA_H_
#define USR_HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_phid_api.h"
//FSP_HEADER

/** External IRQ on ICU Instance. */
//extern const external_irq_instance_t g_external_user_btn;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
//extern icu_instance_ctrl_t g_external_user_btn_ctrl;
//extern const external_irq_cfg_t g_external_user_btn_cfg;

//#ifndef g_user_btn_cb
//void g_user_btn_cb(external_irq_callback_args_t *p_args);
//#endif
/* Basic on USB Instance. */
extern const usb_instance_t g_basic0;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic0_ctrl;
extern const usb_cfg_t g_basic0_cfg;

#ifndef NULL
void NULL(usb_event_info_t*, usb_hdl_t, usb_onoff_t);
#endif
/** PHID Driver on USB Instance. */

extern void usb_apl_callback(usb_callback_args_t * p_event);

#if 2 == BSP_CFG_RTOS
#ifndef usb_phid_callback
void usb_phid_callback(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif



//FSP_FOOTER
#endif /* USR_HAL_DATA_H_ */
