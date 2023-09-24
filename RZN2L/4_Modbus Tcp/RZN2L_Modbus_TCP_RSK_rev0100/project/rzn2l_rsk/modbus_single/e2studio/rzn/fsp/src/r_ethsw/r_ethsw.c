/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

#include <string.h>

#include "bsp_api.h"
#include "r_ethsw.h"
#include <math.h>

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

#ifndef ETHSW_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHSW_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif                                 /* !ETHSW_ERROR_RETURN */

/** "SWIT" in ASCII.  Used to determine if the control block is open. */
#define ETHSW_OPEN                            (0x53574954U)

/* Initial value of down counter for timeout detection */
#define ETHSW_TIMEOUT_COUNT                   (1000000000U)

/* Maximum Frame Length */
#define ETHSW_FRAME_SIZE                      (0x2408U)

/* ENABLE PORT Bit Definitions(PORT_ENA,UCAST_DEFAULT_MASK,BCAST_DEFAULT_MASK,MCAST_DEFAULT_MASK Register) */
#define ETHSW_ENABLE_PORT0                    (1UL << 0)
#define ETHSW_ENABLE_PORT1                    (1UL << 1)
#define ETHSW_ENABLE_PORT2                    (1UL << 2)
#define ETHSW_ENABLE_PORT3                    (1UL << 3)

/* COMMAND_CONFIG Register Bit Definitions */
#define ETHSW_COMMAND_CONFIG_ENA_10           (1UL << 25)
#define ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK    (1UL << 24)
#define ETHSW_COMMAND_CONFIG_CNTL_FRM_ENA     (1UL << 23)
#define ETHSW_COMMAND_CONFIG_LOOP_ENA         (1UL << 15)
#define ETHSW_COMMAND_CONFIG_SW_RESET         (1UL << 13)
#define ETHSW_COMMAND_CONFIG_TX_CRC_APPEND    (1UL << 11)
#define ETHSW_COMMAND_CONFIG_HD_ENA           (1UL << 10)
#define ETHSW_COMMAND_CONFIG_TX_ADDR_INS      (1UL << 9)
#define ETHSW_COMMAND_CONFIG_PAUSE_IGNORE     (1UL << 8)
#define ETHSW_COMMAND_CONFIG_PAUSE_FWD        (1UL << 7)
#define ETHSW_COMMAND_CONFIG_CRC_FWD          (1UL << 6)
#define ETHSW_COMMAND_CONFIG_PAD_EN           (1UL << 5)
#define ETHSW_COMMAND_CONFIG_PROMIS_EN        (1UL << 4)
#define ETHSW_COMMAND_CONFIG_ETH_SPEED        (1UL << 3)
#define ETHSW_COMMAND_CONFIG_RX_ENA           (1UL << 1)
#define ETHSW_COMMAND_CONFIG_TX_ENA           (1UL << 0)

/* INT_CONFIG Register Bit Number Definitions */
#define ETHSW_INT_CONFIG_TSM_INT              (1UL << 29)
#define ETHSW_INT_CONFIG_TDMA_INT             (1UL << 30)
#define ETHSW_INT_CONFIG_IRQ_LNK_SHIFT        (8U)
#define ETHSW_INT_CONFIG_IRQ_LNK_MASK         (0x7 << ETHSW_INT_CONFIG_IRQ_LNK_SHIFT)
#define ETHSW_INT_CONFIG_DLR_INT              (1UL << 5)
#define ETHSW_INT_CONFIG_IRQ_EN               (1UL << 0)

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void ethsw_enable_icu(ethsw_instance_ctrl_t * const p_instance_ctrl);

static void ethsw_disable_icu(ethsw_instance_ctrl_t * const p_instance_ctrl);

void ethsw_isr_intr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if defined(__GNUC__)

/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/

/*LDRA_INSPECTED 69 S */
 #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/** ETHER SWITCH HAL module version data structure */
const fsp_version_t module_version =
{
    .api_version_minor  = ETHSW_API_VERSION_MINOR,
    .api_version_major  = ETHSW_API_VERSION_MAJOR,
    .code_version_major = ETHSW_CODE_VERSION_MAJOR,
    .code_version_minor = ETHSW_CODE_VERSION_MINOR
};

/** ETHER SWITCH HAL API mapping for Ethernet Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ethsw_api_t g_ethsw_on_ethsw =
{
    .open       = R_ETHSW_Open,
    .close      = R_ETHSW_Close,
    .speedCfg   = R_ETHSW_SpeedCfg,
    .versionGet = R_ETHSW_VersionGet,
};

/*******************************************************************************************************************//**
 * @addtogroup ETHSW
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/********************************************************************************************************************//**
 * Open the switch ports and gets it ready to pass through the frames.
 *
 * @retval  FSP_SUCCESS                 Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER SWITCH control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN        Control block has already been opened or channel is being used by another
 *                                      instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL     Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Interrupt is not enabled.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_Open (ethsw_ctrl_t * const p_ctrl, ethsw_cfg_t const * const p_cfg)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ethsw_instance_ctrl_t * p_instance_ctrl =
        (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type * p_reg_switch;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((BSP_FEATURE_ETHSW_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);
    ETHSW_ERROR_RETURN((0 <= p_cfg->irq), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((ETHSW_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);

    if (ETHSW_PHYLINK_ENABLE == p_cfg->phylink)
    {
        ETHSW_ERROR_RETURN((NULL != p_cfg->p_callback), FSP_ERR_INVALID_POINTER);
        ETHSW_ERROR_RETURN((NULL != p_cfg->p_context), FSP_ERR_INVALID_POINTER);
    }
#endif

    p_instance_ctrl->p_reg_switch = (void *) R_ETHSW;

    p_reg_switch = (R_ETHSW_Type *) p_instance_ctrl->p_reg_switch;

    /* Initialize configuration of Ethernet switch module. */
    p_instance_ctrl->p_switch_cfg = p_cfg;

    /* Module start */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_ModuleResetDisable(BSP_MODULE_RESET_ETHSW);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    /* set Maximum Frame Length size */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = ETHSW_FRAME_SIZE;

    /* Initialize COMMAND_CONFIG_p[0] Reg */
    p_reg_switch->COMMAND_CONFIG_P0 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[1] Reg */
    p_reg_switch->COMMAND_CONFIG_P1 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[2] Reg */
    p_reg_switch->COMMAND_CONFIG_P2 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[3] Reg */
    p_reg_switch->COMMAND_CONFIG_P3 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize PORT ENABLE Reg */
    p_reg_switch->PORT_ENA_b.RXENA =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);
    p_reg_switch->PORT_ENA_b.TXENA =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* Initialize Learning/Lookup function global configuration Reg */
    p_reg_switch->LK_CTRL            = 0;
    p_reg_switch->LK_CTRL_b.ALW_MGRT = 1;
    p_reg_switch->LK_CTRL_b.AGING_EN = 1;
    p_reg_switch->LK_CTRL_b.LEARN_EN = 1;
    p_reg_switch->LK_CTRL_b.LKUP_EN  = 1;

    /* enable Default unicast resolution */
    p_reg_switch->UCAST_DEFAULT_MASK0_b.UCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* enable Default broadcast resolution */
    p_reg_switch->BCAST_DEFAULT_MASK0_b.BCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* enable Default multicast resolution */
    p_reg_switch->MCAST_DEFAULT_MASK0_b.MCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* set port in authorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT0_b.AUTH = 1;

    /* set port in authorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT1_b.AUTH = 1;

    /* set port in authorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT2_b.AUTH = 1;

    /* set port in authorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT3_b.AUTH = 1;

    /* Initialize MGMT_TAG_CONFIG Reg */
    if (ETHSW_SPECIFIC_TAG_ENABLE == p_cfg->specific_tag)
    {
        p_reg_switch->MGMT_TAG_CONFIG_b.TAGFIELD  = (uint16_t) p_cfg->specific_tag_id;
        p_reg_switch->MGMT_TAG_CONFIG_b.TYPE2_EN  = 0;
        p_reg_switch->MGMT_TAG_CONFIG_b.TYPE1_EN  = 0;
        p_reg_switch->MGMT_TAG_CONFIG_b.AL_FRAMES = 1;
        p_reg_switch->MGMT_TAG_CONFIG_b.ENABLE    = 1;
    }
    else
    {
        p_reg_switch->MGMT_TAG_CONFIG_b.ENABLE = 0;
    }

    /* Set Ethernet Switch interrupt level and enable */
    ethsw_enable_icu(p_instance_ctrl);

    if (ETHSW_PHYLINK_ENABLE == p_instance_ctrl->p_switch_cfg->phylink)
    {
        /* Set line port phy link change interrupt enable Bit (PORT0 - 2) */
        p_reg_switch->INT_CONFIG_b.IRQ_LINK = (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2);

        /* Set Interrupt global enable */
        p_reg_switch->INT_CONFIG_b.IRQ_EN = 1;
    }

    p_instance_ctrl->open = ETHSW_OPEN;

    return err;
}                                      /* End of function R_ETHSW_Open() */

/********************************************************************************************************************//**
 * Close the switch ports
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_Close (ethsw_ctrl_t * const p_ctrl)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    R_ETHSW_Type * p_reg_switch;

    p_reg_switch = (R_ETHSW_Type *) p_instance_ctrl->p_reg_switch;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set line port phy link change interrupt disable Bit (PORT0 - 2) */
    p_reg_switch->INT_CONFIG_b.IRQ_LINK = 0;

    /* Set Interrupt global disable */
    p_reg_switch->INT_CONFIG_b.IRQ_EN = 0;

    /* set port in unauthorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT0_b.AUTH = 0;

    /* set port in unauthorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT1_b.AUTH = 0;

    /* set port in unauthorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT2_b.AUTH = 0;

    /* set port in unauthorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT3_b.AUTH = 0;

    /* clear Maximum Frame Length Value */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = ETHSW_FRAME_SIZE;

    /* Disable PORT Rx Tx */
    p_reg_switch->PORT_ENA_b.RXENA = 0U;
    p_reg_switch->PORT_ENA_b.TXENA = 0U;

    /* Disable the MAC reception function(COMMAND_CONFIG_p[1] Reg) */
    p_reg_switch->COMMAND_CONFIG_P0 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P1 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P2 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[3] Reg) */
    p_reg_switch->COMMAND_CONFIG_P3 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable Ethernet interrupt. */
    ethsw_disable_icu(p_instance_ctrl);

    /** Clear configure block parameters. */
    p_instance_ctrl->p_switch_cfg = NULL;

    /** Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return err;
}                                      /* End of function R_ETHER_Close() */

/********************************************************************************************************************//**
 * Configs speed and duplex of the port
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid input parameter.
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_SpeedCfg (ethsw_ctrl_t * const p_ctrl, uint8_t const port, ethsw_link_speed_t const speed)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_reg_command_config;
    uint32_t                command_config;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_reg_switch = (R_ETHSW_Type *) p_instance_ctrl->p_reg_switch;

    switch (port)
    {
        case 0:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P0;
            break;
        }

        case 1:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P1;
            break;
        }

        case 2:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P2;
            break;
        }

        default:
        {
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        }
    }

    if (err == FSP_SUCCESS)
    {
        command_config = *p_reg_command_config;

        command_config &= ~(ETHSW_COMMAND_CONFIG_ENA_10 |
                            ETHSW_COMMAND_CONFIG_HD_ENA |
                            ETHSW_COMMAND_CONFIG_ETH_SPEED);
        switch (speed)
        {
            case ETHSW_LINK_SPEED_10H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ENA_10 |
                                   ETHSW_COMMAND_CONFIG_HD_ENA);
                break;
            }

            case ETHSW_LINK_SPEED_10F:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ENA_10);
                break;
            }

            case ETHSW_LINK_SPEED_100H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_HD_ENA);
                break;
            }

            case ETHSW_LINK_SPEED_100F:
            {
                break;
            }

            case ETHSW_LINK_SPEED_1000H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_HD_ENA |
                                   ETHSW_COMMAND_CONFIG_ETH_SPEED);
                break;
            }

            case ETHSW_LINK_SPEED_1000F:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ETH_SPEED);
                break;
            }

            default:
            {
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            }
        }

        *p_reg_command_config = command_config;
    }

    return err;
}                                      /* End of function R_ETHSW_ConfSpeed() */

/********************************************************************************************************************//**
 * Provides API and code version in the user provided pointer. Implements @ref ethsw_api_t::versionGet.
 *
 * @retval  FSP_SUCCESS                  Version information stored in provided p_version.
 * @retval  FSP_ERR_ASSERTION            p_version is NULL.
 ***********************************************************************************************************************/
__INLINE fsp_err_t R_ETHSW_VersionGet (fsp_version_t * const p_version)
{
#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_version);
#endif

    *p_version = module_version;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VersionGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_SWITCH)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Interrupt handler for Ethernet switch.
 * Supports the interrupts for DLR, PHY link change, TSM(Timestamping) and TDMA.
 *
 * @retval      none
 **********************************************************************************************************************/
void ethsw_isr_intr (void)
{
    ethsw_callback_args_t callback_arg;
    uint32_t              ulInt_Stat_Ack_Val;
    R_ETHSW_Type        * p_reg_switch;
    volatile uint32_t     dammy_read;

    IRQn_Type               irq             = R_FSP_CurrentIrqGet();
    ethsw_instance_ctrl_t * p_instance_ctrl =
        (ethsw_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    p_reg_switch = (R_ETHSW_Type *) p_instance_ctrl->p_reg_switch;

    ulInt_Stat_Ack_Val = p_reg_switch->INT_STAT_ACK;

    /* Clear IRQ_LINK Interrupt */
    p_reg_switch->INT_STAT_ACK = ulInt_Stat_Ack_Val;

    /* Dammy read for Countermeasure when the CPU clock is 400 or 800MHz */
    dammy_read = p_reg_switch->INT_STAT_ACK;
    dammy_read = dammy_read;

    /* Callback : Interrupt handler */
    if ((0 != (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_IRQ_LNK_MASK)) &&
        (ETHSW_PHYLINK_ENABLE == p_instance_ctrl->p_switch_cfg->phylink))
    {
        callback_arg.channel = p_instance_ctrl->p_switch_cfg->channel;

        callback_arg.status_link  = p_reg_switch->STATUS_P0_b.PHYLINK;
        callback_arg.status_link |= (uint32_t) (p_reg_switch->STATUS_P1_b.PHYLINK << 1);
        callback_arg.status_link |= (uint32_t) (p_reg_switch->STATUS_P2_b.PHYLINK << 2);

#if 0
        callback_arg.status_link |= p_reg_switch->STATUS_P1_b.PHYLINK << 1U;
        callback_arg.status_link |= p_reg_switch->STATUS_P2_b.PHYLINK << 2;
#endif

        callback_arg.event     = ETHSW_EVENT_LINK_CHG;
        callback_arg.p_context = p_instance_ctrl->p_switch_cfg->p_context;

        (*p_instance_ctrl->p_switch_cfg->p_callback)((void *) &callback_arg);
    }
}

/*******************************************************************************************************************//**
 * Enable ICU for Ether Switcht
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_enable_icu (ethsw_instance_ctrl_t * const p_instance_ctrl)
{
    /** Configure the Ethernet Switch interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq, p_instance_ctrl->p_switch_cfg->ipl, p_instance_ctrl);
}                                      /* End of function ethsw_enable_icu() */

/*******************************************************************************************************************//**
 * Disable ICU for Ether Switcht
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_disable_icu (ethsw_instance_ctrl_t * const p_instance_ctrl)
{
    /* Get IRQ number for A5PSW_Int interrupt. */
    R_BSP_IrqDisable(p_instance_ctrl->p_switch_cfg->irq);
    R_FSP_IsrContextSet(p_instance_ctrl->p_switch_cfg->irq, NULL);
}                                      /* End of function ether_disable_icu() */
