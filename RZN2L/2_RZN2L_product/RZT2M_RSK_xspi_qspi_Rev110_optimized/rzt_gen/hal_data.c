/* generated HAL source file - do not edit */
#include "hal_data.h"
sci_uart_instance_ctrl_t g_uart0_ctrl;

baud_setting_t g_uart0_baud_setting =
{
/* Baud rate calculated with 0.160% error. */.abcse = 0,
  .abcs = 0, .bgdm = 1, .cks = 0, .brr = 51, .mddr = (uint8_t) 256, .brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT,
  .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE,
  .noise_cancel = SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX,
  .p_baud_setting = &g_uart0_baud_setting,
  .uart_mode = UART_MODE_RS232,
  .ctsrts_en = SCI_UART_CTSRTS_RTS_OUTPUT,
#if 0
                .flow_control_pin       = BSP_IO_PORT_00_PIN_00,
#else
  .flow_control_pin = (bsp_io_port_pin_t) (0xFFFFU),
#endif
  .sync_bypass = SCI_UART_SYNCHRONIZER_NOT_BYPASS, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          user_uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
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
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
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
xspi_qspi_instance_ctrl_t g_qspi0_ctrl;

static const spi_flash_erase_command_t g_qspi0_erase_command_list[] =
{
#if 4096 > 0
  { .command = 0x20, .size = 4096 },
#endif
#if 32768 > 0
  { .command = 0x52, .size = 32768 },
#endif
#if 65536 > 0
  { .command = 0xD8, .size = 65536 },
#endif
#if 0xC7 > 0
  { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
#endif
        };

static xspi_qspi_timing_setting_t g_qspi0_timing_settings =
{ .command_to_command_interval = XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_7, .cs_pullup_lag =
          XSPI_QSPI_CS_PULLUP_CLOCKS_NO_EXTENSION,
  .cs_pulldown_lead = XSPI_QSPI_CS_PULLDOWN_CLOCKS_NO_EXTENSION };

static const xspi_qspi_extended_cfg_t g_qspi0_extended_cfg =
{ .unit = 0, .chip_select = XSPI_QSPI_CHIP_SELECT_0, .memory_size = XSPI_QSPI_MEMORY_SIZE_64MB, .p_timing_settings =
          &g_qspi0_timing_settings,
  .prefetch_en = XSPI_QSPI_PREFETCH_FUNCTION_DISABLE, };
const spi_flash_cfg_t g_qspi0_cfg =
{ .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_4,
  .dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_6,
  .read_command = 0xEB,
  .page_program_command = 0x02,
  .write_enable_command = 0x06,
  .status_command = 0x05,
  .write_status_bit = 0,
  .xip_enter_command = 0x20,
  .xip_exit_command = 0xFF,
  .p_erase_command_list = &g_qspi0_erase_command_list[0],
  .erase_command_list_length = sizeof(g_qspi0_erase_command_list) / sizeof(g_qspi0_erase_command_list[0]),
  .p_extend = &g_qspi0_extended_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_qspi0 =
{ .p_ctrl = &g_qspi0_ctrl, .p_cfg = &g_qspi0_cfg, .p_api = &g_spi_flash_on_xspi_qspi, };
void g_hal_init(void)
{
    g_common_init ();
}
