/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_ethsw.h"
#include "r_ethsw_api.h"
#include "r_ether_selector.h"
#include "r_ether_selector_api.h"
#include "r_ether_phy.h"
#include "r_ether_phy_api.h"
#include "r_gmac.h"
#include "r_ether_api.h"
#include "FreeRTOSIPConfig.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/** ether on ethsw Instance. */
extern const ethsw_instance_t g_ethsw0;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ethsw_instance_ctrl_t g_ethsw0_ctrl;
extern const ethsw_cfg_t g_ethsw0_cfg;
#ifndef gmac_callback_ethsw
void gmac_callback_ethsw(ethsw_callback_args_t *const p_arg);
#endif
/** ether_selector on ether_selector Instance. */
extern const ether_selector_instance_t g_ether_selector2;

/** Access the Ethernet Selector instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_selector_instance_ctrl_t g_ether_selector2_ctrl;
extern const ether_selector_cfg_t g_ether_selector2_cfg;
/** ether_phy on ether_phy Instance. */
extern const ether_phy_instance_t g_ether_phy2;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_phy_instance_ctrl_t g_ether_phy2_ctrl;
extern const ether_phy_cfg_t g_ether_phy2_cfg;
/** ether_selector on ether_selector Instance. */
extern const ether_selector_instance_t g_ether_selector1;

/** Access the Ethernet Selector instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_selector_instance_ctrl_t g_ether_selector1_ctrl;
extern const ether_selector_cfg_t g_ether_selector1_cfg;
/** ether_phy on ether_phy Instance. */
extern const ether_phy_instance_t g_ether_phy1;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_phy_instance_ctrl_t g_ether_phy1_ctrl;
extern const ether_phy_cfg_t g_ether_phy1_cfg;
/** ether_selector on ether_selector Instance. */
extern const ether_selector_instance_t g_ether_selector0;

/** Access the Ethernet Selector instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_selector_instance_ctrl_t g_ether_selector0_ctrl;
extern const ether_selector_cfg_t g_ether_selector0_cfg;
/** ether_phy on ether_phy Instance. */
extern const ether_phy_instance_t g_ether_phy0;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_phy_instance_ctrl_t g_ether_phy0_ctrl;
extern const ether_phy_cfg_t g_ether_phy0_cfg;
/** ether on ether Instance. */
extern const ether_instance_t g_ether0;

/** Access the Ethernet instance using these structures when calling API functions directly (::p_api is not used). */
extern gmac_instance_ctrl_t g_ether0_ctrl;
extern const ether_cfg_t g_ether0_cfg;

#ifndef vEtherISRCallback
void vEtherISRCallback(ether_callback_args_t *p_args);
#endif
extern ether_instance_t const *gp_freertos_ether;
/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
