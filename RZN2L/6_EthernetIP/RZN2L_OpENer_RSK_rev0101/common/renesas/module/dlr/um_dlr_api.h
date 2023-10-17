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

#ifndef UM_DLR_API_H
#define UM_DLR_API_H

/*******************************************************************************************************************//**
 * @defgroup DLR_API EtherNet/IP Device Level Ring
 * @ingroup RENESAS_INTERFACES
 * @brief Interface for Device level ring protocol functions.
 *
 * @section DLR_API_SUMMARY Summary
 * The UM_DLR interface provides the features for device level ring protocol with Ethernet Switch.
 *
 * Implemented by:
 * - @ref DLR
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Include APIs */
#include "um_common_api.h"
#include "um_ether_netif_api.h"

/** Dependencies on FSP module */
#include "r_ethsw_extend_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** UM_DLR control block. Allocate an instance specific control block to pass into the UM_DLR API calls.
 * @par Implemented as
 * - dlr_instance_t
 */
typedef void dlr_ctrl_t;

/**
 * Structure for instance configuration.
 */
typedef struct st_dlr_cfg
{
    ether_netif_instance_t const * const p_ether_netif_instance; ///< Pointer to ether_netif instance.
    ethsw_instance_t const * const p_ether_switch_instance;
    uint32_t * const p_ip_address;

} dlr_cfg_t;

/** UM_TCPIP_NETIF API structure. General net functions implemented at the HAL layer follow this API. */
typedef struct st_dlr_api
{
    /** Open network interface.
     * @par Implemented as
     * - @ref UM_DLR_Open()
     *
     * @param [in]  p_ctrl          Pointer to control block set in @ref netif_api_t::open call for this net.
     * @param [in]  p_cfg     	    Pointer to configuration structure. All elements of this structures must be set by user.
     * @return
     */
    usr_err_t (* open)( dlr_ctrl_t * const p_ctrl, dlr_cfg_t const * const p_cfg );

    /** Open network interface.
     * @par Implemented as
     * - @ref UM_DLR_Close()
     * @return
     */
    usr_err_t (* start)( dlr_ctrl_t * const p_ctrl );

    /** Open network interface.
     * @par Implemented as
     * - @ref UM_DLR_Close()
     * @return
     */
    usr_err_t (* stop)( dlr_ctrl_t * const p_ctrl );

    /** Open network interface.
     * @par Implemented as
     * - @ref UM_DLR_Close()
     * @return
     */
    usr_err_t (* close)( dlr_ctrl_t * const p_ctrl );

} dlr_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_dlr_instance
{
	dlr_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    dlr_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    dlr_api_t const * p_api;           ///< Pointer to the API structure for this instance
} dlr_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup UM_DLR_API)
 **********************************************************************************************************************/

#endif		/** UM_DLR_API_H */
