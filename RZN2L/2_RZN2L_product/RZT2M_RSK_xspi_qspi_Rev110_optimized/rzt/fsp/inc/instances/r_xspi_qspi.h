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

/*******************************************************************************************************************//**
 * @addtogroup XSPI_QSPI
 * @{
 **********************************************************************************************************************/

#ifndef R_XSPI_QSPI_H
#define R_XSPI_QSPI_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include <string.h>
#include "r_xspi_qspi_cfg.h"
#include "r_spi_flash_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define XSPI_QSPI_CODE_VERSION_MAJOR    (1U) // DEPRECATED
#define XSPI_QSPI_CODE_VERSION_MINOR    (2U) // DEPRECATED

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* QSPI Flash chip select */
typedef enum e_xspi_qspi_chip_select
{
    XSPI_QSPI_CHIP_SELECT_0,           ///< Device connected to Chip-Select 0
    XSPI_QSPI_CHIP_SELECT_1,           ///< Device connected to Chip-Select 1
} xspi_qspi_chip_select_t;

/* QSPI Flash memory size */
typedef enum e_xspi_qspi_memory_size
{
    XSPI_QSPI_MEMORY_SIZE_1MB  = 0x00, ///< Memory size 1MB
    XSPI_QSPI_MEMORY_SIZE_2MB  = 0x01, ///< Memory size 2MB
    XSPI_QSPI_MEMORY_SIZE_4MB  = 0x03, ///< Memory size 4MB
    XSPI_QSPI_MEMORY_SIZE_8MB  = 0x07, ///< Memory size 8MB
    XSPI_QSPI_MEMORY_SIZE_16MB = 0x0F, ///< Memory size 16MB
    XSPI_QSPI_MEMORY_SIZE_32MB = 0x1F, ///< Memory size 32MB
    XSPI_QSPI_MEMORY_SIZE_64MB = 0x3F, ///< Memory size 64MB
} xspi_qspi_memory_size_t;

/* QSPI command to command interval*/
typedef enum e_xspi_qspi_command_interval_clocks
{
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_1,  ///< 1 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_2,  ///< 2 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_3,  ///< 3 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_4,  ///< 4 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_5,  ///< 5 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_6,  ///< 6 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_7,  ///< 7 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_8,  ///< 8 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_9,  ///< 9 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_10, ///< 10 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_11, ///< 11 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_12, ///< 12 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_13, ///< 13 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_14, ///< 14 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_15, ///< 15 interval clocks
    XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_16, ///< 16 interval clocks
} xspi_qspi_command_interval_clocks_t;

/* QSPI chip select de-assertion duration */
typedef enum e_xspi_qspi_cs_pullup_clocks
{
    XSPI_QSPI_CS_PULLUP_CLOCKS_NO_EXTENSION = 0, ///< CS asserting No extension
    XSPI_QSPI_CS_PULLUP_CLOCKS_1,                ///< CS asserting Extend 1 cycle
} xspi_qspi_cs_pullup_clocks_t;

/* QSPI chip select assertion duration */
typedef enum e_xspi_qspi_cs_pulldown_clocks
{
    XSPI_QSPI_CS_PULLDOWN_CLOCKS_NO_EXTENSION = 0, ///< CS negating No extension
    XSPI_QSPI_CS_PULLDOWN_CLOCKS_1,                ///< CS negating Extend 1 cycle
} xspi_qspi_cs_pulldown_clocks_t;

/* Memory mapped timing */
typedef struct st_qspi_timing_setting
{
    xspi_qspi_command_interval_clocks_t command_to_command_interval; ///< Interval between 2 consecutive commands
    xspi_qspi_cs_pullup_clocks_t        cs_pullup_lag;               ///< Duration to de-assert CS line after the last command
    xspi_qspi_cs_pulldown_clocks_t      cs_pulldown_lead;            ///< Duration to assert CS line before the first command
} xspi_qspi_timing_setting_t;

/* Prefetch function settings */
typedef enum e_xspi_qspi_prefetch_function
{
    XSPI_QSPI_PREFETCH_FUNCTION_DISABLE = 0x00, ///< Prefetch function disable
    XSPI_QSPI_PREFETCH_FUNCTION_ENABLE  = 0x01, ///< Prefetch function enable
} xspi_qspi_prefetch_function_t;

/* Extended configuration. */
typedef struct st_xspi_qspi_extended_cfg
{
    uint8_t                            unit;              ///< Unit number of xSPI
    xspi_qspi_chip_select_t            chip_select;       ///< Device number to be used for memory device
    xspi_qspi_memory_size_t            memory_size;       ///< Size of memory device
    xspi_qspi_timing_setting_t const * p_timing_settings; ///< Memory mapped timing settings
    xspi_qspi_prefetch_function_t      prefetch_en;       ///< Prefetch function settings
} xspi_qspi_extended_cfg_t;

/** Instance control block. DO NOT INITIALIZE.  Initialization occurs when @ref spi_flash_api_t::open is called */
typedef struct st_xspi_qspi_instance_ctrl
{
    spi_flash_cfg_t const * p_cfg;            // Pointer to initial configuration
    spi_flash_data_lines_t  data_lines;       // Data lines
    uint32_t                total_size_bytes; // Total size of the flash in bytes
    uint32_t                open;             // Whether or not driver is open
    R_XSPI0_Type          * p_reg;            // Base register for this channel
    spi_flash_protocol_t    spi_protocol;     // Current SPI protocol selected
} xspi_qspi_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const spi_flash_api_t g_spi_flash_on_xspi_qspi;

/** @endcond */

fsp_err_t R_XSPI_QSPI_Open(spi_flash_ctrl_t * p_ctrl, spi_flash_cfg_t const * const p_cfg);
fsp_err_t R_XSPI_QSPI_Close(spi_flash_ctrl_t * p_ctrl);
fsp_err_t R_XSPI_QSPI_DirectWrite(spi_flash_ctrl_t    * p_ctrl,
                                  uint8_t const * const p_src,
                                  uint32_t const        bytes,
                                  bool const            read_after_write);
fsp_err_t R_XSPI_QSPI_DirectRead(spi_flash_ctrl_t * p_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t R_XSPI_QSPI_SpiProtocolSet(spi_flash_ctrl_t * p_ctrl, spi_flash_protocol_t spi_protocol);
fsp_err_t R_XSPI_QSPI_XipEnter(spi_flash_ctrl_t * p_ctrl);
fsp_err_t R_XSPI_QSPI_XipExit(spi_flash_ctrl_t * p_ctrl);
fsp_err_t R_XSPI_QSPI_Write(spi_flash_ctrl_t    * p_ctrl,
                            uint8_t const * const p_src,
                            uint8_t * const       p_dest,
                            uint32_t              byte_count);
fsp_err_t R_XSPI_QSPI_Erase(spi_flash_ctrl_t * p_ctrl, uint8_t * const p_device_address, uint32_t byte_count);
fsp_err_t R_XSPI_QSPI_StatusGet(spi_flash_ctrl_t * p_ctrl, spi_flash_status_t * const p_status);
fsp_err_t R_XSPI_QSPI_BankSet(spi_flash_ctrl_t * p_ctrl, uint32_t bank);
fsp_err_t R_XSPI_QSPI_VersionGet(fsp_version_t * const p_version);
fsp_err_t R_XSPI_QSPI_DirectTransfer(spi_flash_ctrl_t                  * p_ctrl,
                                     spi_flash_direct_transfer_t * const p_transfer,
                                     spi_flash_direct_transfer_dir_t     direction);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup XSPI_QSPI)
 **********************************************************************************************************************/
