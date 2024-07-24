/***********************************************************************************************************************
 * Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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
 * @addtogroup DMAC
 * @{
 **********************************************************************************************************************/

#ifndef R_DMAC_H
#define R_DMAC_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_dmac_cfg.h"
#include "r_transfer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Max configurable number of transfers in TRANSFER_MODE_NORMAL. */
#define DMAC_MAX_NORMAL_TRANSFER_LENGTH    (0xFFFFFFFF)

/** Max number of transfers per block in TRANSFER_MODE_BLOCK */
#define DMAC_MAX_BLOCK_TRANSFER_LENGTH     (0xFFFFFFFF)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Indicates the descriptor is enabled or disabled. */
typedef enum e_dmac_link_valid
{
    DMAC_LINK_VALID_DESCRIPTOR_DISABLE = 0, ///< The Descriptor is disabled.
    DMAC_LINK_VALID_DESCRIPTOR_ENABLE  = 1, ///< The Descriptor is enabled.
} dmac_link_valid_t;

/** Indicates that the link ends during DMA transfer of this descriptor. */
typedef enum e_dmac_link_end
{
    DMAC_LINK_END_DISABLE = 0,         ///< The link continues.
    DMAC_LINK_END_ENABLE  = 1,         ///< The link ends.
} dmac_link_end_t;

/** Masks write back execution of the dmac_link_cfg_t::link_valid. When disable, DMAC does not perform write-back operation. */
typedef enum e_dmac_link_write_back
{
    DMAC_LINK_WRITE_BACK_ENABLE  = 0,  ///< Set dmac_link_cfg_t::link_valid to disable after the DMA transfer ends.
    DMAC_LINK_WRITE_BACK_DISABLE = 1,  ///< Remain dmac_link_cfg_t::link_valid after DMA transfer ends.
} dmac_link_write_back_t;

/** When dmac_link_cfg_t::link_valid is DMAC_LINK_VALID_DESCRIPTOR_DISABLE at loading of header, specifies whether DMA transfer completion interrupt mask or not. */
typedef enum e_dmac_link_interrupt_mask
{
    DMAC_LINK_INTERRUPT_MASK_DISABLE = 0, ///< DMA transfer completion interrupt is asserted.
    DMAC_LINK_INTERRUPT_MASK_ENABLE  = 1, ///< DMA transfer completion interrupt is masked.
} dmac_link_interrupt_mask_t;

/** Descriptor structure used in DMAC link mode, and variables of dmac_link_cfg_t must be allocated in the memory area. */
typedef struct st_dmac_link_cfg
{
    union
    {
        uint32_t header_u32;                                   ///< Descriptor header
        struct
        {
            dmac_link_valid_t          link_valid         : 1; ///< The descriptor is valid or not.
            dmac_link_end_t            link_end           : 1; ///< The descriptor is end or not.
            dmac_link_write_back_t     write_back_disable : 1; ///< Write back enable or not.
            dmac_link_interrupt_mask_t interrupt_mask     : 1; ///< Interrupt mask is enable or not.
            uint32_t                                      : 28;
        } header;
    };
    void const * volatile p_src;                               ///< Source address.
    void * volatile       p_dest;                              ///< Destination address.
    volatile uint32_t     transaction_byte;                    ///< Transaction byte.
    volatile uint32_t     channel_cfg;                         ///< Channel configuration (Set value for CHCFG_n register).
    volatile uint32_t     channel_interval;                    ///< Channel interval (Set value for CHITVL register).
    volatile uint32_t     channel_extension_cfg;               ///< Channel extension configuration (Set value for CHEXT_n register).
    void * volatile       p_next_link_addr;                    ///< Next link address.
} dmac_link_cfg_t;

/** Select the Next register set to be executed next. */
typedef enum e_dmac_register_select_reverse
{
    DMAC_REGISTER_SELECT_REVERSE_DISABLE = 0x0,                    ///< Use Next0 register set.
    DMAC_REGISTER_SELECT_REVERSE_ENABLE  = 0x1,                    ///< Use Next1 register set.
    DMAC_REGISTER_SELECT_REVERSE_ENABLE_PERFORM_ACCORDINGLY = 0x3, ///< Use Next1 register set after Next0 register set transfer completed.
} dmac_register_select_reverse_t;

/** DACK output mode. See 'DMA Transfer Request Detection Operation Setting Table' of RZ microprocessor manual. */
typedef enum e_dmac_ack_mode
{
    DMAC_ACK_MODE_LEVEL_MODE       = 1, ///< Level mode.
    DMAC_ACK_MODE_BUS_CYCLE_MODE   = 2, ///< Bus cycle mode.
    DMAC_ACK_MODE_MASK_DACK_OUTPUT = 4, ///< Output is masked.
} dmac_ack_mode_t;

/** Detection method of the DMA request signal. See 'DMA Transfer Request Detection Operation Setting Table' of RZ microprocessor manual. */
typedef enum e_dmac_detection
{
    DMAC_DETECTION_FALLING_EDGE = 1,   ///< Falling edge detection.
    DMAC_DETECTION_RISING_EDGE  = 2,   ///< Rising edge detection.
    DMAC_DETECTION_LOW_LEVEL    = 5,   ///< Low level detection.
    DMAC_DETECTION_HIGH_LEVEL   = 6,   ///< High level detection.
} dmac_detection_t;

/** DMA activation request source select. See 'DMA Transfer Request Detection Operation Setting Table' of RZ microprocessor manual. */
typedef enum e_dmac_request_direction
{
    DMAC_REQUEST_DIRECTION_SOURCE_MODULE      = 0, ///< Requested by a transfer source module.
    DMAC_REQUEST_DIRECTION_DESTINATION_MODULE = 1, ///< Requested by a transfer destination module.
} dmac_request_direction_t;

/** DMAC channel scheduling. */
typedef enum e_dmac_channel_scheduling
{
    DMAC_CHANNEL_SCHEDULING_FIXED       = 0, ///< Fixed priority mode.
    DMAC_CHANNEL_SCHEDULING_ROUND_ROBIN = 1, ///< Round-robin mode.
} dmac_channel_scheduling_t;

/** DMAC mode setting. */
typedef enum e_dmac_mode_select
{
    DMAC_MODE_SELECT_REGISTER = 0,     ///< Register mode.
    DMAC_MODE_SELECT_LINK     = 1,     ///< Link mode.
} dmac_mode_select_t;

/** Control block used by driver. DO NOT INITIALIZE - this structure will be initialized in @ref transfer_api_t::open. */
typedef struct st_dmac_instance_ctrl
{
    uint32_t open;                     // Driver ID

    transfer_cfg_t const * p_cfg;

    dmac_mode_select_t      dmac_mode;
    dmac_link_cfg_t const * p_descriptor;

    /* Pointer to base register. */
    R_DMAC0_Type * p_reg;

    void (* p_callback)(transfer_callback_args_t *); // Pointer to callback
    transfer_callback_args_t * p_callback_memory;    // Pointer to optional callback argument memory
    void const               * p_context;            // Pointer to context to be passed into callback function
} dmac_instance_ctrl_t;

/** DMAC transfer configuration extension. This extension is required. */
typedef struct st_dmac_extended_cfg
{
    uint8_t   unit;                     ///< Unit number
    uint8_t   channel;                  ///< Channel number
    IRQn_Type dmac_int_irq;             ///< DMAC interrupt number
    uint8_t   dmac_int_ipl;             ///< DMAC interrupt priority
    uint32_t  dmac_int_irq_detect_type; ///< DMAC interrupt detection type

    /** Select which event will trigger the transfer. */
    elc_event_t activation_source;

    dmac_ack_mode_t          ack_mode;                         ///< DACK output mode
    dmac_detection_t         detection_mode;                   ///< DMAC request detection method
    dmac_request_direction_t activation_request_source_select; ///< DMAC activation request source

    dmac_register_select_reverse_t next_register_operation;    ///< Next register operation settings

    dmac_mode_select_t dmac_mode;                              ///< DMAC Mode

    dmac_link_cfg_t const * p_descriptor;                      ///< The address of the descriptor (DMA Link Mode only)

    uint16_t transfer_interval;                                ///< DMA transfer interval
    dmac_channel_scheduling_t channel_scheduling;              ///< DMA channel scheduling

    /** Callback for transfer end interrupt. */
    void (* p_callback)(transfer_callback_args_t * cb_data);

    /** Placeholder for user data.  Passed to the user p_callback in ::transfer_callback_args_t. */
    void const * p_context;
} dmac_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const transfer_api_t g_transfer_on_dmac;

/** @endcond */

/***********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Open(transfer_ctrl_t * const p_ctrl, transfer_cfg_t const * const p_cfg);
fsp_err_t R_DMAC_Reconfigure(transfer_ctrl_t * const p_ctrl, transfer_info_t * p_info);
fsp_err_t R_DMAC_Reset(transfer_ctrl_t * const p_ctrl,
                       void const * volatile   p_src,
                       void * volatile         p_dest,
                       uint16_t const          num_transfers);
fsp_err_t R_DMAC_SoftwareStart(transfer_ctrl_t * const p_ctrl, transfer_start_mode_t mode);
fsp_err_t R_DMAC_SoftwareStop(transfer_ctrl_t * const p_ctrl);
fsp_err_t R_DMAC_Enable(transfer_ctrl_t * const p_ctrl);
fsp_err_t R_DMAC_Disable(transfer_ctrl_t * const p_ctrl);
fsp_err_t R_DMAC_InfoGet(transfer_ctrl_t * const p_ctrl, transfer_properties_t * const p_info);
fsp_err_t R_DMAC_Close(transfer_ctrl_t * const p_ctrl);
fsp_err_t R_DMAC_Reload(transfer_ctrl_t * const p_ctrl, void const * p_src, void * p_dest,
                        uint32_t const num_transfers);
fsp_err_t R_DMAC_CallbackSet(transfer_ctrl_t * const          p_ctrl,
                             void (                         * p_callback)(transfer_callback_args_t *),
                             void const * const               p_context,
                             transfer_callback_args_t * const p_callback_memory);
fsp_err_t R_DMAC_LinkDescriptorSet(transfer_ctrl_t * const p_ctrl, dmac_link_cfg_t * p_descriptor);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup DMAC)
 **********************************************************************************************************************/
