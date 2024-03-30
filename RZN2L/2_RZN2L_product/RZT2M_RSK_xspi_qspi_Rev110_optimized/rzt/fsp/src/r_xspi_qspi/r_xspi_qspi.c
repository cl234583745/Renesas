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

#include "bsp_api.h"

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_xspi_qspi.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define XSPI_QSPI_PRV_OPEN                            (0x51535049)

/* External address space xSPI0 : 0x60000000 - 0x67FFFFFF,
 * External address space xSPI1 : 0x68000000 - 0x6FFFFFFF */
#define XSPI_QSPI_DEVICE_START_ADDRESS                (0x60000000)
#define XSPI_QSPI_DEVICE_XSPI0_1_ADDRESS_DELTA        (0x8000000)

/* External address space xSPI0 CS0 : 0x60000000 - 0x63FFFFFF,
 * External address space xSPI0 CS1 : 0x64000000 - 0x67FFFFFF */
#define XSPI_QSPI_DEVICE_CS0_1_ADDRESS_DELTA          (0x4000000)

/* Mirror area of external address space xSPI0 : 0x40000000 - 0x47FFFFFF,
 * Mirror area of external address space xSPI1 : 0x48000000 - 0x4FFFFFFF */
#define XSPI_QSPI_DEVICE_START_MIRROR_ADDRESS         (0x40000000)

#define XSPI_QSPI_PRV_LIOCFGCS_PRTMD_OFFSET           (0U)
#define XSPI_QSPI_PRV_LIOCFGCS_PRTMD_VALUE_MASK       (0x3FFU)
#define XSPI_QSPI_PRV_LIOCFGCS_PRTMD_MASK             (0x3FFU << XSPI_QSPI_PRV_LIOCFGCS_PRTMD_OFFSET)
#define XSPI_QSPI_PRV_LIOCFGCS_CSMIN_OFFSET           (16U)
#define XSPI_QSPI_PRV_LIOCFGCS_CSMIN_VALUE_MASK       (0x0FU)
#define XSPI_QSPI_PRV_LIOCFGCS_CSASTEX_OFFSET         (20U)
#define XSPI_QSPI_PRV_LIOCFGCS_CSASTEX_VALUE_MASK     (0x01U)
#define XSPI_QSPI_PRV_LIOCFGCS_CSENGEX_OFFSET         (21U)
#define XSPI_QSPI_PRV_LIOCFGCS_CSENGEX_VALUE_MASK     (0x01U)

#define XSPI_QSPI_PRV_CMCFG0_FFMT_OFFSET              (0U)
#define XSPI_QSPI_PRV_CMCFG0_ADDSIZE_OFFSET           (2U)
#define XSPI_QSPI_PRV_CMCFG0_ADDSIZE_VALUE_MASK       (0x03U)
#define XSPI_QSPI_PRV_CMCFG1_RDCMD_UPPER_OFFSET       (8U)
#define XSPI_QSPI_PRV_CMCFG1_RDCMD_VALUE_MASK         (0xFFU)
#define XSPI_QSPI_PRV_CMCFG1_RDLATE_OFFSET            (16U)
#define XSPI_QSPI_PRV_CMCFG1_RDLATE_VALUE_MASK        (0x1FU)

#define XSPI_QSPI_PRV_CMCFG2_WRCMD_UPPER_OFFSET       (8U)
#define XSPI_QSPI_PRV_CMCFG1_WRCMD_VALUE_MASK         (0xFFU)
#define XSPI_QSPI_PRV_CMCFG2_WRLATE_OFFSET            (16U)

#define XSPI_QSPI_PRV_BMCFG_WRMD_OFFSET               (0U)
#define XSPI_QSPI_PRV_BMCFG_MWRCOMB_OFFSET            (7U)
#define XSPI_QSPI_PRV_BMCFG_MWRSIZE_OFFSET            (8U)
#define XSPI_QSPI_PRV_BMCFG_PREEN_OFFSET              (16U)
#define XSPI_QSPI_PRV_BMCFG_PREEN_VALUE_MASK          (0x01U)

#define XSPI_QSPI_PRV_BMCTL_DEFAULT_VALUE             (0x000000FFUL)

#define XSPI_QSPI_PRV_BMCTL_CSNACC_R_ENABLE           (0x01U)
#define XSPI_QSPI_PRV_BMCTL_CSNACC_R_W_ENABLE         (0x03U)

#define XSPI_QSPI_PRV_CDTBUF_CMDSIZE_OFFSET           (0U)
#define XSPI_QSPI_PRV_CDTBUF_CMDSIZE_VALUE_MASK       (0x03U)
#define XSPI_QSPI_PRV_CDTBUF_ADDSIZE_OFFSET           (2U)
#define XSPI_QSPI_PRV_CDTBUF_ADDSIZE_VALUE_MASK       (0x07U)
#define XSPI_QSPI_PRV_CDTBUF_DATASIZE_OFFSET          (5U)
#define XSPI_QSPI_PRV_CDTBUF_DATASIZE_VALUE_MASK      (0x0FU)
#define XSPI_QSPI_PRV_CDTBUF_LATE_OFFSET              (9U)
#define XSPI_QSPI_PRV_CDTBUF_LATE_VALUE_MASK          (0x1FU)
#define XSPI_QSPI_PRV_CDTBUF_TRTYPE_OFFSET            (15U)
#define XSPI_QSPI_PRV_CDTBUF_TRTYPE_VALUE_MASK        (0x01U)
#define XSPI_QSPI_PRV_CDTBUF_CMD_UPPER_OFFSET         (24U)
#define XSPI_QSPI_PRV_CDTBUF_CMD_VALUE_MASK           (0xFFU)

#define XSPI_QSPI_PRV_COMSTT_MEMACC_OFFSET            (0U)
#define XSPI_QSPI_PRV_COMSTT_MEMACC_VALUE_MASK        (0x01U)
#define XSPI_QSPI_PRV_COMSTT_WRBUFNE_OFFSET           (6U)
#define XSPI_QSPI_PRV_COMSTT_WRBUFNE_VALUE_MASK       (0x01U)

#define XSPI_QSPI_PRV_INTC_CMDCMPC_OFFSET             (0U)

#define XSPI_QSPI_PRV_DEVICE_WRITE_STATUS_BIT_MASK    (1U)

#define XSPI_QSPI_PRV_MAX_COMBINE_SIZE                (64U)

#define XSPI_QSPI_PRV_WORD_ACCESS_SIZE                (4U)
#define XSPI_QSPI_PRV_HALF_WORD_ACCESS_SIZE           (2U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Number of address bytes in 4 byte address mode. */
#define QSPI_4_BYTE_ADDRESS                           (4U)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void      r_xspi_qspi_write_enable(xspi_qspi_instance_ctrl_t * p_instance_ctrl);
static bool      r_xspi_qspi_status_sub(xspi_qspi_instance_ctrl_t * p_instance_ctrl);
static fsp_err_t r_xspi_qspi_xip(xspi_qspi_instance_ctrl_t * p_instance_ctrl, uint8_t code, bool enter_mode);
static void      r_xspi_qspi_direct_transfer(xspi_qspi_instance_ctrl_t         * p_instance_ctrl,
                                             spi_flash_direct_transfer_t * const p_transfer,
                                             spi_flash_direct_transfer_dir_t     direction);

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_xspi_qspi_param_checking_dcom(xspi_qspi_instance_ctrl_t * p_instance_ctrl);
static fsp_err_t r_xspi_qspi_program_param_check(xspi_qspi_instance_ctrl_t * p_instance_ctrl,
                                                 uint8_t const * const       p_src,
                                                 uint8_t * const             p_dest,
                                                 uint32_t                    byte_count);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Version data structure used by error logger macro. */
static const fsp_version_t g_xspi_qspi_version =
{
    .api_version_minor  = SPI_FLASH_API_VERSION_MINOR,
    .api_version_major  = SPI_FLASH_API_VERSION_MAJOR,
    .code_version_major = XSPI_QSPI_CODE_VERSION_MAJOR,
    .code_version_minor = XSPI_QSPI_CODE_VERSION_MINOR
};

/*******************************************************************************************************************//**
 * @addtogroup XSPI_QSPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

const spi_flash_api_t g_spi_flash_on_xspi_qspi =
{
    .open           = R_XSPI_QSPI_Open,
    .directWrite    = R_XSPI_QSPI_DirectWrite,
    .directRead     = R_XSPI_QSPI_DirectRead,
    .spiProtocolSet = R_XSPI_QSPI_SpiProtocolSet,
    .write          = R_XSPI_QSPI_Write,
    .erase          = R_XSPI_QSPI_Erase,
    .statusGet      = R_XSPI_QSPI_StatusGet,
    .xipEnter       = R_XSPI_QSPI_XipEnter,
    .xipExit        = R_XSPI_QSPI_XipExit,
    .bankSet        = R_XSPI_QSPI_BankSet,
    .close          = R_XSPI_QSPI_Close,
    .versionGet     = R_XSPI_QSPI_VersionGet,
    .directTransfer = R_XSPI_QSPI_DirectTransfer,
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Open the QSPI driver module. After the driver is open, the QSPI can be accessed like internal flash memory starting
 * at address 0x60000000 or 0x40000000.
 *
 * Implements @ref spi_flash_api_t::open.
 *
 * Example:
 * @snippet r_xspi_qspi_example.c R_XSPI_QSPI_Open
 *
 * @retval FSP_SUCCESS                    Configuration was successful.
 * @retval FSP_ERR_ASSERTION              The parameter p_instance_ctrl or p_cfg is NULL.
 * @retval FSP_ERR_ALREADY_OPEN           Driver has already been opened with the same p_instance_ctrl.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT The requested channel does not exist on this MCU.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_Open (spi_flash_ctrl_t * p_ctrl, spi_flash_cfg_t const * const p_cfg)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);

    /* Make sure this unit exists. */
    FSP_ERROR_RETURN(BSP_FEATURE_XSPI_CHANNELS & (1U << ((xspi_qspi_extended_cfg_t *) p_cfg->p_extend)->unit),
                     FSP_ERR_IP_CHANNEL_NOT_PRESENT);
#endif

    xspi_qspi_extended_cfg_t * p_cfg_extend = (xspi_qspi_extended_cfg_t *) p_cfg->p_extend;

    /* Enable clock to the QSPI block */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_XSPI, p_cfg_extend->unit);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    uint32_t base_address = (uint32_t) R_XSPI0 + (p_cfg_extend->unit * ((uint32_t) R_XSPI1 - (uint32_t) R_XSPI0));
    p_instance_ctrl->p_reg = (R_XSPI0_Type *) base_address;

    /* Initialize control block. */
    p_instance_ctrl->p_cfg = p_cfg;

    /* xSPI configuration (see RZ microprocessor User's Manual section "Flow of Configuration"). */
    p_instance_ctrl->p_reg->LIOCFGCS[p_cfg_extend->chip_select] = (p_cfg->spi_protocol) <<
                                                                  XSPI_QSPI_PRV_LIOCFGCS_PRTMD_OFFSET;
    p_instance_ctrl->spi_protocol = p_cfg->spi_protocol;

    /* Set xSPI CSn slave memory size. */
    if (XSPI_QSPI_CHIP_SELECT_0 == p_cfg_extend->chip_select)
    {
        p_instance_ctrl->p_reg->CSSCTL_b.CS0SIZE = p_cfg_extend->memory_size;
    }
    else
    {
        p_instance_ctrl->p_reg->CSSCTL_b.CS1SIZE = p_cfg_extend->memory_size;
    }

    p_instance_ctrl->p_reg->WRAPCFG = 0x0000;

    p_instance_ctrl->p_reg->LIOCFGCS[p_cfg_extend->chip_select] =
        ((p_cfg->spi_protocol & XSPI_QSPI_PRV_LIOCFGCS_PRTMD_VALUE_MASK) << XSPI_QSPI_PRV_LIOCFGCS_PRTMD_OFFSET) |
        ((p_cfg_extend->p_timing_settings->command_to_command_interval &
          XSPI_QSPI_PRV_LIOCFGCS_CSMIN_VALUE_MASK) << XSPI_QSPI_PRV_LIOCFGCS_CSMIN_OFFSET) |
        ((p_cfg_extend->p_timing_settings->cs_pulldown_lead & XSPI_QSPI_PRV_LIOCFGCS_CSASTEX_VALUE_MASK) <<
            XSPI_QSPI_PRV_LIOCFGCS_CSASTEX_OFFSET) |
        ((p_cfg_extend->p_timing_settings->cs_pullup_lag & XSPI_QSPI_PRV_LIOCFGCS_CSENGEX_VALUE_MASK) <<
            XSPI_QSPI_PRV_LIOCFGCS_CSENGEX_OFFSET);

    /* Specifies the read/write commands and Read dummy clocks for Device
     * (see RZ microprocessor User's Manual section "Flow of Memory-mapping"). */
    p_instance_ctrl->p_reg->CSa[p_cfg_extend->chip_select].CMCFG0 =
        (0U << XSPI_QSPI_PRV_CMCFG0_FFMT_OFFSET) |
        ((p_cfg->address_bytes & XSPI_QSPI_PRV_CMCFG0_ADDSIZE_VALUE_MASK) << XSPI_QSPI_PRV_CMCFG0_ADDSIZE_OFFSET);

    p_instance_ctrl->p_reg->CSa[p_cfg_extend->chip_select].CMCFG1 =
        ((p_cfg->read_command & XSPI_QSPI_PRV_CMCFG1_RDCMD_VALUE_MASK) << XSPI_QSPI_PRV_CMCFG1_RDCMD_UPPER_OFFSET) |
        ((p_cfg->dummy_clocks & XSPI_QSPI_PRV_CMCFG1_RDLATE_VALUE_MASK) << XSPI_QSPI_PRV_CMCFG1_RDLATE_OFFSET);

    p_instance_ctrl->p_reg->CSa[p_cfg_extend->chip_select].CMCFG2 =
        ((p_cfg->page_program_command & XSPI_QSPI_PRV_CMCFG1_WRCMD_VALUE_MASK) <<
            XSPI_QSPI_PRV_CMCFG2_WRCMD_UPPER_OFFSET) |
        (0U << XSPI_QSPI_PRV_CMCFG2_WRLATE_OFFSET);

    p_instance_ctrl->p_reg->BMCTL0 = XSPI_QSPI_PRV_BMCTL_DEFAULT_VALUE;

    /* Return response after issuing write transaction to xSPI bus, Enable prefetch function. */
    p_instance_ctrl->p_reg->BMCFG = (0 << XSPI_QSPI_PRV_BMCFG_WRMD_OFFSET) |
                                    (1 << XSPI_QSPI_PRV_BMCFG_MWRCOMB_OFFSET) |
                                    (0x0F << XSPI_QSPI_PRV_BMCFG_MWRSIZE_OFFSET) |
                                    ((p_cfg_extend->prefetch_en & XSPI_QSPI_PRV_BMCFG_PREEN_VALUE_MASK) <<
                                     XSPI_QSPI_PRV_BMCFG_PREEN_OFFSET);

    /* Set use Channel. */
    p_instance_ctrl->p_reg->CDCTL0_b.CSSEL = p_cfg_extend->chip_select;

    /* The memory size is read from the device if needed. */
    p_instance_ctrl->total_size_bytes = 0U;

    p_instance_ctrl->open = XSPI_QSPI_PRV_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Writes raw data directly to the QSPI.
 *
 * Implements @ref spi_flash_api_t::directWrite.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_DirectWrite (spi_flash_ctrl_t    * p_ctrl,
                                   uint8_t const * const p_src,
                                   uint32_t const        bytes,
                                   bool const            read_after_write)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_src);
    FSP_PARAMETER_NOT_USED(bytes);
    FSP_PARAMETER_NOT_USED(read_after_write);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Reads raw data directly from the QSPI.
 *
 * Implements @ref spi_flash_api_t::directRead.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_DirectRead (spi_flash_ctrl_t * p_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_dest);
    FSP_PARAMETER_NOT_USED(bytes);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Read/Write raw data directly with the SerialFlash.
 *
 * Implements @ref spi_flash_api_t::directTransfer.
 *
 * Example:
 * @snippet r_xspi_qspi_example.c R_XSPI_QSPI_DirectTransfer
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function must be called after R_XSPI_QSPI_DirectWrite with read_after_write set
 *                                     to true.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_DirectTransfer (spi_flash_ctrl_t                  * p_ctrl,
                                      spi_flash_direct_transfer_t * const p_transfer,
                                      spi_flash_direct_transfer_dir_t     direction)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    fsp_err_t err = r_xspi_qspi_param_checking_dcom(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_transfer);
    FSP_ASSERT(0 != p_transfer->command_length);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    r_xspi_qspi_direct_transfer(p_instance_ctrl, p_transfer, direction);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enters XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipEnter.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_XipEnter (spi_flash_ctrl_t * p_ctrl)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    fsp_err_t err;

    err = r_xspi_qspi_xip(p_instance_ctrl, p_instance_ctrl->p_cfg->xip_enter_command, true);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Exits XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipExit.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_XipExit (spi_flash_ctrl_t * p_ctrl)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    fsp_err_t err;

    err = r_xspi_qspi_xip(p_instance_ctrl, p_instance_ctrl->p_cfg->xip_exit_command, false);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Program a page of data to the flash.
 *
 * Implements @ref spi_flash_api_t::write.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl, p_dest or p_src is NULL, or byte_count crosses a page boundary.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 *
 * @note In this API, data can be written up to 64 bytes at a time.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_Write (spi_flash_ctrl_t    * p_ctrl,
                             uint8_t const * const p_src,
                             uint8_t * const       p_dest,
                             uint32_t              byte_count)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    fsp_err_t err = r_xspi_qspi_program_param_check(p_instance_ctrl, p_src, p_dest, byte_count);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif
    FSP_ERROR_RETURN(false == r_xspi_qspi_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);

    r_xspi_qspi_write_enable(p_instance_ctrl);

    uint32_t i = 0;

    /* Word access */
    if (0 == byte_count % XSPI_QSPI_PRV_WORD_ACCESS_SIZE)
    {
        uint32_t * p_word_aligned_dest = (uint32_t *) p_dest;
        uint32_t * p_word_aligned_src  = (uint32_t *) p_src;
        for (i = 0; i < byte_count / XSPI_QSPI_PRV_WORD_ACCESS_SIZE; i++)
        {
            *p_word_aligned_dest = *p_word_aligned_src;
            p_word_aligned_dest++;
            p_word_aligned_src++;
        }
    }
    /* Half Word access */
    else if (0 == byte_count % XSPI_QSPI_PRV_HALF_WORD_ACCESS_SIZE)
    {
        uint16_t * p_half_word_aligned_dest = (uint16_t *) p_dest;
        uint16_t * p_half_word_aligned_src  = (uint16_t *) p_src;
        for (i = 0; i < byte_count / XSPI_QSPI_PRV_HALF_WORD_ACCESS_SIZE; i++)
        {
            *p_half_word_aligned_dest = *p_half_word_aligned_src;
            p_half_word_aligned_dest++;
            p_half_word_aligned_src++;
        }
    }
    /* Byte access */
    else
    {
        for (i = 0; i < byte_count; i++)
        {
            p_dest[i] = p_src[i];
        }
    }

    /* Ensure that all write data is in the xSPI write buffer. */
    __asm volatile ("dsb");

    /* Request to push the pending data */
    if (XSPI_QSPI_PRV_MAX_COMBINE_SIZE > byte_count)
    {
        /* Push the pending data. */
        p_instance_ctrl->p_reg->BMCTL1_b.MWRPUSH = 1;
    }

    /* Wait until memory access starts in write API. */
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->COMSTT_b.MEMACC, 1);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Erase a block or sector of flash.  The byte_count must exactly match one of the erase sizes defined in spi_flash_cfg_t.
 * For chip erase, byte_count must be SPI_FLASH_ERASE_SIZE_CHIP_ERASE.
 *
 * Implements @ref spi_flash_api_t::erase.
 *
 * @retval FSP_SUCCESS                 The command to erase the flash was executed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_device_address is NULL, or byte_count doesn't match an erase
 *                                     size defined in spi_flash_cfg_t, or device is in XIP mode.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_Erase (spi_flash_ctrl_t * p_ctrl, uint8_t * const p_device_address, uint32_t byte_count)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    fsp_err_t err = r_xspi_qspi_param_checking_dcom(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_device_address);
#endif

    uint16_t erase_command = 0;
    uint32_t chip_address  = (uint32_t) p_device_address - XSPI_QSPI_DEVICE_START_ADDRESS;
    bool     send_address  = true;

    for (uint32_t index = 0; index < p_instance_ctrl->p_cfg->erase_command_list_length; index++)
    {
        /* If requested byte_count is supported by underlying flash, store the command. */
        if (byte_count == p_instance_ctrl->p_cfg->p_erase_command_list[index].size)
        {
            if (SPI_FLASH_ERASE_SIZE_CHIP_ERASE == byte_count)
            {
                /* Don't send address for chip erase. */
                send_address = false;
            }

            erase_command = p_instance_ctrl->p_cfg->p_erase_command_list[index].command;
            break;
        }
    }

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(0U != erase_command);
#endif

    r_xspi_qspi_write_enable(p_instance_ctrl);

    spi_flash_direct_transfer_t direct_command = {0};
    direct_command.command        = erase_command;
    direct_command.address        = chip_address;
    direct_command.address_length = (true == send_address) ?
                                    (((p_instance_ctrl->p_cfg->address_bytes) + 1U)) &
                                    XSPI_QSPI_PRV_CDTBUF_ADDSIZE_VALUE_MASK : 0U;
    direct_command.command_length = 1U;

    r_xspi_qspi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets the write or erase status of the flash.
 *
 * Implements @ref spi_flash_api_t::statusGet.
 *
 * @retval FSP_SUCCESS                 The write status is in p_status.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_status is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_StatusGet (spi_flash_ctrl_t * p_ctrl, spi_flash_status_t * const p_status)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(0U == p_instance_ctrl->p_reg->CMCTL_b.XIPEN, FSP_ERR_INVALID_MODE);
#endif

    uint32_t comstt  = p_instance_ctrl->p_reg->COMSTT;
    bool     memacc  = (bool) ((comstt >> XSPI_QSPI_PRV_COMSTT_MEMACC_OFFSET) & XSPI_QSPI_PRV_COMSTT_MEMACC_VALUE_MASK);
    bool     wrbufne =
        (bool) ((comstt >> XSPI_QSPI_PRV_COMSTT_WRBUFNE_OFFSET) & XSPI_QSPI_PRV_COMSTT_WRBUFNE_VALUE_MASK);

    if ((true == memacc) || (true == wrbufne))
    {
        /* If the xSPI is accessing memory or data is in the write buffer,
         * it is judged a "write in progress" without access to QSPI. */
        p_status->write_in_progress = (bool) (wrbufne | memacc);
    }
    else
    {
        /* Read device status. */
        p_status->write_in_progress = r_xspi_qspi_status_sub(p_instance_ctrl);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Selects the bank to access.
 *
 * Implements @ref spi_flash_api_t::bankSet.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_BankSet (spi_flash_ctrl_t * p_ctrl, uint32_t bank)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(bank);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Sets the SPI protocol.
 *
 * Implements @ref spi_flash_api_t::spiProtocolSet.
 *
 * @retval FSP_SUCCESS                SPI protocol updated on MCU peripheral.
 * @retval FSP_ERR_ASSERTION          A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN           Driver is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT   Invalid SPI protocol requested.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_SpiProtocolSet (spi_flash_ctrl_t * p_ctrl, spi_flash_protocol_t spi_protocol)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Not allow the following arguremnts with this IP. */
    FSP_ERROR_RETURN(SPI_FLASH_PROTOCOL_QPI != spi_protocol, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPI_FLASH_PROTOCOL_SOPI != spi_protocol, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPI_FLASH_PROTOCOL_DOPI != spi_protocol, FSP_ERR_INVALID_ARGUMENT);

    /* Not allow the following arguremnts with QSPI HAL Driver. */
    FSP_ERROR_RETURN(SPI_FLASH_PROTOCOL_8D_8D_8D != spi_protocol, FSP_ERR_INVALID_ARGUMENT);
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif

    spi_flash_cfg_t          * p_cfg        = (spi_flash_cfg_t *) p_instance_ctrl->p_cfg;
    xspi_qspi_extended_cfg_t * p_cfg_extend = (xspi_qspi_extended_cfg_t *) p_cfg->p_extend;

    p_instance_ctrl->spi_protocol = spi_protocol;

    /* Update the SPI protocol. */
    p_instance_ctrl->p_reg->LIOCFGCS_b[p_cfg_extend->chip_select].PRTMD = spi_protocol &
                                                                          XSPI_QSPI_PRV_LIOCFGCS_PRTMD_MASK;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close the QSPI driver module.
 *
 * Implements @ref spi_flash_api_t::close.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_Close (spi_flash_ctrl_t * p_ctrl)
{
    xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) p_ctrl;
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    spi_flash_cfg_t          * p_cfg        = (spi_flash_cfg_t *) p_instance_ctrl->p_cfg;
    xspi_qspi_extended_cfg_t * p_cfg_extend = (xspi_qspi_extended_cfg_t *) p_cfg->p_extend;

    p_instance_ctrl->open = 0U;

    /* Disable clock to the QSPI block */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_STOP(FSP_IP_XSPI, p_cfg_extend->unit);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * DEPRECATED Get the driver version based on compile time macros.
 *
 * Implements @ref spi_flash_api_t::versionGet.
 *
 * @retval     FSP_SUCCESS          Successful close.
 * @retval     FSP_ERR_ASSERTION    p_version is NULL.
 *
 **********************************************************************************************************************/
fsp_err_t R_XSPI_QSPI_VersionGet (fsp_version_t * const p_version)
{
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = g_xspi_qspi_version.version_id;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup XSPI_QSPI)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Send Write enable command to the SerialFlash
 *
 * @param[in]   p_instance_ctrl    Pointer to QSPI specific control structure
 **********************************************************************************************************************/
static void r_xspi_qspi_write_enable (xspi_qspi_instance_ctrl_t * p_instance_ctrl)
{
    spi_flash_direct_transfer_t direct_command = {0};
    spi_flash_cfg_t const     * p_cfg          = p_instance_ctrl->p_cfg;

    direct_command.command        = p_cfg->write_enable_command;
    direct_command.command_length = 1U;

    r_xspi_qspi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
}

/*******************************************************************************************************************//**
 * Enters or exits XIP (execute in place) mode.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 * @param[in]  code                    Code to place in M7-M0
 * @param[in]  enter_mode              True to enter, false to exit
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
static fsp_err_t r_xspi_qspi_xip (xspi_qspi_instance_ctrl_t * p_instance_ctrl, uint8_t code, bool enter_mode)
{
#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE

    /* FSP_ASSERT(NULL != p_instance_ctrl) is optimized out when it shouldn't be.  It appears to be affected by GCC bug
     * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90949. */
    xspi_qspi_instance_ctrl_t * volatile p_volatile_ctrl = p_instance_ctrl;
    FSP_ASSERT(NULL != p_volatile_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    xspi_qspi_extended_cfg_t * p_cfg_extend =
        (xspi_qspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint8_t unit        = p_cfg_extend->unit;
    uint8_t chip_select = p_cfg_extend->chip_select;

    volatile uint8_t dummy = 0;
    FSP_PARAMETER_NOT_USED(dummy);

    /* XiP configuration (see RZ microprocessor User's Manual section "Flow of XiP"). */
    if (true == enter_mode)
    {
        if (XSPI_QSPI_CHIP_SELECT_0 == chip_select)
        {
            p_instance_ctrl->p_reg->BMCTL0_b.CS0ACC = XSPI_QSPI_PRV_BMCTL_CSNACC_R_ENABLE;
        }
        else
        {
            p_instance_ctrl->p_reg->BMCTL0_b.CS1ACC = XSPI_QSPI_PRV_BMCTL_CSNACC_R_ENABLE;
        }
    }

    p_instance_ctrl->p_reg->CMCTL_b.XIPENCODE = code;
    p_instance_ctrl->p_reg->CMCTL_b.XIPEXCODE = code;
    p_instance_ctrl->p_reg->CMCTL_b.XIPEN     = enter_mode;

    /* Read from QSPI to send the XIP entry request.
     * Read via a cache-invalid mirror area to ensure access to QSPI. */
    dummy = *(volatile uint8_t *) (XSPI_QSPI_DEVICE_START_MIRROR_ADDRESS +
                                   (unit * XSPI_QSPI_DEVICE_XSPI0_1_ADDRESS_DELTA) +
                                   (chip_select * XSPI_QSPI_DEVICE_CS0_1_ADDRESS_DELTA));

    if (false == enter_mode)
    {
        if (XSPI_QSPI_CHIP_SELECT_0 == chip_select)
        {
            p_instance_ctrl->p_reg->BMCTL0_b.CS0ACC = XSPI_QSPI_PRV_BMCTL_CSNACC_R_W_ENABLE;
        }
        else
        {
            p_instance_ctrl->p_reg->BMCTL0_b.CS1ACC = XSPI_QSPI_PRV_BMCTL_CSNACC_R_W_ENABLE;
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets device status.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 *
 * @return True if busy, false if not.
 **********************************************************************************************************************/
static bool r_xspi_qspi_status_sub (xspi_qspi_instance_ctrl_t * p_instance_ctrl)
{
    spi_flash_cfg_t const    * p_cfg        = p_instance_ctrl->p_cfg;
    xspi_qspi_extended_cfg_t * p_cfg_extend = (xspi_qspi_extended_cfg_t *) p_cfg->p_extend;

    spi_flash_direct_transfer_t direct_command = {0};

    bool                 restore_spi_protocol = false;
    spi_flash_protocol_t setting_protocol     = (spi_flash_protocol_t) 0x00;

    direct_command.command        = p_cfg->status_command;
    direct_command.command_length = 1U;
    direct_command.data_length    = 1U;

    /* For most devices, the Read Status Register instruction is executed with the 1S-1S-1S instruction,
     * even if it assumes 1S-2S-2S or 1S-4S-4S Write instruction.
     * Therefore, the communication protocol is temporarily switched. */
    if ((SPI_FLASH_PROTOCOL_1S_4S_4S == p_instance_ctrl->spi_protocol) ||
        (SPI_FLASH_PROTOCOL_1S_2S_2S == p_instance_ctrl->spi_protocol))
    {
        setting_protocol = p_instance_ctrl->spi_protocol;
        p_instance_ctrl->p_reg->LIOCFGCS_b[p_cfg_extend->chip_select].PRTMD = SPI_FLASH_PROTOCOL_1S_1S_1S;
        p_instance_ctrl->spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S;
        restore_spi_protocol          = true;
    }

    r_xspi_qspi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);

    /* Restore communication protocol. */
    if (true == restore_spi_protocol)
    {
        p_instance_ctrl->p_reg->LIOCFGCS_b[p_cfg_extend->chip_select].PRTMD = setting_protocol;
        p_instance_ctrl->spi_protocol = setting_protocol;
    }

    return (bool) ((direct_command.data >> p_instance_ctrl->p_cfg->write_status_bit) &
                   XSPI_QSPI_PRV_DEVICE_WRITE_STATUS_BIT_MASK);
}

/*******************************************************************************************************************//**
 * Performs direct data transfer with the SerialFlash
 *
 * @param[in]   p_instance_ctrl    Pointer to QSPI specific control structure
 * @param[in]   p_transfer             Pointer to transfer parameters
 * @param[in]   direction          Read/Write
 **********************************************************************************************************************/
static void r_xspi_qspi_direct_transfer (xspi_qspi_instance_ctrl_t         * p_instance_ctrl,
                                         spi_flash_direct_transfer_t * const p_transfer,
                                         spi_flash_direct_transfer_dir_t     direction)
{
    p_instance_ctrl->p_reg->CDCTL0_b.TRNUM = 0;

    /* Direct Read/Write settings
     * (see RZ microprocessor User's Manual section "Flow of Manual-command Procedure"). */
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CDCTL0_b.TRREQ, 0);

    p_instance_ctrl->p_reg->BUF[0].CDT =
        ((p_transfer->command_length & XSPI_QSPI_PRV_CDTBUF_CMDSIZE_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_CMDSIZE_OFFSET) |
        ((p_transfer->address_length & XSPI_QSPI_PRV_CDTBUF_ADDSIZE_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_ADDSIZE_OFFSET) |
        ((p_transfer->data_length & XSPI_QSPI_PRV_CDTBUF_DATASIZE_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_DATASIZE_OFFSET) |
        ((p_transfer->dummy_cycles & XSPI_QSPI_PRV_CDTBUF_LATE_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_LATE_OFFSET) |
        ((direction & XSPI_QSPI_PRV_CDTBUF_TRTYPE_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_TRTYPE_OFFSET) |
        ((p_transfer->command & XSPI_QSPI_PRV_CDTBUF_CMD_VALUE_MASK) <<
            XSPI_QSPI_PRV_CDTBUF_CMD_UPPER_OFFSET);

    p_instance_ctrl->p_reg->BUF[0].CDA = p_transfer->address;

    if (SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE == direction)
    {
        p_instance_ctrl->p_reg->BUF[0].CDD0 = p_transfer->data;
    }

    p_instance_ctrl->p_reg->CDCTL0_b.TRREQ = 1;
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->INTS_b.CMDCMP, 1);

    if (SPI_FLASH_DIRECT_TRANSFER_DIR_READ == direction)
    {
        p_transfer->data = p_instance_ctrl->p_reg->BUF[0].CDD0;
    }

    p_instance_ctrl->p_reg->INTC = 1 << XSPI_QSPI_PRV_INTC_CMDCMPC_OFFSET;
}

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 *
 * @retval FSP_SUCCESS                 Device size stored in p_device_size_bytes
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 **********************************************************************************************************************/
static fsp_err_t r_xspi_qspi_param_checking_dcom (xspi_qspi_instance_ctrl_t * p_instance_ctrl)
{
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(XSPI_QSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(0U == p_instance_ctrl->p_reg->CMCTL_b.XIPEN, FSP_ERR_INVALID_MODE);

    /* Verify device is not busy. */
    FSP_ERROR_RETURN(!r_xspi_qspi_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);

    return FSP_SUCCESS;
}

#endif

#if XSPI_QSPI_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking for R_XSPI_QSPI_Write.
 *
 * @param[in] p_instance_ctrl          Pointer to a driver handle
 * @param[in] p_src                    The source of the data to write to QSPI
 * @param[in] p_dest                   The address in QSPI to write to
 * @param[in] byte_count               The number of bytes to write
 *
 * @retval FSP_SUCCESS                 Parameters are valid.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl,p_device_address or p_memory_address is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
static fsp_err_t r_xspi_qspi_program_param_check (xspi_qspi_instance_ctrl_t * p_instance_ctrl,
                                                  uint8_t const * const       p_src,
                                                  uint8_t * const             p_dest,
                                                  uint32_t                    byte_count)
{
    fsp_err_t err = r_xspi_qspi_param_checking_dcom(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);

    /* Check if byte_count is valid */
    uint32_t page_size_bytes    = p_instance_ctrl->p_cfg->page_size_bytes;
    uint32_t bytes_left_in_page = page_size_bytes - ((uint32_t) p_dest % page_size_bytes);
    FSP_ASSERT(byte_count <= bytes_left_in_page);

    FSP_ASSERT(XSPI_QSPI_PRV_MAX_COMBINE_SIZE >= byte_count);

    return FSP_SUCCESS;
}

#endif
