/* generated thread source file - do not edit */
#include "main_thread.h"

#if defined(BSP_MCU_GROUP_RA6M4) && (BSP_TZ_SECURE_BUILD != 1) && (BSP_TZ_NONSECURE_BUILD != 1)
#define ETHER_BUFFER_PLACE_IN_SECTION BSP_PLACE_IN_SECTION(".ns_buffer.eth")
#elif defined(BSP_MCU_GROUP_RZT2M)
#define ETHER_BUFFER_PLACE_IN_SECTION BSP_PLACE_IN_SECTION(".noncache_buffer.eth")
#else
#define ETHER_BUFFER_PLACE_IN_SECTION
#endif
#if 1
static StaticTask_t main_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t main_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
static uint8_t main_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.main_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t main_thread;
void main_thread_create(void);
static void main_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
sci_uart_instance_ctrl_t g_uart0_ctrl;

baud_setting_t g_uart0_baud_setting = {
/* Baud rate calculated with 0.160% error. */.abcse = 0, .abcs = 0, .bgdm = 1,
		.cks = 0, .brr = 51, .mddr = (uint8_t) 256, .brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend = {
		.clock = SCI_UART_CLOCK_INT, .rx_edge_start =
				SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
				SCI_UART_NOISE_CANCELLATION_DISABLE, .rx_fifo_trigger =
				SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting =
				&g_uart0_baud_setting, .uart_mode = UART_MODE_RS232,
		.ctsrts_en = SCI_UART_CTSRTS_RTS_OUTPUT,
#if 0
                .flow_control_pin       = BSP_IO_PORT_00_PIN_00,
#else
		.flow_control_pin = (bsp_io_port_pin_t)(0xFFFFU),
#endif
		.sync_bypass = SCI_UART_SYNCHRONIZER_NOT_BYPASS, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg = { .channel = 0, .data_bits = UART_DATA_BITS_8,
		.parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
				user_uart_callback, .p_context = NULL, .p_extend =
				&g_uart0_cfg_extend,
#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
		.p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &FSP_NOT_DEFINED,
#endif
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
		.p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &FSP_NOT_DEFINED,
#endif
#undef FSP_NOT_DEFINED
		.rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI0_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
		.rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
		.txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
		.tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI0_ERI,
#else
		.eri_irq = FSP_INVALID_VECTOR,
#endif
		};

/* Instance structure to use this module. */
const uart_instance_t g_uart0 = { .p_ctrl = &g_uart0_ctrl,
		.p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
ethsw_instance_ctrl_t g_ethsw0_ctrl;

const ethsw_cfg_t g_ethsw0_cfg = { .channel = 0,

.specific_tag = ETHSW_SPECIFIC_TAG_ENABLE, .specific_tag_id = 0xE001,

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
				(ether_phy_chip_t) ETHER_PHY_CHIP_VSC8541, .mdio_type =
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
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
				NULL,
#else
                    &FSP_NOT_DEFINED,
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

extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t main_thread_parameters = { .p_context =
		(void*) NULL, };

void main_thread_create(void) {
	/* Increment count so we will know the number of threads created in the FSP Configuration editor. */
	g_fsp_common_thread_count++;

	/* Initialize each kernel object. */

#if 1
	main_thread = xTaskCreateStatic(
#else
                    BaseType_t main_thread_create_err = xTaskCreate(
                    #endif
			main_thread_func, (const char*) "Main Thread", 1024 / 4, // In words, not bytes
			(void*) &main_thread_parameters, //pvParameters
			5,
#if 1
			(StackType_t*) &main_thread_stack,
			(StaticTask_t*) &main_thread_memory
#else
                        & main_thread
                        #endif
			);

#if 1
	if (NULL == main_thread) {
		rtos_startup_err_callback(main_thread, 0);
	}
#else
                    if (pdPASS != main_thread_create_err)
                    {
                        rtos_startup_err_callback(main_thread, 0);
                    }
                    #endif
}
static void main_thread_func(void *pvParameters) {
	/* Initialize common components */
	rtos_startup_common_init();

	/* Initialize each module instance. */

	/* Enter user code for this thread. Pass task handle. */
	main_thread_entry(pvParameters);
}
