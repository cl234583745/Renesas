/* generated common source file - do not edit */
#include "common_data.h"
#if defined(BSP_MCU_GROUP_RA6M4) && (BSP_TZ_SECURE_BUILD != 1) && (BSP_TZ_NONSECURE_BUILD != 1)
#define ETHER_BUFFER_PLACE_IN_SECTION BSP_PLACE_IN_SECTION(".ns_buffer.eth")
#elif defined(BSP_MCU_GROUP_RZT2M)
#define ETHER_BUFFER_PLACE_IN_SECTION BSP_PLACE_IN_SECTION(".noncache_buffer.eth")
#else
#define ETHER_BUFFER_PLACE_IN_SECTION
#endif
ethsw_instance_ctrl_t g_ethsw0_ctrl;

const ethsw_cfg_t g_ethsw0_cfg = { .channel = 0,

.specific_tag = ETHSW_SPECIFIC_TAG_DISABLE, .specific_tag_id = 0xE001,

.phylink = ETHSW_PHYLINK_DISABLE,

#if defined(VECTOR_NUMBER_ETHSW_INTR)
    .irq                       = VECTOR_NUMBER_ETHSW_INTR,
#else
		.irq = FSP_INVALID_VECTOR,
#endif
		.ipl = (12), .p_callback = gmac_callback_ethsw, .p_context =
				&g_ether0_ctrl, .p_extend = NULL,

};
/* Instance structure to use this module. */
const ethsw_instance_t g_ethsw0 = { .p_ctrl = &g_ethsw0_ctrl, .p_cfg =
		&g_ethsw0_cfg, .p_api = &g_ethsw_on_ethsw };
ether_selector_instance_ctrl_t g_ether_selector2_ctrl;

const ether_selector_cfg_t g_ether_selector2_cfg = { .port = 2, .phylink =
		ETHER_SELECTOR_PHYLINK_POLARITY_LOW, .interface =
		ETHER_SELECTOR_INTERFACE_RGMII, .speed = ETHER_SELECTOR_SPEED_1000MBPS,
		.duplex = ETHER_SELECTOR_DUPLEX_FULL, .ref_clock =
				ETHER_SELECTOR_REF_CLOCK_INPUT, .p_extend = NULL, };

/* Instance structure to use this module. */
const ether_selector_instance_t g_ether_selector2 = { .p_ctrl =
		&g_ether_selector2_ctrl, .p_cfg = &g_ether_selector2_cfg, .p_api =
		&g_ether_selector_on_ether_selector };
ether_phy_instance_ctrl_t g_ether_phy2_ctrl;

const ether_phy_extend_cfg_t g_ether_phy2_extend =
		{ .port_type = ETHER_PHY_PORT_TYPE_ETHERNET, .phy_chip =
				(ether_phy_chip_t) BOARD_PHY_TYPE, .mdio_type =
				ETHER_PHY_MDIO_GMAC,

		.bps = ETHER_PHY_SPEED_10_1000, .duplex = ETHER_PHY_DUPLEX_FULL,
				.auto_negotiation = ETHER_PHY_AUTO_NEGOTIATION_ON,

				.phy_reset_pin = BSP_IO_PORT_13_PIN_4, .phy_reset_time = 15000,

				.p_selector_instance =
						(ether_selector_instance_t*) &g_ether_selector2, };

const ether_phy_cfg_t g_ether_phy2_cfg = {

.channel = 2, .phy_lsi_address = 2, .phy_reset_wait_time = 0x00020000,
		.mii_bit_access_wait_time = 0,                         // Unused
		.flow_control = ETHER_PHY_FLOW_CONTROL_DISABLE, .mii_type =
				(ether_phy_mii_type_t) 0,  // Unused
		.p_context = NULL, .p_extend = &g_ether_phy2_extend };

/* Instance structure to use this module. */
const ether_phy_instance_t g_ether_phy2 = { .p_ctrl = &g_ether_phy2_ctrl,
		.p_cfg = &g_ether_phy2_cfg, .p_api = &g_ether_phy_on_ether_phy };
ether_selector_instance_ctrl_t g_ether_selector1_ctrl;

const ether_selector_cfg_t g_ether_selector1_cfg = { .port = 1, .phylink =
		ETHER_SELECTOR_PHYLINK_POLARITY_LOW, .interface =
		ETHER_SELECTOR_INTERFACE_RGMII, .speed = ETHER_SELECTOR_SPEED_1000MBPS,
		.duplex = ETHER_SELECTOR_DUPLEX_FULL, .ref_clock =
				ETHER_SELECTOR_REF_CLOCK_INPUT, .p_extend = NULL, };

/* Instance structure to use this module. */
const ether_selector_instance_t g_ether_selector1 = { .p_ctrl =
		&g_ether_selector1_ctrl, .p_cfg = &g_ether_selector1_cfg, .p_api =
		&g_ether_selector_on_ether_selector };
ether_phy_instance_ctrl_t g_ether_phy1_ctrl;

const ether_phy_extend_cfg_t g_ether_phy1_extend =
		{ .port_type = ETHER_PHY_PORT_TYPE_ETHERNET, .phy_chip =
				(ether_phy_chip_t) BOARD_PHY_TYPE, .mdio_type =
				ETHER_PHY_MDIO_GMAC,

		.bps = ETHER_PHY_SPEED_10_1000, .duplex = ETHER_PHY_DUPLEX_FULL,
				.auto_negotiation = ETHER_PHY_AUTO_NEGOTIATION_ON,

				.phy_reset_pin = BSP_IO_PORT_13_PIN_4, .phy_reset_time = 15000,

				.p_selector_instance =
						(ether_selector_instance_t*) &g_ether_selector1, };

const ether_phy_cfg_t g_ether_phy1_cfg = {

.channel = 1, .phy_lsi_address = 1, .phy_reset_wait_time = 0x00020000,
		.mii_bit_access_wait_time = 0,                         // Unused
		.flow_control = ETHER_PHY_FLOW_CONTROL_DISABLE, .mii_type =
				(ether_phy_mii_type_t) 0,  // Unused
		.p_context = NULL, .p_extend = &g_ether_phy1_extend };

/* Instance structure to use this module. */
const ether_phy_instance_t g_ether_phy1 = { .p_ctrl = &g_ether_phy1_ctrl,
		.p_cfg = &g_ether_phy1_cfg, .p_api = &g_ether_phy_on_ether_phy };
ether_selector_instance_ctrl_t g_ether_selector0_ctrl;

const ether_selector_cfg_t g_ether_selector0_cfg = { .port = 0, .phylink =
		ETHER_SELECTOR_PHYLINK_POLARITY_LOW, .interface =
		ETHER_SELECTOR_INTERFACE_RGMII, .speed = ETHER_SELECTOR_SPEED_1000MBPS,
		.duplex = ETHER_SELECTOR_DUPLEX_FULL, .ref_clock =
				ETHER_SELECTOR_REF_CLOCK_INPUT, .p_extend = NULL, };

/* Instance structure to use this module. */
const ether_selector_instance_t g_ether_selector0 = { .p_ctrl =
		&g_ether_selector0_ctrl, .p_cfg = &g_ether_selector0_cfg, .p_api =
		&g_ether_selector_on_ether_selector };
ether_phy_instance_ctrl_t g_ether_phy0_ctrl;

const ether_phy_extend_cfg_t g_ether_phy0_extend =
		{ .port_type = ETHER_PHY_PORT_TYPE_ETHERNET, .phy_chip =
				(ether_phy_chip_t) ETHER_PHY_CHIP_VSC8541, .mdio_type =
				ETHER_PHY_MDIO_GMAC,

		.bps = ETHER_PHY_SPEED_10_1000, .duplex = ETHER_PHY_DUPLEX_FULL,
				.auto_negotiation = ETHER_PHY_AUTO_NEGOTIATION_ON,

				.phy_reset_pin = BSP_IO_PORT_13_PIN_4, .phy_reset_time = 15000,

				.p_selector_instance =
						(ether_selector_instance_t*) &g_ether_selector0, };

const ether_phy_cfg_t g_ether_phy0_cfg = {

.channel = 0, .phy_lsi_address = 0, .phy_reset_wait_time = 0x00020000,
		.mii_bit_access_wait_time = 0,                         // Unused
		.flow_control = ETHER_PHY_FLOW_CONTROL_DISABLE, .mii_type =
				(ether_phy_mii_type_t) 0,  // Unused
		.p_context = NULL, .p_extend = &g_ether_phy0_extend };

/* Instance structure to use this module. */
const ether_phy_instance_t g_ether_phy0 = { .p_ctrl = &g_ether_phy0_ctrl,
		.p_cfg = &g_ether_phy0_cfg, .p_api = &g_ether_phy_on_ether_phy };
const ether_phy_instance_t *g_ether0_phy_instance[BSP_FEATURE_GMAC_MAX_PORTS] =
		{
#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED == g_ether_phy0)
                    NULL,
#else
				&g_ether_phy0,
#endif
#if (FSP_NOT_DEFINED == g_ether_phy1)
                    NULL,
#else
				&g_ether_phy1,
#endif
#if (FSP_NOT_DEFINED == g_ether_phy2)
                    NULL,
#else
				&g_ether_phy2,
#endif
#undef FSP_NOT_DEFINED
		};

gmac_instance_ctrl_t g_ether0_ctrl;

uint8_t g_ether0_mac_address[6] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

__attribute__((__aligned__(16)))  gmac_instance_descriptor_t g_ether0_tx_descriptors[8] ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(16)))  gmac_instance_descriptor_t g_ether0_rx_descriptors[8] ETHER_BUFFER_PLACE_IN_SECTION;

__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer0[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer1[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer2[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer3[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer4[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer5[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer6[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer7[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer8[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer9[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer10[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer11[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer12[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer13[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer14[2048]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(32)))uint8_t g_ether0_ether_buffer15[2048]ETHER_BUFFER_PLACE_IN_SECTION;

uint8_t *pp_g_ether0_ether_buffers[(8 + 8)] = {
		(uint8_t*) &g_ether0_ether_buffer0[0],
		(uint8_t*) &g_ether0_ether_buffer1[0],
		(uint8_t*) &g_ether0_ether_buffer2[0],
		(uint8_t*) &g_ether0_ether_buffer3[0],
		(uint8_t*) &g_ether0_ether_buffer4[0],
		(uint8_t*) &g_ether0_ether_buffer5[0],
		(uint8_t*) &g_ether0_ether_buffer6[0],
		(uint8_t*) &g_ether0_ether_buffer7[0],
		(uint8_t*) &g_ether0_ether_buffer8[0],
		(uint8_t*) &g_ether0_ether_buffer9[0],
		(uint8_t*) &g_ether0_ether_buffer10[0],
		(uint8_t*) &g_ether0_ether_buffer11[0],
		(uint8_t*) &g_ether0_ether_buffer12[0],
		(uint8_t*) &g_ether0_ether_buffer13[0],
		(uint8_t*) &g_ether0_ether_buffer14[0],
		(uint8_t*) &g_ether0_ether_buffer15[0],

};

const gmac_extend_cfg_t g_ether0_extend_cfg =
		{ .p_rx_descriptors = g_ether0_rx_descriptors, .p_tx_descriptors =
				g_ether0_tx_descriptors,

		.phylink = GMAC_PHYLINK_DISABLE,

#if defined(VECTOR_NUMBER_GMAC_PMT)
                .pmt_irq                 = VECTOR_NUMBER_GMAC_PMT,
#else
				.pmt_irq = FSP_INVALID_VECTOR,
#endif
				.pmt_interrupt_priority = (12),

				.pp_phy_instance =
						(ether_phy_instance_t const *(*)[BSP_FEATURE_GMAC_MAX_PORTS]) g_ether0_phy_instance,
				.p_ethsw_instance = &g_ethsw0 };

const ether_cfg_t g_ether0_cfg = { .channel = 0, .zerocopy =
		ETHER_ZEROCOPY_DISABLE, .multicast = (ether_multicast_t) 0,    // Unused
		.promiscuous = ETHER_PROMISCUOUS_ENABLE, .flow_control =
				ETHER_FLOW_CONTROL_DISABLE, .broadcast_filter = 0, // Unused
		.p_mac_address = g_ether0_mac_address,

		.num_tx_descriptors = 8, .num_rx_descriptors = 8,

		.pp_ether_buffers = pp_g_ether0_ether_buffers,

		.ether_buffer_size = 2048,

#if defined(VECTOR_NUMBER_GMAC_SBD)
                .irq                     = VECTOR_NUMBER_GMAC_SBD,
#else
		.irq = FSP_INVALID_VECTOR,
#endif
		.interrupt_priority = (12),

		.p_callback = vEtherISRCallback, .p_ether_phy_instance =
				(ether_phy_instance_t*) 0, // Unused
		.p_context = NULL, .p_extend = &g_ether0_extend_cfg };

/* Instance structure to use this module. */
const ether_instance_t g_ether0 = { .p_ctrl = &g_ether0_ctrl, .p_cfg =
		&g_ether0_cfg, .p_api = &g_ether_on_gmac };
ether_instance_t const *gp_freertos_ether = &g_ether0;
/** IOPORT interface configuration for event link **/
const ioport_extend_cfg_t g_ioport_cfg_extend = {
		.port_group_output_cfg[IOPORT_PORT_GROUP_1] =
		{ .pin_select = (uint8_t)(IOPORT_EVENT_PIN_SELECTION_NONE), .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW },
		.port_group_output_cfg[IOPORT_PORT_GROUP_2] =
		{ .pin_select = (uint8_t)(IOPORT_EVENT_PIN_SELECTION_NONE), .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW },
		.port_group_input_cfg[IOPORT_PORT_GROUP_1] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .pin_select =
				(uint8_t)(IOPORT_EVENT_PIN_SELECTION_NONE), .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE, .overwrite_control =
				IOPORT_EVENT_CONTROL_DISABLE, .buffer_init_value =
				IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 7U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 6U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 5U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 3U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 2U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 1U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW },
		.port_group_input_cfg[IOPORT_PORT_GROUP_2] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .pin_select =
				(uint8_t)(IOPORT_EVENT_PIN_SELECTION_NONE), .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE, .overwrite_control =
				IOPORT_EVENT_CONTROL_DISABLE, .buffer_init_value =
				IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 6U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 5U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 4U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 3U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 2U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 1U
						| IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW },
		.single_port_cfg[IOPORT_SINGLE_PORT_0] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction =
				IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
				(uint16_t) BSP_IO_PORT_16_PIN_0, .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE },
		.single_port_cfg[IOPORT_SINGLE_PORT_1] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction =
				IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
				(uint16_t) BSP_IO_PORT_16_PIN_0, .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE },
		.single_port_cfg[IOPORT_SINGLE_PORT_2] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction =
				IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
				(uint16_t) BSP_IO_PORT_16_PIN_0, .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE },
		.single_port_cfg[IOPORT_SINGLE_PORT_3] =
		{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction =
				IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
				(uint16_t) BSP_IO_PORT_16_PIN_0, .operation =
				IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection =
				IOPORT_EVENT_DETECTION_RISING_EDGE } };

ioport_instance_ctrl_t g_ioport_ctrl;

const ioport_instance_t g_ioport = { .p_api = &g_ioport_on_ioport, .p_ctrl =
		&g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg };

void g_common_init(void) {
}
