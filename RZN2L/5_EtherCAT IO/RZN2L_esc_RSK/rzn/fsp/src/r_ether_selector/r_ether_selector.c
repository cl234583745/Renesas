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
 * Includes
 **********************************************************************************************************************/
#include "r_ether_selector.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "ETHS" in ASCII, used to determine if channel is open. */
#define ETHER_SELECTOR_OPEN                       (('E' << 24U) | ('T' << 16U) | ('H' << 8U) | ('S' << 0U))

/*CONVCTRL[m] register defines*/
#define ETHER_SELECTOR_FULLD                      (1 << 8)

#define ETHER_SELECTOR_CONV_MODE_MASK             (0x1f) /* Mask of Converter operation mode */
#define ETHER_SELECTOR_CONV_MODE_MII_MODE         (0x00) /* MII mode (Through mode) */
#define ETHER_SELECTOR_CONV_MODE_RMII_10M_IN      (0x04) /* RMII mode 10Mbps REF_CLK input */
#define ETHER_SELECTOR_CONV_MODE_RMII_100M_IN     (0x05) /* RMII mode 100Mbps REF_CLK input */
#define ETHER_SELECTOR_CONV_MODE_RMII_10M_OUT     (0x14) /* RMII mode 10Mbps REF_CLK output */
#define ETHER_SELECTOR_CONV_MODE_RMII_100M_OUT    (0x15) /* RMII mode 100Mbps REF_CLK output */
#define ETHER_SELECTOR_CONV_MODE_RGMII_1G         (0x0a) /* RGMII mode 1000Mbps */
#define ETHER_SELECTOR_CONV_MODE_RGMII_100M       (0x09) /* RGMII mode 100Mbps */
#define ETHER_SELECTOR_CONV_MODE_RGMII_10M        (0x08) /* RGMII mode 10Mbps */

#define ETHER_SELECTOR_CONV_BIT_SPEED_10M         (0x00) /* 10Mbps */
#define ETHER_SELECTOR_CONV_BIT_SPEED_100M        (0x01) /* 100Mbps */
#define ETHER_SELECTOR_CONV_BIT_SPEED_1000M       (0x02) /* 1000Mbps */
#define ETHER_SELECTOR_CONV_BIT_SPEED_MASK        (0x03) /* Mask of Converter speed */

#define ETHER_SELECTOR_CONV_BIT_MODE_MII          (0x00) /* MII mode */
#define ETHER_SELECTOR_CONV_BIT_MODE_RMII         (0x04) /* RMII mode */
#define ETHER_SELECTOR_CONV_BIT_MODE_RGMII        (0x08) /* RGMII mode */
#define ETHER_SELECTOR_CONV_BIT_MODE_MASK         (0x0c) /* Mask of Converter mode */

#define ETHER_SELECTOR_CONV_BIT_REF_CLK_IN        (0x00) /* REF_CLK input */
#define ETHER_SELECTOR_CONV_BIT_REF_CLK_OUT       (0x10) /* RE_CLK  output */
#define ETHER_SELECTOR_CONV_BIT_REF_CLK_MASK      (0x10) /* Mask of Converter REF_CLK */

#define ETHER_SELECTOR_MODCTRL_BIT_SWMODE_MASK    (0x03) /* Mask of SW_MODE[1:0] */
#define ETHER_SELECTOR_PHYLNK_BIT_SWLINK_MASK     (0x07) /* Mask of SWLINK[2:0] */
#define ETHER_SELECTOR_PHYLNK_BIT_CATLNK_MASK     (0x07) /* Mask of CATLNK[2:0] */
#define ETHER_SELECTOR_CONVRST_BIT_PHYIR_MASK     (0x07) /* Mask of PHYIR[2:0] */

/* Key code for PRCMD register */
#define ETHER_SELECTOR_PRCMD_UNLOCK1              (0x000000A5U)
#define ETHER_SELECTOR_PRCMD_UNLOCK2              (0x00000001U)
#define ETHER_SELECTOR_PRCMD_UNLOCK3              (0x0000FFFEU)
#define ETHER_SELECTOR_PRCMD_UNLOCK4              (0x00000001U)
#define ETHER_SELECTOR_PRCMD_LOCK                 (0x00000000U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_ether_selector_reg_protection_disable(void * const p_reg_base);
static void r_ether_selector_reg_protection_enable(void * const p_reg_base);
static void r_ether_selector_state_initialize(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Version data structure. */
static const fsp_version_t s_ether_selector_version =
{
    .api_version_minor  = ETHER_SELECTOR_API_VERSION_MINOR,
    .api_version_major  = ETHER_SELECTOR_API_VERSION_MAJOR,
    .code_version_minor = ETHER_SELECTOR_CODE_VERSION_MINOR,
    .code_version_major = ETHER_SELECTOR_CODE_VERSION_MAJOR,
};

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** ETHER_SELECTOR Implementation of General Timer Driver  */
const ether_selector_api_t g_ether_selector_on_ether_selector =
{
    .open         = R_ETHER_SELECTOR_Open,
    .converterSet = R_ETHER_SELECTOR_ConverterSet,
    .close        = R_ETHER_SELECTOR_Close,
    .versionGet   = R_ETHER_SELECTOR_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup ETHER_SELECTOR
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes the ETHER_SELECTOR module instance. Implements @ref ether_selector_api_t::open.
 *
 *
 * @retval FSP_SUCCESS                 Ethernet port configuration set successfully.
 * @retval FSP_ERR_ASSERTION           Pointer to ETHER_SELECTOR control block or configuration structure is NULL.
 * @retval FSP_ERR_INVALID_CHANNEL     Invalid channel number is given.
 * @retval FSP_ERR_ALREADY_OPEN        R_ETHER_SELECTOR_Open has already been called for this p_ctrl.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_SELECTOR_Open (ether_selector_ctrl_t * const p_ctrl, ether_selector_cfg_t const * const p_cfg)
{
    ether_selector_instance_ctrl_t * p_instance_ctrl = (ether_selector_instance_ctrl_t *) p_ctrl;
    R_ETHSS_Type * p_reg_ethss = R_ETHSS;

    volatile uint32_t * p_reg_convctrl;
    uint32_t            sw_mode;
    uint32_t            convctrl;
    uint32_t            phylink;
    uint32_t            convrst;
    uint8_t             port;

    port = p_cfg->port;

#if ETHER_SELECTOR_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ERROR_RETURN((BSP_FEATURE_ETHSS_MAX_PORTS > port), FSP_ERR_INVALID_CHANNEL);
#endif

    FSP_ERROR_RETURN(ETHER_SELECTOR_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);

    p_instance_ctrl->p_reg = p_reg_ethss;
    p_instance_ctrl->p_cfg = p_cfg;

    /* One time initialization for all ETHER_SELECTOR instances. */
    r_ether_selector_state_initialize();

    /* Unlock write access protection for Ethernet subsystem registers */
    r_ether_selector_reg_protection_disable(p_reg_ethss);

    /* Set the function of Ethernet ports. */
    sw_mode = ETHER_SELECTOR_CFG_MODE;
    p_reg_ethss->MODCTRL_b.SW_MODE = sw_mode & ETHER_SELECTOR_MODCTRL_BIT_SWMODE_MASK;

    /* Set the MAC of all port for half-duplex.  */
    p_reg_ethss->SWDUPC_b.PHY_DUPLEX = 0;

    /* Set all Ethernet switch port to select not use 10Mbps.  */
    p_reg_ethss->SWCTRL_b.SET10 = 0;

    /* Set RGMII/RMII Converter configuration */
    switch (port)
    {
        case 0:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[0];
            break;
        }

        case 1:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[1];
            break;
        }

        case 2:
        default:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[2];
            break;
        }
    }

    convctrl  = *p_reg_convctrl;
    convctrl &= ~(uint32_t) (ETHER_SELECTOR_FULLD | ETHER_SELECTOR_CONV_MODE_MASK);

    if (ETHER_SELECTOR_INTERFACE_MII == p_instance_ctrl->p_cfg->interface)
    {
        convctrl |= ETHER_SELECTOR_CONV_MODE_MII_MODE;
    }
    else
    {
        if (ETHER_SELECTOR_INTERFACE_RMII == p_instance_ctrl->p_cfg->interface)
        {
            convctrl |= ETHER_SELECTOR_CONV_MODE_RMII_10M_IN;
        }
        else
        {
            convctrl |= ETHER_SELECTOR_CONV_MODE_RGMII_10M;
        }

        if (ETHER_SELECTOR_SPEED_100MBPS == p_instance_ctrl->p_cfg->speed)
        {
            convctrl |= ETHER_SELECTOR_CONV_BIT_SPEED_100M;
        }

        if (ETHER_SELECTOR_SPEED_1000MBPS == p_instance_ctrl->p_cfg->speed)
        {
            convctrl |= ETHER_SELECTOR_CONV_BIT_SPEED_1000M;
        }

        if (ETHER_SELECTOR_REF_CLOCK_OUTPUT == p_instance_ctrl->p_cfg->ref_clock)
        {
            convctrl |= ETHER_SELECTOR_CONV_BIT_REF_CLK_OUT;
        }
    }

    if (ETHER_SELECTOR_DUPLEX_FULL == p_instance_ctrl->p_cfg->duplex)
    {
        convctrl |= ETHER_SELECTOR_FULLD;
    }

    *p_reg_convctrl = convctrl;

    /* Set active level of PHYLINK signal using the Ethernet switch interface. */
    /* The SWLINK goes Active-High with a value of 0. */
    phylink  = p_reg_ethss->PHYLNK_b.SWLINK;
    phylink &= ~(uint32_t) (1 << port);
    phylink |= ((uint32_t) (~(p_instance_ctrl->p_cfg->phylink) << port));
    p_reg_ethss->PHYLNK_b.SWLINK = phylink & ETHER_SELECTOR_PHYLNK_BIT_SWLINK_MASK;

    /* Set active level of PHYLINK signal using the EtherCAT interface. */
    /* The CATLNK goes Active-High with a value of 1. */
    phylink  = p_reg_ethss->PHYLNK_b.CATLNK;
    phylink &= ~(uint32_t) (1 << port);
    phylink |= ((uint32_t) p_instance_ctrl->p_cfg->phylink << port);
    p_reg_ethss->PHYLNK_b.CATLNK = phylink & ETHER_SELECTOR_PHYLNK_BIT_CATLNK_MASK;

    /* Release reset of RGMII/RMII converter. */
    convrst  = p_reg_ethss->CONVRST_b.PHYIR;
    convrst &= ~(uint32_t) (1 << port);
    convrst |= (uint32_t) (1 << port);
    p_reg_ethss->CONVRST_b.PHYIR = convrst & ETHER_SELECTOR_CONVRST_BIT_PHYIR_MASK;

    /* Lock write access protection for Ethernet subsystem registers */
    r_ether_selector_reg_protection_enable(p_reg_ethss);

    /* Mark instance as opened. */
    p_instance_ctrl->open = ETHER_SELECTOR_OPEN;

    /* All done.  */
    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the converter speed and duplex after initilaize. Implements @ref ether_selector_api_t::converterSet.
 *
 *
 * @retval FSP_SUCCESS                 Speed adn duplex seetings successfully changed.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_SELECTOR_ConverterSet (ether_selector_ctrl_t * const p_ctrl,
                                         ether_selector_speed_t        speed,
                                         ether_selector_duplex_t       duplex)

{
    ether_selector_instance_ctrl_t * p_instance_ctrl = (ether_selector_instance_ctrl_t *) p_ctrl;
    R_ETHSS_Type      * p_reg_ethss;
    volatile uint32_t * p_reg_convctrl;

    uint32_t convctrl;
    uint8_t  port;

#if ETHER_SELECTOR_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_SELECTOR_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    port        = p_instance_ctrl->p_cfg->port;
    p_reg_ethss = p_instance_ctrl->p_reg;

    /* Unlock write access protection for Ethernet subsystem registers */
    r_ether_selector_reg_protection_disable(p_reg_ethss);

    switch (port)
    {
        case 0:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[0];
            break;
        }

        case 1:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[1];
            break;
        }

        case 2:
        default:
        {
            p_reg_convctrl = (uint32_t *) &p_reg_ethss->CONVCTRL[2];
            break;
        }
    }

    convctrl  = *p_reg_convctrl;
    convctrl &= (uint32_t) (~(ETHER_SELECTOR_FULLD | ETHER_SELECTOR_CONV_BIT_SPEED_MASK));

    if (ETHER_SELECTOR_SPEED_100MBPS == speed)
    {
        convctrl |= ETHER_SELECTOR_CONV_BIT_SPEED_100M;
    }

    if (ETHER_SELECTOR_SPEED_1000MBPS == speed)
    {
        convctrl |= ETHER_SELECTOR_CONV_BIT_SPEED_1000M;
    }

    if (ETHER_SELECTOR_DUPLEX_FULL == duplex)
    {
        convctrl |= ETHER_SELECTOR_FULLD;
    }

    *p_reg_convctrl = convctrl;

    /* Lock write access protection for Ethernet subsystem registers */
    r_ether_selector_reg_protection_enable(p_reg_ethss);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close driver data.  Implements @ref ether_selector_api_t::close.
 *
 * @retval FSP_SUCCESS                 Ethernet selector closed.
 * @retval FSP_ERR_ASSERTION           p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_SELECTOR_Close (ether_selector_ctrl_t * const p_ctrl)
{
    ether_selector_instance_ctrl_t * p_instance_ctrl = (ether_selector_instance_ctrl_t *) p_ctrl;

#if ETHER_SELECTOR_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_SELECTOR_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Mark instance as closed. */
    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets driver version based on compile time macros.  Implements @ref ether_selector_api_t::versionGet.
 *
 * @retval     FSP_SUCCESS          Version in p_version.
 * @retval     FSP_ERR_ASSERTION    The parameter p_version is NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_SELECTOR_VersionGet (fsp_version_t * const p_version)
{
#if ETHER_SELECTOR_CFG_PARAM_CHECKING_ENABLE

    /* Verify parameters are valid */
    FSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = s_ether_selector_version.version_id;

    return FSP_SUCCESS;
}

/** @} (end addtogroup ETHER_SELECTOR) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Unlock write access protection for Ethernet subsystem registers
 *
 **********************************************************************************************************************/
static void r_ether_selector_reg_protection_disable (void * const p_reg_base)
{
    R_ETHSS_Type * p_reg_ethss = (R_ETHSS_Type *) p_reg_base;

    /* Unlock the protection of ethernet subsystem register */
    p_reg_ethss->PRCMD = ETHER_SELECTOR_PRCMD_UNLOCK1;
    p_reg_ethss->PRCMD = ETHER_SELECTOR_PRCMD_UNLOCK2;
    p_reg_ethss->PRCMD = ETHER_SELECTOR_PRCMD_UNLOCK3;
    p_reg_ethss->PRCMD = ETHER_SELECTOR_PRCMD_UNLOCK4;
}

/*******************************************************************************************************************//**
 * Lock write access protection for Ethernet subsystem registers
 *
 **********************************************************************************************************************/
static void r_ether_selector_reg_protection_enable (void * const p_reg_base)
{
    R_ETHSS_Type * p_reg_ethss = (R_ETHSS_Type *) p_reg_base;

    /* Lock the protection of ethernet access register */
    p_reg_ethss->PRCMD = ETHER_SELECTOR_PRCMD_LOCK;
}

/*******************************************************************************************************************//**
 * One time state initialization for all ETHER_SELECTOR instances.
 **********************************************************************************************************************/
static void r_ether_selector_state_initialize (void)
{
    /* Stores initialization state to skip initialization in ::R_ETHER_SELECTOR_Open after the first call. */
    static bool g_ether_selector_state_initialized = false;

    /* ETHER_SELECTOR requires a one time initialization.
     *  This will be handled only the first time this function is called.
     *  This initialization:
     *  -# Powers on the ETHSS block.
     *  -# Release reset control of the ETHSS and the MII Converter  */
    if (!g_ether_selector_state_initialized)
    {
        g_ether_selector_state_initialized = true;

        /* Power on ETHSS*/
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
        R_BSP_MODULE_START(FSP_IP_ETHSS, 0);

        /* Release modules */
        R_BSP_ModuleResetDisable(BSP_MODULE_RESET_ESC_ETH_ACCESSARY);
        R_BSP_ModuleResetDisable(BSP_MODULE_RESET_MII);
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
    }
}
