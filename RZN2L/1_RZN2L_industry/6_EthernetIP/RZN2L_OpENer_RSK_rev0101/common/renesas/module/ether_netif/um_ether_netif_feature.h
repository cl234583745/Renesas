/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#ifndef UM_ETHER_NETIF_FEATURE_H_
#define UM_ETHER_NETIF_FEATURE_H_

#include "bsp_mcu_family_cfg.h"

#if defined(BSP_MCU_GROUP_RA6M3)

/** This module supports RA6M3 series with RA FSP. */
#define UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER       (1)
#define UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC        (0)
#define UM_ETHER_NETIF_FEATURE_R_ETHER_DEPEND_ON_R_ETHSW    (0)
#define UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG (0)
#define UM_ETHER_NETIF_FEATURE_NUMBER_OF_ETHER_PORTS        (1)

#elif (defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZN2L))

/** This module supports RZT2M series with RZT FSP. */
#define UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER         (0)
#define UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC          (1)
#define UM_ETHER_NETIF_FEATURE_R_ETHER_DEPEND_ON_R_ETHSW    (1)
#define UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG (1)
#define UM_ETHER_NETIF_FEATURE_NUMBER_OF_ETHER_PORTS        (3)

#else

#error "This module supports devices of RA6M3 and RZT2M group with FSP only."

#endif

#endif /** UM_ETHER_NETIF_FEATURE_H_ */
