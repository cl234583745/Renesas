/* generated HAL source file - do not edit */
#include "hal_data.h"

gpt_instance_ctrl_t g_timer0_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer0_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_UDF)
    .trough_irq          = VECTOR_NUMBER_GPT0_UDF,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .interrupt_skip_source_ext1 = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count_ext1  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_source_ext2 = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count_ext2  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_func_ovf    = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_unf    = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_adc_a  = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_adc_b  = GPT_INTERRUPT_SKIP_SELECT_NONE,
};
#endif
const gpt_extended_cfg_t g_timer0_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
#if (0 == (0))
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE),
          .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE),
#else
    .count_up_source     = (gpt_source_t) ((GPT_PHASE_COUNTING_MODE_1_UP | (GPT_PHASE_COUNTING_MODE_1_DN << 16)) & 0x000FFFFU),
    .count_down_source   = (gpt_source_t) (((GPT_PHASE_COUNTING_MODE_1_UP | (GPT_PHASE_COUNTING_MODE_1_DN << 16)) & 0xFFFF0000U) >> 16),
#endif
          .capture_a_source = (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_CCMPA)
    .capture_a_irq       = VECTOR_NUMBER_GPT0_CCMPA,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_CCMPB)
    .capture_b_irq       = VECTOR_NUMBER_GPT0_CCMPB,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer0_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
          .dead_time_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_DTE)
    .dead_time_irq       = VECTOR_NUMBER_GPT0_DTE,
#else
          .dead_time_irq = FSP_INVALID_VECTOR,
#endif
          .icds = 0, };
const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_ONE_SHOT,
/* Actual period: 0.001 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x61a80,
  .duty_cycle_counts = 0x30d40, .source_div = (timer_source_div_t) 0, .channel = GPT_CHANNEL_UNIT0_0, .p_callback =
          g_timer0CB,
  .p_context = NULL, .p_extend = &g_timer0_extend, .cycle_end_ipl = (12),
#if defined(VECTOR_NUMBER_GPT0_OVF)
    .cycle_end_irq       = VECTOR_NUMBER_GPT0_OVF,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
dmac_instance_ctrl_t g_transfer1_ctrl;

transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .mode =
          TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL, .p_src = (void const*) NULL, .length = 0, .src_size = TRANSFER_SIZE_1_BYTE, .dest_size =
          TRANSFER_SIZE_1_BYTE,
  .p_next1_src = (void const*) NULL, .p_next1_dest = (void*) NULL, .next1_length = 1 };

#ifndef NULL
extern dmac_link_cfg_t NULL;
#endif

const dmac_extended_cfg_t g_transfer1_extend =
{ .unit = 0, .channel = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT1)
    .dmac_int_irq         = VECTOR_NUMBER_DMAC0_INT1,
#else
  .dmac_int_irq = FSP_INVALID_VECTOR,
#endif
  .dmac_int_ipl = (12),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_SCI0_RXI,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (1) << 1 | (0) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_SOURCE_MODULE,

  .next_register_operation = DMAC_REGISTER_SELECT_REVERSE_DISABLE,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
#if 0 == 0
  .channel_scheduling = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL0_7,
#else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL0_7,
#endif
  .p_callback = g_uart0_rx_transfer_callback,
  .p_context = NULL, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info, .p_extend = &g_transfer1_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dmac };
dmac_instance_ctrl_t g_transfer0_ctrl;

transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .mode =
          TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL, .p_src = (void const*) NULL, .length = 0, .src_size = TRANSFER_SIZE_1_BYTE, .dest_size =
          TRANSFER_SIZE_1_BYTE,
  .p_next1_src = (void const*) NULL, .p_next1_dest = (void*) NULL, .next1_length = 1 };

#ifndef NULL
extern dmac_link_cfg_t NULL;
#endif

const dmac_extended_cfg_t g_transfer0_extend =
{ .unit = 0, .channel = 0,
#if defined(VECTOR_NUMBER_DMAC0_INT0)
    .dmac_int_irq         = VECTOR_NUMBER_DMAC0_INT0,
#else
  .dmac_int_irq = FSP_INVALID_VECTOR,
#endif
  .dmac_int_ipl = (12),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_SCI0_TXI,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (1) << 1 | (0) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_DESTINATION_MODULE,

  .next_register_operation = DMAC_REGISTER_SELECT_REVERSE_DISABLE,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
#if 0 == 0
  .channel_scheduling = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL0_7,
#else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL0_7,
#endif
  .p_callback = g_uart0_tx_transfer_callback,
  .p_context = NULL, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
sci_uart_instance_ctrl_t g_uart0_ctrl;

#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED) != (g_transfer0)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
extern void sci_uart_tx_dmac_callback(sci_uart_instance_ctrl_t *p_instance_ctrl);

void g_uart0_tx_transfer_callback(transfer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED (p_args);
    sci_uart_tx_dmac_callback (&g_uart0_ctrl);
}
#endif

#if (FSP_NOT_DEFINED) != (g_transfer1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
extern void sci_uart_rx_dmac_callback(sci_uart_instance_ctrl_t *p_instance_ctrl);

void g_uart0_rx_transfer_callback(transfer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED (p_args);
    sci_uart_rx_dmac_callback (&g_uart0_ctrl);
}
#endif
#undef FSP_NOT_DEFINED

sci_baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 0.160% error. */.baudrate_bits_b.abcse = 0,
          .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 51, .baudrate_bits_b.mddr =
                  (uint8_t) 256,
          .baudrate_bits_b.brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart0_baud_setting,
#if 1
  .clock_source = SCI_UART_CLOCK_SOURCE_SCI0ASYNCCLK,
#else
                .clock_source           = SCI_UART_CLOCK_SOURCE_PCLKM,
#endif
  .flow_control = SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH, .assertion_time = 1, .negation_time =
            1, }, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          user_uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend, .p_transfer_tx = g_uart0_P_TRANSFER_TX, .p_transfer_rx =
          g_uart0_P_TRANSFER_RX,
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
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
void g_hal_init(void)
{
    g_common_init ();
}
