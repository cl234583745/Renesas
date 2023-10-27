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

#include "bsp_api.h"
#include "r_ether_phy.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define ether_phy_targets_initialize_default    ether_phy_targets_initialize_ksz9131

#ifndef ETHER_PHY_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHER_PHY_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif

#define ETHERC_REG_SIZE                               (0x400UL)

/** "RPHY" in ASCII.  Used to determine if the control block is open. */
#define ETHER_PHY_OPEN                                (0x52504859U)

/* Media Independent Interface */
#define ETHER_PHY_MII_ST                              (1)
#define ETHER_PHY_MII_READ                            (2)
#define ETHER_PHY_MII_WRITE                           (1)

/* Standard PHY Registers */
#define ETHER_PHY_REG_CONTROL                         (0)
#define ETHER_PHY_REG_STATUS                          (1)
#define ETHER_PHY_REG_IDENTIFIER1                     (2)
#define ETHER_PHY_REG_IDENTIFIER2                     (3)
#define ETHER_PHY_REG_AN_ADVERTISEMENT                (4)
#define ETHER_PHY_REG_AN_LINK_PARTNER                 (5)
#define ETHER_PHY_REG_AN_EXPANSION                    (6)
#define ETHER_PHY_REG_1000BT_CONTROL                  (9)
#define ETHER_PHY_REG_1000BT_STATUS                   (10)

/* Basic Mode Control Register Bit Definitions */
#define ETHER_PHY_CONTROL_RESET                       (1 << 15)
#define ETHER_PHY_CONTROL_LOOPBACK                    (1 << 14)
#define ETHER_PHY_CONTROL_100_MBPS                    (1 << 13)
#define ETHER_PHY_CONTROL_AN_ENABLE                   (1 << 12)
#define ETHER_PHY_CONTROL_POWER_DOWN                  (1 << 11)
#define ETHER_PHY_CONTROL_ISOLATE                     (1 << 10)
#define ETHER_PHY_CONTROL_AN_RESTART                  (1 << 9)
#define ETHER_PHY_CONTROL_FULL_DUPLEX                 (1 << 8)
#define ETHER_PHY_CONTROL_COLLISION                   (1 << 7)
#define ETHER_PHY_CONTROL_SPEED_SELCT                 (1 << 6)

/* Basic Mode Status Register Bit Definitions */
#define ETHER_PHY_STATUS_100_T4                       (1 << 15)
#define ETHER_PHY_STATUS_100F                         (1 << 14)
#define ETHER_PHY_STATUS_100H                         (1 << 13)
#define ETHER_PHY_STATUS_10F                          (1 << 12)
#define ETHER_PHY_STATUS_10H                          (1 << 11)
#define ETHER_PHY_STATUS_EX_STATUS                    (1 << 8)
#define ETHER_PHY_STATUS_AN_COMPLETE                  (1 << 5)
#define ETHER_PHY_STATUS_RM_FAULT                     (1 << 4)
#define ETHER_PHY_STATUS_AN_ABILITY                   (1 << 3)
#define ETHER_PHY_STATUS_LINK_UP                      (1 << 2)
#define ETHER_PHY_STATUS_JABBER                       (1 << 1)
#define ETHER_PHY_STATUS_EX_CAPABILITY                (1 << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define ETHER_PHY_AN_ADVERTISEMENT_NEXT_PAGE          (1 << 15)
#define ETHER_PHY_AN_ADVERTISEMENT_RM_FAULT           (1 << 13)
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR            (1 << 11)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE              (1 << 10)
#define ETHER_PHY_AN_ADVERTISEMENT_100_T4             (1 << 9)
#define ETHER_PHY_AN_ADVERTISEMENT_100F               (1 << 8)
#define ETHER_PHY_AN_ADVERTISEMENT_100H               (1 << 7)
#define ETHER_PHY_AN_ADVERTISEMENT_10F                (1 << 6)
#define ETHER_PHY_AN_ADVERTISEMENT_10H                (1 << 5)
#define ETHER_PHY_AN_ADVERTISEMENT_SELECTOR           (1 << 0)
#define ETHER_PHY_AN_ADVERTISEMENT_SELECTOR_MASK      (1 << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define ETHER_PHY_AN_LINK_PARTNER_NEXT_PAGE           (1 << 15)
#define ETHER_PHY_AN_LINK_PARTNER_ACK                 (1 << 14)
#define ETHER_PHY_AN_LINK_PARTNER_RM_FAULT            (1 << 13)
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR             (1 << 11)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE               (1 << 10)
#define ETHER_PHY_AN_LINK_PARTNER_100_T4              (1 << 9)
#define ETHER_PHY_AN_LINK_PARTNER_100F                (1 << 8)
#define ETHER_PHY_AN_LINK_PARTNER_100H                (1 << 7)
#define ETHER_PHY_AN_LINK_PARTNER_10F                 (1 << 6)
#define ETHER_PHY_AN_LINK_PARTNER_10H                 (1 << 5)
#define ETHER_PHY_AN_LINK_PARTNER_SELECTOR            (1 << 0)

/* 1000BASE-T Control */
#define ETHER_PHY_1000BT_CONTROL_1000F                (1 << 9)
#define ETHER_PHY_1000BT_CONTROL_1000H                (1 << 8)

/* 1000BASE-T Status */
#define ETHER_PHY_1000BT_STATUS_PARTNER_1000F         (1 << 11)
#define ETHER_PHY_1000BT_STATUS_PARTNER_1000H         (1 << 10)

#define ETHER_PHY_PIR_MDI_MASK                        (1 << 3)
#define ETHER_PHY_PIR_MDO_HIGH                        (0x04)
#define ETHER_PHY_PIR_MDO_LOW                         (0x00)
#define ETHER_PHY_PIR_MMD_WRITE                       (0x02)
#define ETHER_PHY_PIR_MMD_READ                        (0x00)
#define ETHER_PHY_PIR_MDC_HIGH                        (0x01)
#define ETHER_PHY_PIR_MDC_LOW                         (0x00)

#define ETHER_PHY_PREAMBLE_LENGTH                     (32U)
#define ETHER_PHY_WRITE_DATA_BIT_MASK                 (0x8000)

/* Define for MDIO_CFG_STATUS of MDIO_ETHSW */
#define ETHER_PHY_ETHSW_MDIO_CFG_STATUS_BUSY          (1 << 0)  /**< busy bit mask */
#define ETHER_PHY_ETHSW_MDIO_CFG_STATUS_READERR       (1 << 1)  /**< read error mask */

/* Define for MDIO_COMMAND of MDIO_ETHSW */
#define ETHER_PHY_ETHSW_MDIO_COMMAND_REGADDR_MASK     (0x1f)    /**< REG address mask */
#define ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_SHIFT    (5)       /**< PHY address shift */
                                                                /**< PHY address mask */
#define ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_MASK     (0x1f << ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_SHIFT)
#define ETHER_PHY_ETHSW_MDIO_COMMAND_TRANINIT_READ    (1 << 15) /**< transaction init read */

#define ETHER_PHY_16BIT_DATA_MASK                     (0xffff)

/* Define for delay time */
#define ETHER_PHY_DELAY_2US                           (2) /* 2us */

/* Initial value of down counter for timeout detection */
#define ETHER_PHY_TIMEOUT_COUNT                       (1000000000)

/* Illegal PHY register read value  */
#define ETHER_PHY_REGISTER_READ_ERROR                 (0xffff)

/* Bit definen of GMII_Address Register */
#define ETHER_PHY_GMII_ADDRESS_PA_SHIFT               (11)     /**< Physical Layer Address */
#define ETHER_PHY_GMII_ADDRESS_PA_MASK                (0x1f << ETHER_PHY_GMII_ADDRESS_PA_SHIFT)
#define ETHER_PHY_GMII_ADDRESS_GR_SHIFT               (6)      /**< GMII Register */
#define ETHER_PHY_GMII_ADDRESS_GR_MASK                (0x1f << ETHER_PHY_GMII_ADDRESS_GR_SHIFT)
#define ETHER_PHY_GMII_ADDRESS_CR                     (4 << 2) /**< CSR Clock Range */
#define ETHER_PHY_GMII_ADDRESS_GW                     (1 << 1) /**< GMII Write */
#define ETHER_PHY_GMII_ADDRESS_GB                     (1 << 0) /**< GMII Busy */

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t ether_phy_open_param_check(ether_phy_instance_ctrl_t   * p_instance_ctrl,
                                            ether_phy_cfg_t const * const p_cfg);

#endif

static ether_selector_speed_t  ether_phy_get_selector_speed(ether_phy_speed_t bps);
static ether_selector_duplex_t ether_phy_get_selector_duplex(ether_phy_duplex_t duplex);

static fsp_err_t ether_phy_reset(ether_phy_instance_ctrl_t * p_instance_ctrl, ether_phy_cfg_t const * const p_cfg);

static fsp_err_t ether_phy_set_auto_negotiate_off(ether_phy_instance_ctrl_t * p_instance_ctrl);
static fsp_err_t ether_phy_set_auto_negotiate_on(ether_phy_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t ether_phy_get_ability_auto_negotiate_on(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                         uint32_t * const            p_line_speed_duplex,
                                                         uint32_t * const            p_local_pause,
                                                         uint32_t * const            p_partner_pause);
static fsp_err_t ether_phy_get_ability_auto_negotiate_off(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                          uint32_t * const            p_line_speed_duplex,
                                                          uint32_t * const            p_local_pause,
                                                          uint32_t * const            p_partner_pause);

void ether_phy_convert_speed_duplex(uint32_t                  line_speed_duple,
                                    ether_selector_speed_t  * p_selector_speed,
                                    ether_selector_duplex_t * p_selector_duplex);

uint32_t        ether_phy_read(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
static uint32_t ether_phy_read_gmac(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr);
static uint32_t ether_phy_read_ethsw(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_add);
static uint32_t ether_phy_read_esc(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_add);

void        ether_phy_write(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);
static void ether_phy_write_gmac(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);
static void ether_phy_write_ethsw(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);
static void ether_phy_write_esc(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data);

static void ether_phy_targets_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_VSC8541) == ETHER_PHY_CHIP_VSC8541)
static void ether_phy_targets_initialize_vsc8541(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9131) == ETHER_PHY_CHIP_KSZ9131)
static void ether_phy_targets_initialize_ksz9131(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9031) == ETHER_PHY_CHIP_KSZ9031)
static void ether_phy_targets_initialize_ksz9031(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8081) == ETHER_PHY_CHIP_KSZ8081)
static void ether_phy_targets_initialize_ksz8081(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8041) == ETHER_PHY_CHIP_KSZ8041)
static void ether_phy_targets_initialize_ksz8041(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

/** ETHER_PHYHAL module version data structure */
static const fsp_version_t module_version =
{
    .api_version_minor  = ETHER_PHY_API_VERSION_MINOR,
    .api_version_major  = ETHER_PHY_API_VERSION_MAJOR,
    .code_version_major = ETHER_PHY_CODE_VERSION_MAJOR,
    .code_version_minor = ETHER_PHY_CODE_VERSION_MINOR
};

/** ETHER_PHY HAL API mapping for Ethernet PHY Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ether_phy_api_t g_ether_phy_on_ether_phy =
{
    .open                  = R_ETHER_PHY_Open,
    .close                 = R_ETHER_PHY_Close,
    .startAutoNegotiate    = R_ETHER_PHY_StartAutoNegotiate,
    .linkPartnerAbilityGet = R_ETHER_PHY_LinkPartnerAbilityGet,
    .linkStatusGet         = R_ETHER_PHY_LinkStatusGet,
    .versionGet            = R_ETHER_PHY_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/********************************************************************************************************************//**
 * Resets Ethernet PHY device. Implements @ref ether_phy_api_t::open.
 *
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_PHY control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN                    Control block has already been opened or channel is being used by another
 *                                                  instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_ARGUMENT                Invalid arguments.
 * @retval  FSP_ERR_TIMEOUT                         PHY-LSI Reset wait timeout.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open (ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    ether_phy_extend_cfg_t    * p_extend;
    ether_selector_instance_t * p_selector_instance;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)

    /** Check parameters. */
    err = ether_phy_open_param_check(p_instance_ctrl, p_cfg); /** check arguments */
    ETHER_PHY_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif

    p_extend = (ether_phy_extend_cfg_t *) p_cfg->p_extend;

    /* Initialize configuration of ethernet phy module. */
    p_instance_ctrl->p_ether_phy_cfg = p_cfg;

    switch (p_extend->mdio_type)
    {
        case ETHER_PHY_MDIO_GMAC:
        default:
        {
            p_instance_ctrl->p_reg_etherc = (uint32_t *) R_GMAC_BASE;
            break;
        }

        case ETHER_PHY_MDIO_ETHSW:
        {
            p_instance_ctrl->p_reg_etherc = (uint32_t *) R_ETHSW_BASE;
            break;
        }

        case ETHER_PHY_MDIO_ESC:
        {
            p_instance_ctrl->p_reg_etherc = (uint32_t *) R_ESC_BASE;
            break;
        }
    }

    p_instance_ctrl->local_advertise = 0;

    p_selector_instance = p_extend->p_selector_instance;

    err = p_selector_instance->p_api->open(p_selector_instance->p_ctrl, p_selector_instance->p_cfg);

    if (FSP_SUCCESS == err)
    {
        err =
            p_selector_instance->p_api->converterSet(p_selector_instance->p_ctrl,
                                                     ether_phy_get_selector_speed(p_extend->bps),
                                                     ether_phy_get_selector_duplex(p_extend->duplex));
    }

    if (FSP_SUCCESS == err)
    {
        if (p_extend->port_type == ETHER_PHY_PORT_TYPE_ETHERNET)
        {
            err = ether_phy_reset(p_instance_ctrl, p_cfg);
        }
    }

    if (FSP_SUCCESS == err)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        if (ETHER_PHY_AUTO_NEGOTIATION_ON == p_extend->auto_negotiation)
        {
            err = ether_phy_set_auto_negotiate_on(p_instance_ctrl);
        }
        else
        {
            err = ether_phy_set_auto_negotiate_off(p_instance_ctrl);
        }

        if (FSP_SUCCESS == err)
        {
            p_instance_ctrl->open = ETHER_PHY_OPEN;
        }
    }

    return err;
}                                      /* End of function R_ETHER_PHY_Open() */

/********************************************************************************************************************//**
 * Close Ethernet PHY device. Implements @ref ether_phy_api_t::close.
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Close (ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    ether_phy_extend_cfg_t    * p_extend;
    ether_selector_instance_t * p_selector_instance;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_extend            = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;
    p_selector_instance = p_extend->p_selector_instance;

    p_selector_instance->p_api->close(p_selector_instance->p_ctrl);

    /** Clear configure block parameters. */
    p_instance_ctrl->p_ether_phy_cfg = NULL;
    p_instance_ctrl->p_reg_etherc    = 0;
    p_instance_ctrl->p_reg_etherc    = 0;

    p_instance_ctrl->open = 0;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_Close() */

/********************************************************************************************************************//**
 * Starts auto-negotiate. Implements @ref ether_phy_api_t::startAutoNegotiate.
 *
 * @retval  FSP_SUCCESS             ETHER_PHY successfully starts auto-negotiate.
 * @retval  FSP_ERR_ASSERTION       Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN        The control block has not been opened.
 * @retval  FSP_ERR_INVALID_MODE    It was aborted the process because it is not auto-negotiation mode.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_StartAutoNegotiate (ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    ether_phy_extend_cfg_t    * p_extend;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_AUTO_NEGOTIATION_ON == (p_extend->auto_negotiation), FSP_ERR_INVALID_MODE);

    /* Restart Auto-Negoration */
    ether_phy_write(p_instance_ctrl,
                    ETHER_PHY_REG_CONTROL,
                    (ETHER_PHY_CONTROL_AN_ENABLE |
                     ETHER_PHY_CONTROL_AN_RESTART));

    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT);

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_StartAutoNegotiate() */

/********************************************************************************************************************//**
 * Reports the other side's physical capability. Implements @ref ether_phy_api_t::linkPartnerAbilityGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to arguments are NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 * @retval  FSP_ERR_ETHER_PHY_NOT_READY                 The auto-negotiation isn't completed
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet (ether_phy_ctrl_t * const p_ctrl,
                                             uint32_t * const         p_line_speed_duplex,
                                             uint32_t * const         p_local_pause,
                                             uint32_t * const         p_partner_pause)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    fsp_err_t                err;
    ether_phy_extend_cfg_t * p_extend;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(NULL != p_line_speed_duplex, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_local_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_partner_pause, FSP_ERR_INVALID_POINTER);
#endif

    p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    if (ETHER_PHY_AUTO_NEGOTIATION_ON == p_extend->auto_negotiation)
    {
        err = ether_phy_get_ability_auto_negotiate_on(p_instance_ctrl,
                                                      p_line_speed_duplex,
                                                      p_local_pause,
                                                      p_partner_pause);
        if (FSP_SUCCESS == err)
        {
            ether_selector_speed_t      selector_speed;
            ether_selector_duplex_t     selector_duplex;
            ether_selector_instance_t * p_selector_instance;

            ether_phy_convert_speed_duplex(*p_line_speed_duplex, &selector_speed, &selector_duplex);

            p_selector_instance = p_extend->p_selector_instance;

            err =
                p_selector_instance->p_api->converterSet(p_selector_instance->p_ctrl, selector_speed, selector_duplex);
        }
    }
    else
    {
        err = ether_phy_get_ability_auto_negotiate_off(p_instance_ctrl,
                                                       p_line_speed_duplex,
                                                       p_local_pause,
                                                       p_partner_pause);
    }

    return err;
}                                      /* End of function R_ETHER_PHY_LinkPartnerAbilityGet() */

/********************************************************************************************************************//**
 * Returns the status of the physical link. Implements @ref ether_phy_api_t::linkStatusGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkStatusGet (ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t  reg;
    fsp_err_t err = FSP_SUCCESS;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);

    /* When the link isn't up, return error */
    if ((ETHER_PHY_REGISTER_READ_ERROR == (reg & ETHER_PHY_REGISTER_READ_ERROR)) ||
        (ETHER_PHY_STATUS_LINK_UP != (reg & ETHER_PHY_STATUS_LINK_UP)))
    {
        /* Link is down */
        err = FSP_ERR_ETHER_PHY_ERROR_LINK;
    }
    else
    {
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;
}                                      /* End of function R_ETHER_PHY_LinkStatusGet() */

/********************************************************************************************************************//**
 * Provides API and code version in the user provided pointer. Implements @ref ether_phy_api_t::versionGet.
 *
 * @param[in] p_version   Version number set here
 *
 * @retval  FSP_SUCCESS                  Version information stored in provided p_version.
 * @retval  FSP_ERR_ASSERTION            p_version is NULL.
 ***********************************************************************************************************************/
__INLINE fsp_err_t R_ETHER_PHY_VersionGet (fsp_version_t * const p_version)
{
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_version);
#endif

    *p_version = module_version;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_VersionGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/**
 * Private functions
 */

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * Parameter error check function for open.
 *
 * @param[in] p_instance_ctrl   Pointer to the control block for the channel
 * @param[in] p_cfg             Pointer to pin configuration structure
 *
 * @retval  FSP_SUCCESS                  No parameter error found
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER control block or configuration structure is NULL
 * @retval  FSP_ERR_ALREADY_OPEN         Control block has already been opened
 * @retval  FSP_ERR_INVALID_CHANNEL      Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_ARGUMENT      Irq number lower then 0.
 **********************************************************************************************************************/
static fsp_err_t ether_phy_open_param_check (ether_phy_instance_ctrl_t   * p_instance_ctrl,
                                             ether_phy_cfg_t const * const p_cfg)
{
    ether_phy_extend_cfg_t * p_extend;

    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);

    p_extend = (ether_phy_extend_cfg_t *) p_cfg->p_extend;

    ETHER_PHY_ERROR_RETURN((ETHER_PHY_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
    ETHER_PHY_ERROR_RETURN((BSP_FEATURE_ETHER_PHY_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);

    ETHER_PHY_ERROR_RETURN(((ETHER_PHY_MDIO_GMAC == p_extend->mdio_type) ||
                            (ETHER_PHY_MDIO_ETHSW == p_extend->mdio_type) ||
                            (ETHER_PHY_MDIO_ESC == p_extend->mdio_type)),
                           FSP_ERR_INVALID_ARGUMENT);

    ETHER_PHY_ERROR_RETURN(((ETHER_PHY_SPEED_10_100 == p_extend->bps) ||
                            (ETHER_PHY_SPEED_10_1000 == p_extend->bps) ||
                            (ETHER_PHY_SPEED_10 == p_extend->bps) ||
                            (ETHER_PHY_SPEED_100 == p_extend->bps) ||
                            (ETHER_PHY_SPEED_1000 == p_extend->bps)),
                           FSP_ERR_INVALID_ARGUMENT);

    ETHER_PHY_ERROR_RETURN(((ETHER_PHY_DUPLEX_HALF == p_extend->duplex) ||
                            (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)),
                           FSP_ERR_INVALID_ARGUMENT);

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Get link speed for selector deiver.
 *
 * @param[in]   bps     Configured link speed for PHY driver
 *
 * @retval  Link speed for selector deiver
 **********************************************************************************************************************/
static ether_selector_speed_t ether_phy_get_selector_speed (ether_phy_speed_t bps)
{
    ether_selector_speed_t selector_speed;

    switch (bps)
    {
        case ETHER_PHY_SPEED_10_100:
        {
            selector_speed = ETHER_SELECTOR_SPEED_100MBPS;
            break;
        }

        case ETHER_PHY_SPEED_10_1000:
        {
            selector_speed = ETHER_SELECTOR_SPEED_1000MBPS;
            break;
        }

        case ETHER_PHY_SPEED_10:
        {
            selector_speed = ETHER_SELECTOR_SPEED_10MBPS;
            break;
        }

        case ETHER_PHY_SPEED_100:
        {
            selector_speed = ETHER_SELECTOR_SPEED_100MBPS;
            break;
        }

        case ETHER_PHY_SPEED_1000:
        {
            selector_speed = ETHER_SELECTOR_SPEED_1000MBPS;
            break;
        }

        default:
        {
            selector_speed = (ether_selector_speed_t) -1;
            break;
        }
    }

    return selector_speed;
}

/*******************************************************************************************************************//**
 * Get duplex for selector deiver.
 *
 * @param[in]   duplex  Configured duplex for PHY driver
 *
 * @retval  Duplex for selector deiver
 **********************************************************************************************************************/
static ether_selector_duplex_t ether_phy_get_selector_duplex (ether_phy_duplex_t duplex)
{
    ether_selector_duplex_t selector_duplex;

    switch (duplex)
    {
        case ETHER_PHY_DUPLEX_HALF:
        {
            selector_duplex = ETHER_SELECTOR_DUPLEX_HALF;
            break;
        }

        case ETHER_PHY_DUPLEX_FULL:
        {
            selector_duplex = ETHER_SELECTOR_DUPLEX_FULL;
            break;
        }

        default:
            selector_duplex = (ether_selector_duplex_t) -1;
    }

    return selector_duplex;
}

/*******************************************************************************************************************//**
 * PHY reset
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   p_cfg               Pointer to pin configuration structure
 *
 * @retval  FSP_SUCCESS             Successfully
 * @retval  FSP_ERR_TIMEOUT         Reset timeout
 **********************************************************************************************************************/
fsp_err_t ether_phy_reset (ether_phy_instance_ctrl_t * p_instance_ctrl, ether_phy_cfg_t const * const p_cfg)
{
    static bool g_ether_phy_state_initialized = false;

    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_cfg->p_extend;
    fsp_err_t                err      = FSP_SUCCESS;

    if (!g_ether_phy_state_initialized)
    {
        /* Reset PHY by hard */
        g_ether_phy_state_initialized = true;

        /* This code uses BSP IO functions to show how it is used.*/
        R_BSP_PinAccessEnable();

        /* Write Low-output to ESC_RESETOUT# as PHY reset */
        R_BSP_PinWrite(p_extend->phy_reset_pin, BSP_IO_LEVEL_LOW);

        /* Assertion time for reset  */
        R_BSP_SoftwareDelay(p_extend->phy_reset_time, BSP_DELAY_UNITS_MICROSECONDS);

        /* Write High-output to ESC_RESETOUT# as release */
        R_BSP_PinWrite(p_extend->phy_reset_pin, BSP_IO_LEVEL_HIGH);

        /* Delay after release */
        R_BSP_SoftwareDelay(p_extend->phy_reset_time, BSP_DELAY_UNITS_MICROSECONDS);

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();
    }
    else
    {
        if (ETHER_PHY_AUTO_NEGOTIATION_ON == p_extend->auto_negotiation)
        {
            uint32_t reg;
            uint32_t count = 0;

            /* Reset PHY by soft */
            ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);

            while (1)
            {
                reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);

                if ((ETHER_PHY_REGISTER_READ_ERROR != (reg & ETHER_PHY_REGISTER_READ_ERROR)) &&
                    (0 == (reg & ETHER_PHY_CONTROL_RESET)))
                {
                    break;
                }

                if (count > p_cfg->phy_reset_wait_time)
                {
                    err = FSP_ERR_TIMEOUT;
                    break;
                }

                count++;
            }
        }
    }

    return err;
}                                      /* End of function ether_phy_reset() */

/*******************************************************************************************************************//**
 * Sets the communication conditions when auto-negotiation is OFF.
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval  FSP_SUCCESS                                 Successfully
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid arguments.
 **********************************************************************************************************************/
fsp_err_t ether_phy_set_auto_negotiate_off (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    uint32_t reg = 0;

    switch (p_extend->bps)
    {
        case ETHER_PHY_SPEED_10:
        {
            break;
        }

        case ETHER_PHY_SPEED_100:
        {
            reg |= ETHER_PHY_CONTROL_100_MBPS;
            break;
        }

        case ETHER_PHY_SPEED_1000:
        {
            reg |= ETHER_PHY_CONTROL_SPEED_SELCT;
            break;
        }

        case ETHER_PHY_SPEED_10_100:
        case ETHER_PHY_SPEED_10_1000:
        default:

            return FSP_ERR_INVALID_ARGUMENT;
    }

    switch (p_extend->duplex)
    {
        case ETHER_PHY_DUPLEX_HALF:
        {
            reg &= ~(uint32_t) ETHER_PHY_CONTROL_FULL_DUPLEX;
            break;
        }

        case ETHER_PHY_DUPLEX_FULL:
        {
            reg |= ETHER_PHY_CONTROL_FULL_DUPLEX;
            break;
        }

        default:

            return FSP_ERR_INVALID_ARGUMENT;
    }

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, reg);

    p_instance_ctrl->local_advertise = 0;

    return FSP_SUCCESS;
}                                      /* End of function ether_phy_set_auto_negotiate_off() */

/*******************************************************************************************************************//**
 * Sets the advertisement when auto-negotiation is ON.
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval  FSP_SUCCESS                                 Successfully
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid arguments.
 **********************************************************************************************************************/
fsp_err_t ether_phy_set_auto_negotiate_on (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    uint32_t reg_adv = ETHER_PHY_AN_ADVERTISEMENT_SELECTOR;
    uint32_t reg_ctr = 0;

    switch (p_extend->bps)
    {
        case ETHER_PHY_SPEED_10_100:
        {
            if (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H |
                           ETHER_PHY_AN_ADVERTISEMENT_100H |
                           ETHER_PHY_AN_ADVERTISEMENT_10F |
                           ETHER_PHY_AN_ADVERTISEMENT_100F;
            }
            else
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H |
                           ETHER_PHY_AN_ADVERTISEMENT_100H;
            }

            break;
        }

        case ETHER_PHY_SPEED_10_1000:
        {
            if (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H |
                           ETHER_PHY_AN_ADVERTISEMENT_100H |
                           ETHER_PHY_AN_ADVERTISEMENT_10F |
                           ETHER_PHY_AN_ADVERTISEMENT_100F;

                reg_ctr |= ETHER_PHY_1000BT_CONTROL_1000H |
                           ETHER_PHY_1000BT_CONTROL_1000F;
            }
            else
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H |
                           ETHER_PHY_AN_ADVERTISEMENT_100H;

                reg_ctr |= ETHER_PHY_1000BT_CONTROL_1000H;
            }

            break;
        }

        case ETHER_PHY_SPEED_10:
        {
            if (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H |
                           ETHER_PHY_AN_ADVERTISEMENT_10F;
            }
            else
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_10H;
            }

            break;
        }

        case ETHER_PHY_SPEED_100:
        {
            if (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_100H |
                           ETHER_PHY_AN_ADVERTISEMENT_100F;
            }
            else
            {
                reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_100H;
            }

            break;
        }

        case ETHER_PHY_SPEED_1000:
        {
            if (ETHER_PHY_DUPLEX_FULL == p_extend->duplex)
            {
                reg_ctr |= ETHER_PHY_1000BT_CONTROL_1000H |
                           ETHER_PHY_1000BT_CONTROL_1000F;
            }
            else
            {
                reg_ctr |= ETHER_PHY_1000BT_CONTROL_1000H;
            }

            break;
        }

        default:

            return FSP_ERR_INVALID_ARGUMENT;
    }

    /* When pause frame is used */
    if (ETHER_PHY_FLOW_CONTROL_ENABLE == p_instance_ctrl->p_ether_phy_cfg->flow_control)
    {
        reg_adv |= ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR |
                   ETHER_PHY_AN_ADVERTISEMENT_PAUSE;
    }

    p_instance_ctrl->local_advertise = reg_adv;

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, reg_adv);
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_1000BT_CONTROL, reg_ctr);

    return FSP_SUCCESS;
}                                      /* End of function ether_phy_set_auto_negotiate_on() */

/********************************************************************************************************************//**
 * Gets link ability when auto-negotiation is OFF.
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[out] p_line_speed_duplex  Pointer to the location of both the line speed and the duplex.
 * @param[out] p_local_pause        Pointer to the location to store the local pause bits.
 * @param[out] p_partner_pause      Pointer to the location to store the partner pause bits.
 *
 * @retval  FSP_SUCCESS             ETHER_PHY successfully get link partner ability.
 ***********************************************************************************************************************/
fsp_err_t ether_phy_get_ability_auto_negotiate_off (ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                    uint32_t * const            p_line_speed_duplex,
                                                    uint32_t * const            p_local_pause,
                                                    uint32_t * const            p_partner_pause)
{
    uint32_t reg;

    reg = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_CONTROL);

    if (ETHER_PHY_CONTROL_SPEED_SELCT == (reg & ETHER_PHY_CONTROL_SPEED_SELCT))
    {
        if (ETHER_PHY_CONTROL_FULL_DUPLEX == (reg & ETHER_PHY_CONTROL_FULL_DUPLEX))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000F;
        }
        else
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000H;
        }
    }
    else if (ETHER_PHY_CONTROL_100_MBPS == (reg & ETHER_PHY_CONTROL_100_MBPS))
    {
        if (ETHER_PHY_CONTROL_FULL_DUPLEX == (reg & ETHER_PHY_CONTROL_FULL_DUPLEX))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100F;
        }
        else
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100H;
        }
    }
    else
    {
        if (ETHER_PHY_CONTROL_FULL_DUPLEX == (reg & ETHER_PHY_CONTROL_FULL_DUPLEX))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10F;
        }
        else
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10H;
        }
    }

    *p_local_pause   = 0;
    *p_partner_pause = 0;

    return FSP_SUCCESS;
}                                      /* End of function ether_phy_get_ability_auto_negotiate_off() */

/********************************************************************************************************************//**
 * Gets link ability when auto-negotiation is ON.
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[out] p_line_speed_duplex  Pointer to the location of both the line speed and the duplex.
 * @param[out] p_local_pause        Pointer to the location to store the local pause bits.
 * @param[out] p_partner_pause      Pointer to the location to store the partner pause bits.
 *
 * @retval  FSP_SUCCESS                         ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK        PHY-LSI is not link up.
 * @retval  FSP_ERR_ETHER_PHY_NOT_READY         The auto-negotiation isn't completed
 ***********************************************************************************************************************/
fsp_err_t ether_phy_get_ability_auto_negotiate_on (ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                   uint32_t * const            p_line_speed_duplex,
                                                   uint32_t * const            p_local_pause,
                                                   uint32_t * const            p_partner_pause)
{
    uint32_t reg_status;
    uint32_t reg_4;
    uint32_t reg_5;
    uint32_t reg_9;
    uint32_t reg_10;

    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);
    reg_status = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_STATUS);

    /* When the link isn't up, return error */
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_LINK_UP == (reg_status & ETHER_PHY_STATUS_LINK_UP),
                           FSP_ERR_ETHER_PHY_ERROR_LINK);

    /* When the auto-negotiation isn't completed, return error */
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_AN_COMPLETE == (reg_status & ETHER_PHY_STATUS_AN_COMPLETE),
                           FSP_ERR_ETHER_PHY_NOT_READY);

    reg_4 = p_instance_ctrl->local_advertise;
    reg_5 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_AN_LINK_PARTNER);

    /* Establish local pause capability */
    *p_local_pause = 0;

    if (ETHER_PHY_AN_ADVERTISEMENT_PAUSE == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_PAUSE))
    {
        (*p_local_pause) |= (1 << 1);
    }

    if (ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR))
    {
        (*p_local_pause) |= 1;
    }

    /* Establish partner pause capability */
    *p_partner_pause = 0;

    if (ETHER_PHY_AN_LINK_PARTNER_PAUSE == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_PAUSE))
    {
        (*p_partner_pause) |= (1 << 1);
    }

    if (ETHER_PHY_AN_LINK_PARTNER_ASM_DIR == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_ASM_DIR))
    {
        (*p_partner_pause) |= 1;
    }

    if ((ETHER_PHY_AN_LINK_PARTNER_10H == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_10H)) &&
        (ETHER_PHY_AN_ADVERTISEMENT_10H == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_10H)))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10H;
    }

    if ((ETHER_PHY_AN_LINK_PARTNER_10F == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_10F)) &&
        (ETHER_PHY_AN_ADVERTISEMENT_10F == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_10F)))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10F;
    }

    if ((ETHER_PHY_AN_LINK_PARTNER_100H == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_100H)) &&
        (ETHER_PHY_AN_ADVERTISEMENT_100H == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_100H)))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100H;
    }

    if ((ETHER_PHY_AN_LINK_PARTNER_100F == (reg_5 & ETHER_PHY_AN_LINK_PARTNER_100F)) &&
        (ETHER_PHY_AN_ADVERTISEMENT_100F == (reg_4 & ETHER_PHY_AN_ADVERTISEMENT_100F)))
    {
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_100F;
    }

    if (reg_status & ETHER_PHY_STATUS_EX_STATUS)
    {
        reg_9  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_1000BT_CONTROL);
        reg_10 = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_1000BT_STATUS);

        if ((ETHER_PHY_1000BT_STATUS_PARTNER_1000H == (reg_10 & ETHER_PHY_1000BT_STATUS_PARTNER_1000H)) &&
            (ETHER_PHY_1000BT_CONTROL_1000H == (reg_9 & ETHER_PHY_1000BT_CONTROL_1000H)))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000H;
        }

        if ((ETHER_PHY_1000BT_STATUS_PARTNER_1000F == (reg_10 & ETHER_PHY_1000BT_STATUS_PARTNER_1000F)) &&
            (ETHER_PHY_1000BT_CONTROL_1000F == (reg_9 & ETHER_PHY_1000BT_CONTROL_1000F)))
        {
            (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_1000F;
        }
    }

    return FSP_SUCCESS;
}                                      /* End of function ether_phy_get_ability_auto_negotiate_on() */

/********************************************************************************************************************//**
 * Convert link speed and duplex for sector driver
 *
 * @param[in]   line_speed_duple    Link speed and duplex
 * @param[out]  p_selector_speed    Speed for sector driver
 * @param[out]  p_selector_duplex   Duplex for sector driver
 *
 * @retval  none
 ***********************************************************************************************************************/
void ether_phy_convert_speed_duplex (uint32_t                  line_speed_duple,
                                     ether_selector_speed_t  * p_selector_speed,
                                     ether_selector_duplex_t * p_selector_duplex)
{
    switch (line_speed_duple)
    {
        case ETHER_PHY_LINK_SPEED_10H:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_10MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_HALF;
            break;
        }

        case ETHER_PHY_LINK_SPEED_10F:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_10MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_FULL;
            break;
        }

        case ETHER_PHY_LINK_SPEED_100H:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_100MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_HALF;
            break;
        }

        case ETHER_PHY_LINK_SPEED_100F:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_100MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_FULL;
            break;
        }

        case ETHER_PHY_LINK_SPEED_1000H:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_1000MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_HALF;
            break;
        }

        case ETHER_PHY_LINK_SPEED_1000F:
        default:
        {
            *p_selector_speed  = ETHER_SELECTOR_SPEED_1000MBPS;
            *p_selector_duplex = ETHER_SELECTOR_DUPLEX_FULL;
            break;
        }
    }
}

/*******************************************************************************************************************//**
 * Reads a PHY register
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 *
 * @retval      read value
 **********************************************************************************************************************/
uint32_t ether_phy_read (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    switch (p_extend->mdio_type)
    {
        case ETHER_PHY_MDIO_GMAC:
        {
            return ether_phy_read_gmac(p_instance_ctrl, reg_addr);
        }

        case ETHER_PHY_MDIO_ETHSW:
        {
            return ether_phy_read_ethsw(p_instance_ctrl, reg_addr);
        }

        case ETHER_PHY_MDIO_ESC:
        {
            return ether_phy_read_esc(p_instance_ctrl, reg_addr);
        }

        default:

            return (uint32_t) -1;
    }
}                                      /* End of function ether_phy_read() */

/*******************************************************************************************************************//**
 * Reads a PHY register by GMAC control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 *
 * @retval      read value
 **********************************************************************************************************************/
uint32_t ether_phy_read_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
    volatile R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;
    uint32_t               timeout;
    uint32_t               val_tmp;

    val_tmp =
        ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHER_PHY_GMII_ADDRESS_PA_SHIFT) &
         ETHER_PHY_GMII_ADDRESS_PA_MASK) |
        ((reg_addr << ETHER_PHY_GMII_ADDRESS_GR_SHIFT) & ETHER_PHY_GMII_ADDRESS_GR_MASK) | ///< Phy reg address
        ETHER_PHY_GMII_ADDRESS_CR |                                                        ///< Speed
        ETHER_PHY_GMII_ADDRESS_GB;                                                         ///< Busy

    p_reg_etherc->GMII_Address = val_tmp;

    /* calculate timeout based on current timestamp */
    timeout = ETHER_PHY_TIMEOUT_COUNT;

    while (1)
    {
        /* Wait read complete */
        if (!(p_reg_etherc->GMII_Address_b.GB))
        {

            /* Return phy value */
            return p_reg_etherc->GMII_Data & ETHER_PHY_16BIT_DATA_MASK;
        }

        if (0 == timeout)
        {
            /* Timeout */
            break;
        }

        timeout--;
    }

    return (uint32_t) -1;
}                                      /* End of function ether_phy_read_gmac() */

/*******************************************************************************************************************//**
 * Reads a PHY register by ETHSW control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 *
 * @retval      read value
 **********************************************************************************************************************/
uint32_t ether_phy_read_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
    R_ETHSW_Type * p_Reg_Ethsw = (R_ETHSW_Type *) p_instance_ctrl->p_reg_etherc;
    uint32_t       timeout;
    uint32_t       reg;

    /* initialize and execute read command */
    reg  = reg_addr & ETHER_PHY_ETHSW_MDIO_COMMAND_REGADDR_MASK;
    reg |= ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_SHIFT) &
            ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_MASK);
    reg |= ETHER_PHY_ETHSW_MDIO_COMMAND_TRANINIT_READ;

    p_Reg_Ethsw->MDIO_COMMAND = reg;

    /* calculate timeout based on current timestamp */
    timeout = ETHER_PHY_TIMEOUT_COUNT;

    /* wait until MDIO is ready */
    while (1)
    {
        reg = p_Reg_Ethsw->MDIO_CFG_STATUS;

        if (ETHER_PHY_ETHSW_MDIO_CFG_STATUS_BUSY != (reg & ETHER_PHY_ETHSW_MDIO_CFG_STATUS_BUSY))
        {
            if (ETHER_PHY_ETHSW_MDIO_CFG_STATUS_READERR != (reg & ETHER_PHY_ETHSW_MDIO_CFG_STATUS_READERR))
            {

                /* Return phy value */
                return p_Reg_Ethsw->MDIO_DATA & ETHER_PHY_16BIT_DATA_MASK;
            }

            break;
        }

        if (0 == timeout)
        {
            /* Timeout */
            break;
        }

        timeout--;
    }

    return (uint32_t) -1;
}                                      /* End of function ether_phy_read_ethsw() */

/*******************************************************************************************************************//**
 * Reads a PHY register by ESC control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 *
 * @retval      read value
 **********************************************************************************************************************/
uint32_t ether_phy_read_esc (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr)
{
    R_ESC_Type * p_Reg_Esc = (R_ESC_Type *) p_instance_ctrl->p_reg_etherc;
    uint16_t     reg_tmp;
    uint32_t     timeout;

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_CONT_STAT_b.BUSY == 1)
    {
        if (0 == timeout)
        {

            /* Timeout */
            return (uint32_t) -1;
        }

        timeout--;
    }

    /* Get PDI access right */
    p_Reg_Esc->MII_PDI_ACS_STAT_b.ACSMII = 1;

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_ECAT_ACS_STAT_b.ACSMII == 1)
    {
        if (0 == timeout)
        {

            /* Timeout */
            return (uint32_t) -1;
        }

        timeout--;
    }

    /* Read PHY register */
    p_Reg_Esc->PHY_ADR       = p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address;
    p_Reg_Esc->PHY_REG_ADR   = (uint8_t) reg_addr;
    p_Reg_Esc->MII_CONT_STAT = (1 << 8) | (1 << 0); ///< Read command

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_CONT_STAT_b.BUSY == 1)
    {
        if (0 == timeout)
        {

            /* Timeout */
            return (uint32_t) -1;
        }

        timeout--;
    }

    /* Check the error bit */
    if ((p_Reg_Esc->MII_CONT_STAT_b.CMDERR == 1) ||
        (p_Reg_Esc->MII_CONT_STAT_b.READERR == 1))
    {
        /* Error operation */

        // Clear error bit
        // p_Reg_Esc->MIICONTSTAT.WORD = 0x0000;
    }

    /* Get read data */
    reg_tmp = p_Reg_Esc->PHY_DATA;

    /* Give the access right to ECAT */
    p_Reg_Esc->MII_PDI_ACS_STAT_b.ACSMII = 0;

    return reg_tmp;
}                                      /* End of function ether_phy_read_esc() */

/*******************************************************************************************************************//**
 * Writes to a PHY register
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 * @param[in]   data                Value
 *
 * @retval      None
 **********************************************************************************************************************/
void ether_phy_write (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    switch (p_extend->mdio_type)
    {
        case ETHER_PHY_MDIO_GMAC:
        {
            ether_phy_write_gmac(p_instance_ctrl, reg_addr, data);
            break;
        }

        case ETHER_PHY_MDIO_ETHSW:
        {
            ether_phy_write_ethsw(p_instance_ctrl, reg_addr, data);
            break;
        }

        case ETHER_PHY_MDIO_ESC:
        {
            ether_phy_write_esc(p_instance_ctrl, reg_addr, data);
            break;
        }

        default:
        {
            break;
        }
    }
}                                      /* End of function ether_phy_write() */

/***********************************************************************************************************************
 * Writes to a PHY register by GMAC control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 * @param[in]   data                value
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_write_gmac (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
    volatile R_GMAC_Type * p_reg_etherc = (R_GMAC_Type *) p_instance_ctrl->p_reg_etherc;
    uint32_t               timeout;
    uint32_t               val_tmp;

    val_tmp =
        ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHER_PHY_GMII_ADDRESS_PA_SHIFT) &
         ETHER_PHY_GMII_ADDRESS_PA_MASK) |
        ((reg_addr << ETHER_PHY_GMII_ADDRESS_GR_SHIFT) & ETHER_PHY_GMII_ADDRESS_GR_MASK) | ///< Phy reg address
        ETHER_PHY_GMII_ADDRESS_CR |                                                        ///< Speed
        ETHER_PHY_GMII_ADDRESS_GW |                                                        ///< Write
        ETHER_PHY_GMII_ADDRESS_GB;                                                         ///< Busy

    p_reg_etherc->GMII_Data    = data;                                                     ///< Set write value
    p_reg_etherc->GMII_Address = val_tmp;                                                  ///< Write phy register

    /* calculate timeout based on current timestamp */
    timeout = ETHER_PHY_TIMEOUT_COUNT;

    while (1)
    {
        /* wait write complete */
        if (!(p_reg_etherc->GMII_Address_b.GB))
        {
            /* Sucess */
            break;
        }

        if (0 == timeout)
        {
            /* Timeout */
            break;
        }

        timeout--;
    }
}                                      /* End of function ether_phy_write_gmac() */

/*******************************************************************************************************************//**
 * Writes to a PHY register by ETHSW control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 * @param[in]   data                value
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_write_ethsw (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
    R_ETHSW_Type * p_Reg_Ethsw = (R_ETHSW_Type *) p_instance_ctrl->p_reg_etherc;
    uint32_t       timeout;
    uint32_t       reg;

    /* initialize and execute write command */
    reg  = reg_addr & ETHER_PHY_ETHSW_MDIO_COMMAND_REGADDR_MASK;
    reg |= ((p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address << ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_SHIFT) &
            ETHER_PHY_ETHSW_MDIO_COMMAND_PHYADDR_MASK);

    p_Reg_Ethsw->MDIO_COMMAND = reg;
    p_Reg_Ethsw->MDIO_DATA    = data & ETHER_PHY_16BIT_DATA_MASK;

    /* calculate timeout based on current timestamp */
    timeout = ETHER_PHY_TIMEOUT_COUNT;

    /* wait until MDIO is ready */
    while (1)
    {
        reg = p_Reg_Ethsw->MDIO_CFG_STATUS;

        if (ETHER_PHY_ETHSW_MDIO_CFG_STATUS_BUSY != (reg & ETHER_PHY_ETHSW_MDIO_CFG_STATUS_BUSY))
        {
            break;
        }

        if (0 == timeout)
        {
            /* Timeout */
            break;
        }

        timeout--;
    }
}                                      /* End of function ether_phy_write_ethsw() */

/*******************************************************************************************************************//**
 * Writes to a PHY register by ESC control
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 * @param[in]   reg_addr            Address of the PHY register
 * @param[in]   data                value
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_write_esc (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t reg_addr, uint32_t data)
{
    R_ESC_Type * p_Reg_Esc = (R_ESC_Type *) p_instance_ctrl->p_reg_etherc;
    uint32_t     timeout;

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_CONT_STAT_b.BUSY == 1)
    {
        if (0 == timeout)
        {
            return;
        }

        timeout--;
    }

    /* Get PDI access right */
    p_Reg_Esc->MII_PDI_ACS_STAT_b.ACSMII = 1;

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_ECAT_ACS_STAT_b.ACSMII == 1)
    {
        if (0 == timeout)
        {
            return;
        }

        timeout--;
    }

    /* Write PHY register */
    p_Reg_Esc->PHY_ADR       = p_instance_ctrl->p_ether_phy_cfg->phy_lsi_address;
    p_Reg_Esc->PHY_REG_ADR   = (uint8_t) reg_addr;
    p_Reg_Esc->PHY_DATA      = (uint16_t) data;
    p_Reg_Esc->MII_CONT_STAT = (2 << 8) | (1 << 0); ///< Write command

    /* WAIT_LOOP */
    timeout = ETHER_PHY_TIMEOUT_COUNT;
    while (p_Reg_Esc->MII_CONT_STAT_b.BUSY == 1)
    {
        if (0 == timeout)
        {
            return;
        }

        timeout--;
    }

    /* Check the error bit */
    if ((p_Reg_Esc->MII_CONT_STAT_b.CMDERR == 1) ||
        (p_Reg_Esc->MII_CONT_STAT_b.READERR == 1))
    {
        /*  Error operation */

        // Clear error bit
        // p_Reg_Esc->MIICONTSTAT.WORD = 0x0000;
    }

    /* Give the access right to ECAT */
    p_Reg_Esc->MII_PDI_ACS_STAT_b.ACSMII = 0;
}                                      /* End of function ether_phy_write_esc() */

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    ether_phy_extend_cfg_t * p_extend = (ether_phy_extend_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend;

    switch (p_extend->phy_chip)
    {
#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_VSC8541) == ETHER_PHY_CHIP_VSC8541)
        case ETHER_PHY_CHIP_VSC8541:
        {
            ether_phy_targets_initialize_vsc8541(p_instance_ctrl);
            break;
        }
#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9131) == ETHER_PHY_CHIP_KSZ9131)
        case ETHER_PHY_CHIP_KSZ9131:
        {
            ether_phy_targets_initialize_ksz9131(p_instance_ctrl);
            break;
        }
#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9031) == ETHER_PHY_CHIP_KSZ9031)
        case ETHER_PHY_CHIP_KSZ9031:
        {
            ether_phy_targets_initialize_ksz9031(p_instance_ctrl);
            break;
        }
#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8081) == ETHER_PHY_CHIP_KSZ8081)
        case ETHER_PHY_CHIP_KSZ8081:
        {
            ether_phy_targets_initialize_ksz8081(p_instance_ctrl);
            break;
        }
#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8041) == ETHER_PHY_CHIP_KSZ8041)
        case ETHER_PHY_CHIP_KSZ8041:
        {
            ether_phy_targets_initialize_ksz8041(p_instance_ctrl);
            break;
        }
#endif

        default:
        {
            break;
        }
    }
}                                      /* End of function ether_phy_targets_initialize() */

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_VSC8541) == ETHER_PHY_CHIP_VSC8541)

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing for VSC8541
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize_vsc8541 (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    /* Vendor Specific PHY Registers */
 #define ETHER_PHY_REG_LED_MODE_SELECT                 (0x1D)
 #define ETHER_PHY_REG_LED_BEHAVIOR                    (0x1E)
 #define ETHER_PHY_REG_EXTEND_GPIO_PAGE                (0x1F)

 #define ETHER_PHY_REG_LED0_MODE_SELECT_OFFSET         (0x0)
 #define ETHER_PHY_REG_LED1_MODE_SELECT_OFFSET         (0x4)
 #define ETHER_PHY_REG_LED0_FEATURE_DISABLE_OFFSET     (0x0)
 #define ETHER_PHY_REG_LED1_FEATURE_DISABLE_OFFSET     (0x1)

 #define ETHER_PHY_LEDMODE_ANYSPEED_LINK_ACTIVITY      (0x0)
 #define ETHER_PHY_LEDMODE_1000M_LINK_ACTIVITY         (0x1)
 #define ETHER_PHY_LEDMODE_100M_LINK_ACTIVITY          (0x2)
 #define ETHER_PHY_LEDMODE_10M_LINK_ACTIVITY           (0x3)
 #define ETHER_PHY_LEDMODE_100M_1000M_LINK_ACTIVITY    (0x4)
 #define ETHER_PHY_LEDMODE_10M_100M_LINK_ACTIVITY      (0x5)
 #define ETHER_PHY_LEDMODE_BIT_MASK                    (0xF)

    /* Vendor Specific PHY Registers: Extended Page 2 */
 #define ETHER_PHY_REG_EXPAGE2_RGMII_CTRL              (0x14)

 #define ETHER_PHY_TXCLKDLY_OFFSET                     (0)
 #define ETHER_PHY_RXCLKDLY_OFFSET                     (4)
 #define ETHER_PHY_TXCLKDLY_MASK                       (0x7)
 #define ETHER_PHY_RXCLKDLY_MASK                       (0x7)

    uint32_t reg;

    /*
     * When VSC8541-02 of the Microsemi Corporation is used,
     * the pin that outputs the state of LINK is used combinedly with LINK/ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     * LED Mode Select (0x1D)
     * b7:4 = 0000b : LED1 mode any speed LINK/ACTIVITY
     * b3:0 = 0000b : LED0 mode any speed LINK/ACTIVITY
     * LED Behavior (0x1E)
     * b1 = 1 : LED1 combination feature disable (link only, duplex only)
     * b0 = 0 : LED0 combination feature enable  (link/activity)
     *//* LED Mode Select */
    ether_phy_write(p_instance_ctrl,
                    ETHER_PHY_REG_LED_MODE_SELECT,
                    (ETHER_PHY_LEDMODE_ANYSPEED_LINK_ACTIVITY << ETHER_PHY_REG_LED1_MODE_SELECT_OFFSET) |
                    (ETHER_PHY_LEDMODE_ANYSPEED_LINK_ACTIVITY << ETHER_PHY_REG_LED0_MODE_SELECT_OFFSET));

    /* LED Behavior */
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_LED_BEHAVIOR);
    reg &= ~(1U << ETHER_PHY_REG_LED0_FEATURE_DISABLE_OFFSET);
    reg |= 1U << ETHER_PHY_REG_LED1_FEATURE_DISABLE_OFFSET;
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_LED_BEHAVIOR, reg);
}                                      /* End of function ether_phy_targets_initialize() */

#endif /* ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_VSC8541) == ETHER_PHY_CHIP_VSC8541) */

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9131) == ETHER_PHY_CHIP_KSZ9131)

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing for KSZ9131
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize_ksz9131 (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    /* Vendor Specific PHY Registers */
 #define ETHER_PHY_REG_MMD_ACCESS_CONTROL           (0x0D)
 #define ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET    (0xE)
 #define ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA     (0x0E)

 #define ETHER_PHY_MMD_DEVICE_ADDRESS_2             (2)

 #define ETHER_PHY_MMD_REGISTER_77                  (77)

    uint32_t reg;

    /* 2.77 TX DLL */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_77);

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    reg &= ~(1U << 12);                /* Enable TX DLL */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, (uint16_t) reg);

    /* Reset TX DLL */
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    reg |= (1U << 13);                 /* Set Reset bit */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, (uint16_t) reg);

    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    reg &= ~(1U << 13);                /* Clear reset bit */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, (uint16_t) reg);
}                                      /* End of function ether_phy_targets_initialize() */

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9031) == ETHER_PHY_CHIP_KSZ9031)

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing for KSZ9031
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize_ksz9031 (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
 #define ETHER_PHY_REG_MMD_ACCESS_CONTROL           (0x0D)
 #define ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA     (0x0E)
 #define ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET    (0xE)
 #define ETHER_PHY_REG_LED_OVERRIDE_OFFSET          (0x4)

 #define ETHER_PHY_MMD_DEVICE_ADDRESS_2             (2)
 #define ETHER_PHY_MMD_REGISTER_0                   (0)
 #define ETHER_PHY_MMD_REGISTER_6                   (6)
 #define ETHER_PHY_MMD_REGISTER_8                   (8)
 #define ETHER_PHY_MMD_REGISTER_77                  (77)

 #define ETHER_PHY_MMD_REGISTER_6_KSZ9031_DATA      (0)
 #define ETHER_PHY_MMD_REGISTER_8_KSZ9031_DATA      ((31 << 5) | (7 << 0))
 #define ETHER_PHY_MMD_REGISTER_77_KSZ9031_DATA     (0x0851)

    uint32_t reg;

    /*
     * When KSZ9031 of the Micrel, Inc. is used,
     * the pin that outputs the state of LINK is used combinedly with LINK/ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     * MMD Device Addres - 2
     * MMD Register      - 0
     * LED Mode Override Mode 2.0.4
     * b4 = 1 : Single LED Mode (LINK)
     * b4 = 0 : Dual LED Mode (LINK/ACTIVITY)
     * The b4 is write only. The value of b4 can be read by b3.
     */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_0);

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA);
    reg |= 1 << ETHER_PHY_REG_LED_OVERRIDE_OFFSET;

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, reg);

    /* 2.8 Clock Pad skew */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_8);

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_8_KSZ9031_DATA);

    /* 2.6 TX Data Pad skew */
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (0 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_6);

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_CONTROL,
                    (1 << ETHER_PHY_REG_MMD_OPERATION_MODE_OFFSET | ETHER_PHY_MMD_DEVICE_ADDRESS_2));

    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_MMD_ACCESS_REGISTER_DATA, ETHER_PHY_MMD_REGISTER_6_KSZ9031_DATA);
}                                      /* End of function ether_phy_targets_initialize() */

#endif /* ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ9031) == ETHER_PHY_CHIP_KSZ9031) */

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8081) == ETHER_PHY_CHIP_KSZ8081)

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing for KSZ8081
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize_ksz8081 (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    (void) p_instance_ctrl;
}                                      /* End of function ether_phy_targets_initialize() */

#endif

#if ((ETHER_PHY_CFG_PHY_LSI & ETHER_PHY_CHIP_KSZ8041) == ETHER_PHY_CHIP_KSZ8041)

/*******************************************************************************************************************//**
 * PHY-LSI specific initialization processing for KSZ8041
 *
 * @param[in]   p_instance_ctrl     Pointer to the control block for the channel
 *
 * @retval      none
 **********************************************************************************************************************/
void ether_phy_targets_initialize_ksz8041 (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    /* Vendor Specific PHY Registers */
 #define ETHER_PHY_REG_PHY_CONTROL_1    (0x1E)

    uint32_t reg;

    /*
     * When KSZ8041NL of the Micrel, Inc. is used,
     * the pin that outputs the state of LINK is used combinedly with ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     */
    reg  = ether_phy_read(p_instance_ctrl, ETHER_PHY_REG_PHY_CONTROL_1);
    reg &= ~(1U << 15);
    reg |= (1U << 14);
    ether_phy_write(p_instance_ctrl, ETHER_PHY_REG_PHY_CONTROL_1, reg);
}                                      /* End of function ether_phy_targets_initialize() */

#endif
