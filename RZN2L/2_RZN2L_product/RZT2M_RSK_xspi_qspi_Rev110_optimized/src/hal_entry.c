#include "hal_data.h"
#include "sio_char.h"
#include <stdio.h>

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event)
BSP_PLACE_IN_SECTION(".warm_start");
FSP_CPP_FOOTER

void handle_error(fsp_err_t err);
void user_uart_callback (uart_callback_args_t * p_args);

uint8_t  g_out_of_band_received[TRANSFER_LENGTH];
volatile uint32_t g_transfer_complete = 0;
volatile uint32_t g_receive_complete  = 0;
uint32_t g_out_of_band_index = 0;

char gbuff[16];



void qspi_set_Quad_Mode_enable(void);
void qspi_set_Quad_Mode_disable(void);

#define QSPI_DEVICE_START_ADDRESS (0x60000000)
#define DATA_SIZE            (64)
#define WRITE_ADDRESS        (0x10000)
#define WRITE_ADDRESS1       (0x10234)
uint8_t g_src[DATA_SIZE];

/*******************************************************************************************************************//**
 * @brief  xSPI_QSPI Application
 *
 * After deleting the data in the QSPI flash memory, write and read the data.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
    /* Open the transfer instance with initial configuration. */
    fsp_err_t err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    handle_error(err);

    __asm volatile ("cpsie i");

    printf("\n\n\nHello World\n");
    printf("========================FLASH===========================\n");
    printf("0x6000 0000*********************************************\n");
    printf("0x6000 1000*************Frameware***********************\n");
    printf("0x6000 2000*********************************************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("0x6001 0000*************Test Addr***********************\n");
    printf("0x6001 1000*************Test Addr***********************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("********************************************************\n");
    printf("========================================================\n");

    /* Open the QSPI instance */
    err = R_XSPI_QSPI_Open(&g_qspi0_ctrl, &g_qspi0_cfg);
    handle_error(err);
    printf("[QSPI_Open]:successed!\n");

    /* Enter QPI mode */
    qspi_set_Quad_Mode_enable();
    printf("[Quad_Mode_enable]:successed!\n");

    printf("--------------------------------------------------------\n");
    printf("Power On Read!\n");
    printf("[Pointer Read]:0x4000 0000 + 0x%x\n", WRITE_ADDRESS1);
    uint8_t dest_Data0 = *(uint8_t *)(QSPI_DEVICE_START_ADDRESS - 0x20000000 + WRITE_ADDRESS1);
    printf("%02X\n", dest_Data0);

    printf("[Pointer Read]:0x4000 0000 + 0x%04X\n", WRITE_ADDRESS);
    for(uint32_t i0 = 0; i0 < DATA_SIZE; i0++)
    {
        uint8_t dest_Data1 = *(uint8_t *)(QSPI_DEVICE_START_ADDRESS - 0x20000000 + WRITE_ADDRESS + i0);
        printf("%02X ", dest_Data1);
    }printf("\n");
    printf("--------------------------------------------------------\n");


    /*---------- Chip Erase ----------*/
    //err = R_XSPI_QSPI_Erase(&g_qspi0_ctrl, (uint8_t *)QSPI_DEVICE_START_ADDRESS, SPI_FLASH_ERASE_SIZE_CHIP_ERASE);
    //handle_error(err);

    /*---------- Sector Erase ----------*/
    err = R_XSPI_QSPI_Erase(&g_qspi0_ctrl, (uint8_t *)QSPI_DEVICE_START_ADDRESS+WRITE_ADDRESS, 4096);
    handle_error(err);

    /* Wait for status register to update. */
    spi_flash_status_t status_erase;
    do
    {
        R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MICROSECONDS);
        (void) R_XSPI_QSPI_StatusGet(&g_qspi0_ctrl, &status_erase);
    } while (true == status_erase.write_in_progress);
    printf("[Erase] successed!: addr:0x%x, len:4096\n", QSPI_DEVICE_START_ADDRESS+WRITE_ADDRESS);

    /*---------- Direct transfer (WRITE) ----------*/
    spi_flash_direct_transfer_t qspi_test_direct_transfer_write_enable = {0};
    qspi_test_direct_transfer_write_enable.command        = 0x06,  /* Write Enable */
    qspi_test_direct_transfer_write_enable.command_length = 1U,
    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &qspi_test_direct_transfer_write_enable, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    printf("[DirectTransfe WRITE] successed!: cmd:0x06\n");

    spi_flash_direct_transfer_t qspi_test_direct_transfer_write =
    {
        .command        = 0x02U,  /* 4PP instruction */
        .address        = WRITE_ADDRESS1,
        .data           = 0x12U,
        .command_length = 1U,
        .address_length = 4U,
        .data_length    = 1U,
        .dummy_cycles   = 0U
    };
    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &qspi_test_direct_transfer_write, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);

    /* Wait for status register to update. */
    spi_flash_status_t status_write;
    do
    {
        (void) R_XSPI_QSPI_StatusGet(&g_qspi0_ctrl, &status_write);
    } while (true == status_write.write_in_progress);
    printf("[DirectTransfe WRITE] successed!: cmd:0x02, addr:0x%x, data:0x12\n", WRITE_ADDRESS1);

    /*---------- Direct transfer (READ) ----------*/
    spi_flash_direct_transfer_t qspi_test_direct_transfer_read =
    {
        .command        = 0xEBU,  /* 4READ instruction */
        .address        = WRITE_ADDRESS1,
        .data           = 0U,
        .command_length = 1U,
        .address_length = 4U,
        .data_length    = 1U,
         /* 4READ instruction needs (2+4) dummy cycles */
        .dummy_cycles   = 6U
    };
    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &qspi_test_direct_transfer_read, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    printf("[DirectTransfe READ] successed!: cmd:0xEB, addr:0x%x, data:0x%lx\n", WRITE_ADDRESS1, qspi_test_direct_transfer_read.data);

    /*---------- Verify Write data = Read data? ----------*/
    if (qspi_test_direct_transfer_read.data != qspi_test_direct_transfer_write.data)
    {
        /* Exit QPI mode */
        qspi_set_Quad_Mode_disable();
        printf("[ERR-1]! line:%d\n", __LINE__);
        while(1);
    }
    printf("[Verify-1] successed! line:%d\n", __LINE__);
    printf("--------------------------------------------------------\n");

    /*---------- Read data using memory-mapping mode ----------*/
    uint32_t read_buffer = *(uint8_t *)(QSPI_DEVICE_START_ADDRESS - 0x20000000 +  qspi_test_direct_transfer_write.address); //xSPI0_CS0 memory (Mirror area)
    printf("[Read] successed!: addr:0x6000 0000-0x2000 0000+0x%lx, read_buffer:0x%lx\n", qspi_test_direct_transfer_write.address, read_buffer);
    /*---------- Verify Write data = Read data? ----------*/
    if (read_buffer != qspi_test_direct_transfer_write.data)
    {
        /* Exit QPI mode */
        qspi_set_Quad_Mode_disable();
        printf("[ERR-2]! line:%d\n", __LINE__);
        while(1);
    }
    printf("[Verify-2] successed! line:%d\n", __LINE__);
    printf("--------------------------------------------------------\n");


    printf("[QSPI_Write] addr:0x6000 0000-0x2000 0000+0x%x:\n", WRITE_ADDRESS);
    /*---------- Initialize Write Data ----------*/
    for(uint32_t i = 0; i < DATA_SIZE; i++)
    {
        g_src[i] = (uint8_t)i;
        printf("%02X ", g_src[i]);
    }printf("\n");

    /*---------- Write Data via write API ----------*/
    uint8_t * dest = (uint8_t *)(QSPI_DEVICE_START_ADDRESS - 0x20000000 + WRITE_ADDRESS);  //xSPI0_CS0 memory, non-cache area use.
    err = R_XSPI_QSPI_Write(&g_qspi0_ctrl, g_src, dest, DATA_SIZE);
    handle_error(err);

    /* Wait for status register to update. */
    spi_flash_status_t status_write2;
    do
    {
        R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MICROSECONDS);
        (void) R_XSPI_QSPI_StatusGet(&g_qspi0_ctrl, &status_write2);
    } while (true == status_write.write_in_progress);
    printf("[QSPI_Write] successed!\n");


    printf("[QSPI_Read] addr:0x6000 0000-0x2000 0000+0x%x:\n", WRITE_ADDRESS);
    /*---------- Verify Write data = Read data? ----------*/
    for(uint32_t i = 0; i < DATA_SIZE; i++)
    {
        uint8_t src_data = g_src[i];
        uint8_t dest_Data = *(uint8_t *)(QSPI_DEVICE_START_ADDRESS - 0x20000000 + WRITE_ADDRESS + i);
        printf("%02X ", dest_Data);
        if(src_data != dest_Data)
        {
            /* Exit QPI mode */
            qspi_set_Quad_Mode_disable();
            printf("[ERR-3]! line:%d\n", __LINE__);
            while(1);
        }
    }printf("\n");

    printf("[Verify-3] successed! line:%d\n", __LINE__);
    printf("--------------------------------------------------------\n");

    /* Exit QPI mode */
    qspi_set_Quad_Mode_disable();

    while(1)
    {
        printf("Qspi demo successed! running!\n");
        R_BSP_SoftwareDelay (5000, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

void handle_error(fsp_err_t err)
{
    FSP_PARAMETER_NOT_USED(err);
}

void qspi_set_Quad_Mode_enable(void)
{
    /* For following settings, temporary set protocol 1S-1S-1S */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_1S_1S_1S);

    /*--------------- Read status register(RDSR) (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDSR = {0};
    direct_command_RDSR.command        = 0x05U;
    direct_command_RDSR.command_length = 1U;
    direct_command_RDSR.data_length    = 1U;

    fsp_err_t err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDSR, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    uint32_t read_status = direct_command_RDSR.data;
    FSP_PARAMETER_NOT_USED(read_status);
    /*----------------------------------------------------------*/

    /*---------------- Read configure register(RDCR) (15h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDCR = {0};
    direct_command_RDCR.command        = 0x15U;
    direct_command_RDCR.command_length = 1U;
    direct_command_RDCR.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDCR, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    uint32_t read_configure = direct_command_RDCR.data;
    FSP_PARAMETER_NOT_USED(read_configure);
    /*--------------------------------------------------------------*/

    /*----------------- Enter 4byte address mode(EN4B) (0xB7) ----------------*/
    spi_flash_direct_transfer_t direct_command_EN4B = {0};
    direct_command_EN4B.command        = 0xB7U;
    direct_command_EN4B.command_length = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_EN4B, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*--------------------------------------------------------------*/

    /*--------------- Enable QPI mode(EQIO) (35h)---------------*/
    spi_flash_direct_transfer_t direct_command_EQIO = {0};
    direct_command_EQIO.command        = 0x35U;
    direct_command_EQIO.command_length = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_EQIO, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*----------------------------------------------------------*/

    /* 4S-4S-4S protocol settings. */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_4S_4S_4S);

    /*--------------- Read status register(RDSR) (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDSR_2 = {0};
    direct_command_RDSR_2.command        = 0x05U;
    direct_command_RDSR_2.command_length = 1U;
    direct_command_RDSR_2.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDSR_2, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    read_status = direct_command_RDSR_2.data;
    /*----------------------------------------------------------*/

    /*---------------- Read configure register(RDCR) (15h) ---------------*/
    spi_flash_direct_transfer_t direct_command_RDCR_2 = {0};
    direct_command_RDCR_2.command        = 0x15U;
    direct_command_RDCR_2.command_length = 1U;
    direct_command_RDCR_2.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RDCR_2, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    read_configure = direct_command_RDCR_2.data;
    /*--------------------------------------------------------------*/
}

void qspi_set_Quad_Mode_disable(void)
{
    /*--------------- Reset QPI(RSTQIO) (F5h)---------------*/
    spi_flash_direct_transfer_t direct_command_RSTQIO = {0};
    direct_command_RSTQIO.command        = 0xF5U;
    direct_command_RSTQIO.command_length = 1U;

    fsp_err_t err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command_RSTQIO, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    handle_error(err);
    /*----------------------------------------------------------*/

    /* 1S-1S-1S protocol settings. */
    R_XSPI_QSPI_SpiProtocolSet(&g_qspi0_ctrl, SPI_FLASH_PROTOCOL_1S_1S_1S);

    /*--------------- Read status register (05h) ---------------*/
    spi_flash_direct_transfer_t direct_command = {0};
    direct_command.command        = 0x05U;
    direct_command.command_length = 1U;
    direct_command.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    /*----------------------------------------------------------*/

    /*---------------- Read configure register (15h) ---------------*/
    direct_command.command        = 0x15U;
    direct_command.command_length = 1U;
    direct_command.data_length    = 1U;

    err = R_XSPI_QSPI_DirectTransfer(&g_qspi0_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    handle_error(err);
    /*--------------------------------------------------------------*/

}

void user_uart_callback (uart_callback_args_t * p_args)
{
    /* Handle the UART event */
    switch (p_args->event)
    {
        /* Received a character */
        case UART_EVENT_RX_CHAR:
        {
            /* Only put the next character in the receive buffer if there is space for it */
            if (sizeof(g_out_of_band_received) > g_out_of_band_index)
            {
                /* Write either the next one or two bytes depending on the receive data size */
                if ((UART_DATA_BITS_7 == g_uart0_cfg.data_bits) || (UART_DATA_BITS_8 == g_uart0_cfg.data_bits))
                {
                    g_out_of_band_received[g_out_of_band_index++] = (uint8_t) p_args->data;
                }
                else
                {
                    uint16_t * p_dest = (uint16_t *) &g_out_of_band_received[g_out_of_band_index];
                    *p_dest              = (uint16_t) p_args->data;
                    g_out_of_band_index += 2;
                }
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete = 1;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete = 1;
            break;
        }
        default:
        {
        }
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
