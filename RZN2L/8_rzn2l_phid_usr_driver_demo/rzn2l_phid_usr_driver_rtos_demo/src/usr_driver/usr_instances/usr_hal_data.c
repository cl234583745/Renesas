#include "usr_hal_data.h"

usb_instance_ctrl_t g_basic0_ctrl;

#if !defined(g_usb_descriptor)
extern usb_descriptor_t g_usb_descriptor;
#endif

const usb_cfg_t g_basic0_cfg =
{ .usb_mode = USB_MODE_PERI,
  .usb_speed = USB_SPEED_FS,
  .module_number = 0,
  .type = USB_CLASS_PHID,
#if defined(g_usb_descriptor)
                .p_usb_reg = g_usb_descriptor,
#else
  .p_usb_reg = &g_usb_descriptor,
#endif
  .usb_complience_cb = NULL,
#if defined(VECTOR_NUMBER_USBFS_INT)
                .irq       = VECTOR_NUMBER_USBFS_INT,
#else
  .irq = 285,
#endif
#if defined(VECTOR_NUMBER_USBFS_RESUME)
                .irq_r     = VECTOR_NUMBER_USBFS_RESUME,
#else
  .irq_r = FSP_INVALID_VECTOR,
#endif
  .irq_d0 = 286,
  .irq_d1 = 287,
#if defined(VECTOR_NUMBER_USBHS_USB_INT_RESUME)
                .hsirq     = VECTOR_NUMBER_USBHS_USB_INT_RESUME,
#else
  .hsirq = FSP_INVALID_VECTOR,
#endif
  .hsirq_d0 = FSP_INVALID_VECTOR,
  .hsirq_d1 = FSP_INVALID_VECTOR,
  .ipl = (12),
  .ipl_r = (12),
  .ipl_d0 = 15,
  .ipl_d1 = 15,
  .hsipl = (12),
  .hsipl_d0 = 15,
  .hsipl_d1 = 15,
#if (BSP_CFG_RTOS != 0)
  .p_usb_apl_callback = usb_phid_callback,
#else
  .p_usb_apl_callback = usb_apl_callback,

#endif
#if defined(NULL)
  .p_context = NULL,
#else
  .p_context = &NULL,
#endif

};


/* Instance structure to use this module. */
const usb_instance_t g_basic0 =
{ .p_ctrl = &g_basic0_ctrl, .p_cfg = &g_basic0_cfg, .p_api = &g_usb_on_usb, };
