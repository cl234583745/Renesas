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
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Includes user modules */
#include "um_common_api.h"
#include "um_ether_netif_api.h"
#include "um_ether_netif_cfg.h"
#include "um_ether_netif_feature.h"
#include "um_ether_netif.h"
#include "um_ether_netif_internal.h"

/** Includes target FSP modules */
#include "fsp_common_api.h"

/** Includes r_ether module */
#include "r_ether_api.h"
#include "r_ether_cfg.h"

#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER
#include "r_ether.h"
#endif
#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC
#include "r_gmac.h"
#endif
#if UM_ETHER_NETIF_FEATURE_R_ETHER_DEPEND_ON_R_ETHSW
#include "r_ethsw_api.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER
#define ETHER_NETIF_INTERRUPT_FACTOR_RECEPTION          (0x01070000)
#endif /** defined(BSP_MCU_GROUP_RA6M3) */

#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC
#define ETHER_NETIF_INTERRUPT_FACTOR_RECEPTION			(0x000000C0)
#endif

#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG
/** Define management tag for reception. */
#define GMAC_RX_MGTAG_DATA1_PORT_SHIFT      (0U)
#define GMAC_RX_MGTAG_DATA1_PORT_MASK       (0x03 << GMAC_RX_MGTAG_DATA1_PORT_SHIFT )
#define GMAC_RX_MGTAG_DATA1_TIMER           (1U << 4)
#define GMAC_RX_MGTAG_DATA1_RED             (1U << 6)

/** Define management tag for transmit */
#define GMAC_TX_MGTAG_DATA1_FORWARD_FORCE   (1U << 0)
#define GMAC_TX_MGTAG_DATA1_TIMESTAMP       (1U << 3)
#define GMAC_TX_MGTAG_DATA1_ONE_STEP        (1U << 4)
#define GMAC_TX_MGTAG_DATA1_PRP_SUPPRESS    (1U << 5)
#define GMAC_TX_MGTAG_DATA1_PRP_FORCE       (1U << 6)
#define GMAC_TX_MGTAG_DATA1_PRP_SEQUENCE    (1U << 7)
#define GMAC_TX_MGTAG_DATA1_QUE_NUM         (1U << 9)
#define GMAC_TX_MGTAG_DATA1_TIM_NUM         (1U << 13)

#define GMAC_TX_MGTAG_DATA1_QUE_NUM_SHIFT   (10U)
#define GMAC_TX_MGTAG_DATA1_QUE_NUM_MASK    (0x07 << GMAC_TX_MGTAG_DATA1_QUE_NUM_SHIFT)
#define GMAC_TX_MGTAG_DATA1_TIM_NUM_SHIFT   (14U)
#define GMAC_TX_MGTAG_DATA1_TIM_NUM_MASK    (0x01 << GMAC_TX_MGTAG_DATA1_TIM_NUM_SHIFT)

#define GMAC_TX_MGTAG_DATA2_PORT_SHIFT      (0U)
#define GMAC_TX_MGTAG_DATA2_PORT_MASK       (0x07 << GMAC_TX_MGTAG_DATA2_PORT_SHIFT)
#define GMAC_TX_MGTAG_DATA2_TSID_SHIFT      (9U)
#define GMAC_TX_MGTAG_DATA2_TSID_MASK       (0x7F << GMAC_TX_MGTAG_DATA2_TSID_SHIFT)
#endif

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG
typedef struct st_ether_frame
{
    uint8_t dst_addr[6];               /* destination addres */
    uint8_t src_addr[6];               /* source address */
    uint8_t type[2];                   /* type */
    uint8_t data[2];                   /* data */
} ether_frame_t;

typedef struct st_ether_mgtag
{
    uint8_t control_tag[2];            /* control_tag of management-tag  */
    uint8_t control_data[2];           /* control_data of management-tag  */
    uint8_t control_data2[4];          /* control_data2 of management-tag  */
} ether_mgtag_t;

typedef struct st_ether_frame_mgtag
{
    uint8_t       dst_addr[6];         /* destination address */
    uint8_t       src_addr[6];         /* source address */
    ether_mgtag_t mgtag;               /* management tag */
    uint8_t       type[2];             /* type */
    uint8_t       data[2];             /* data */
} ether_frame_mgtag_t;
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG
static usr_err_t _build_tx_management_tag ( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame, ether_mgtag_t * p_mgtag );
static usr_err_t _fetch_rx_management_tag ( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame, ether_mgtag_t const * p_mgtag );
static usr_err_t _insert_management_tag ( ether_netif_frame_t * const p_frame, ether_mgtag_t const * p_mgtag );
static usr_err_t _remove_management_tag ( ether_netif_frame_t * const p_frame, ether_mgtag_t * p_mgtag );
#endif

/***********************************************************************************************************************
 * Imported function prototypes
 **********************************************************************************************************************/
/** r_ether callback function set by r_ether_cfg. */
extern void vEtherISRCallback (ether_callback_args_t * p_args);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/**
 * Context for callback of r_ether module.
 * TODO:
 *  r_ether module has user context interface but it does not work;
 *    RA FSP v2.x.x:  p_context is not used.
 *    RA FSP v3.x.x:  p_context cannot be set by FSP configuration.
 *    RZT FSP v1.x.x: p_context cannot be set by FSP configuration.
 *  Therefore, this is instead of the user context interface.
 */
static void * sp_r_ether_callback_context = NULL;

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl				Pointer to the controller
 * @param[in] p_ether_instance		Pointer to the Ethernet instance
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_ether_open(ether_netif_ether_ctrl_t * const p_ctrl,
                                    ether_instance_t const * const p_ether_instance,
                                    ether_netif_reader_ctrl_t * const p_reader_ctrl)
{
	/** Error codes */
	fsp_err_t fsp_err;

    /** Set target control. */
    p_ctrl->p_ether_instance = p_ether_instance;

    /** Set internal submodule control. */
    p_ctrl->p_reader_ctrl = p_reader_ctrl;

	/** Create a mutex for r_ether */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	USR_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_ERR_NOT_INITIALIZED );

	/** Open the Ethernet instance */
	fsp_err = p_ctrl->p_ether_instance->p_api->open(p_ctrl->p_ether_instance->p_ctrl, p_ctrl->p_ether_instance->p_cfg);
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err || FSP_ERR_ALREADY_OPEN == fsp_err , USR_ERR_NOT_INITIALIZED );

    /** Set context */
    sp_r_ether_callback_context = p_ctrl;

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Read Ethernet frame from Ethernet peripherals via driver.
 *
 * @param[in]  p_ctrl			Pointer to the controller
 * @param[out] p_frame			Pointer to the frame buffer to be set to the read Ethernet frame.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_ether_read( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame )
{
	/** Error codes */
	fsp_err_t fsp_err;

    /** Initialize the port number */
    p_frame->port = ETHER_NETIF_PORT_RECV_PORT_ANY;

	/** Lock statement. */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
    	/** Read Ethernet frame. */
        fsp_err = p_ctrl->p_ether_instance->p_api->read(p_ctrl->p_ether_instance->p_ctrl, p_frame->p_buffer, &p_frame->length);
        USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle , FSP_SUCCESS == fsp_err, USR_ERR_ABORTED);
    }
    USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG

    /** Resolve the gmac's extended configuration. */
    gmac_extend_cfg_t const * gmac_extend_cfg = p_ctrl->p_ether_instance->p_cfg->p_extend;

    /** The received Ethernet frame does not have the management tag. */
    if( gmac_extend_cfg->p_ethsw_instance->p_cfg->specific_tag == ETHSW_SPECIFIC_TAG_ENABLE )
    {
        ether_mgtag_t mgtag;
        (void) _remove_management_tag(p_frame, &mgtag);
        (void) _fetch_rx_management_tag(p_ctrl, p_frame, &mgtag);
    }

#endif

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Write Ethernet frame to Ethernet peripherals via driver.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[out] p_frame			Pointer to the frame buffer to send .
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_ether_write( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame )
{
	/** Error codes */
	fsp_err_t fsp_err;

#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG

	/** Management tag */
    ether_mgtag_t mgtag;

    /** Resolve the gmac extended configuration. */
    gmac_extend_cfg_t const * gmac_extend_cfg = p_ctrl->p_ether_instance->p_cfg->p_extend;

    /** The received Ethernet frame does not have the management tag. */
    if( ETHSW_SPECIFIC_TAG_ENABLE == gmac_extend_cfg->p_ethsw_instance->p_cfg->specific_tag )
    {
        (void) _build_tx_management_tag(p_ctrl, p_frame, &mgtag);
        (void) _insert_management_tag(p_frame, &mgtag);
    }

#endif

    /** Lock statement. */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
        /** Write Ethernet frame. */
        fsp_err = p_ctrl->p_ether_instance->p_api->write(p_ctrl->p_ether_instance->p_ctrl, p_frame->p_buffer, p_frame->length );
        USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle , FSP_SUCCESS == fsp_err, USR_ERR_ABORTED);
    }
    USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

    /** Return FSP error of r_ether. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Monitor the link state of Ethernet peripherals via driver.
 *
 * @param[in] p_ctrl			Pointer to the controller
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_ABORTED		Fail to the process.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_ether_monitor (ether_netif_ether_ctrl_t * const p_ctrl, uint32_t * p_link_status )
{
    /** Error status */
    fsp_err_t fsp_err;

	/** Lock statement. */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
		/** Try linking the process */
		fsp_err = p_ctrl->p_ether_instance->p_api->linkProcess(p_ctrl->p_ether_instance->p_ctrl);
    }
    USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER

    /** Record port status */
    *p_link_status = ( FSP_SUCCESS == fsp_err ) ? ETHER_NETIF_PORT_BIT(0) : ETHER_NETIF_PORT_NONE;

#endif

#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC

    /** Check the link state of multiple ports */
    uint8_t port = 0;
    ether_link_status_t port_status;

    /** Clear link status. */
    *p_link_status &= ~ETHER_NETIF_PORT_ALL;

    for( port = 0; port < BSP_FEATURE_GMAC_MAX_PORTS; port++ )
    {
        /** Check the link state of single port. */
        USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
        {
            fsp_err = p_ctrl->p_ether_instance->p_api->getLinkStatus( p_ctrl->p_ether_instance->p_ctrl, port, &port_status );
        }
        USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

        /** If link status can be gotten correctly and is ready, the port is linking. */
        if( FSP_SUCCESS == fsp_err )
        {
            if( ETHER_LINK_STATUS_READY == port_status || ETHER_LINK_STATUS_UP == port_status )
            {
                *p_link_status |= ETHER_NETIF_PORT_BIT(port);
            }
        }
        /** Otherwise, the port is not linking. */
        else
        {
            *p_link_status &= ~ETHER_NETIF_PORT_BIT(port);
        }
    }
#endif

    /** If there are linking port, return success.*/
    return ( *p_link_status & ETHER_NETIF_PORT_MASK ) ?  USR_SUCCESS : USR_ERR_ABORTED ;
}

/*******************************************************************************************************************//**
 * Callback handler to set to Ethernet module instance
 *
 * @param[in]   p_args     Pointer to ether instance callback arguments
 **********************************************************************************************************************/
void vEtherISRCallback (ether_callback_args_t * p_args)
{
    /** Flag for requesting the RTOS to switch the context to highest priority task. */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /** Get the instance as the context. */
    ether_netif_ether_ctrl_t * const p_ctrl = sp_r_ether_callback_context;

    /** Task handle for notification */
    TaskHandle_t p_task_handle;

    /** Check Callback event */
    switch ( p_args->event )
    {

#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_ETHER
    case ETHER_EVENT_INTERRUPT:
	if (p_args->status_eesr & ETHER_NETIF_INTERRUPT_FACTOR_RECEPTION)
#endif
#if UM_ETHER_NETIF_FEATURE_R_ETHER_IMPL_R_GMAC
    case ETHER_EVENT_SBD_INTERRUPT: /** Case that Ether module interrupts. */
    if (p_args->status_ether & ETHER_NETIF_INTERRUPT_FACTOR_RECEPTION)
#endif
    {
        /** Get task handle of reader task */
        (void) um_ether_netif_task_reader_get_task_handle(p_ctrl->p_reader_ctrl, &p_task_handle);

    	if ( p_task_handle != NULL )
		{
			vTaskNotifyGiveFromISR( p_task_handle, &xHigherPriorityTaskWoken);
		}
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
    break;

    /** Break when other cases occur */
    default:
    	break;
    }
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

#if UM_ETHER_NETIF_FEATURE_R_ETHSW_SUPPORT_SPECIFIC_TAG
/*******************************************************************************************************************//**
 * Build management tag for Ethernet frame to be sent.
 * @param p_ctrl        Pointer to controller
 * @param p_frame       Pointer to Ethernet frame structure of network interface.
 * @param p_mgtag       Pointer to management tag to be built.
 * @retval USR_SUCCESS
 **********************************************************************************************************************/
static usr_err_t _build_tx_management_tag ( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame, ether_mgtag_t * p_mgtag )
{
    /** Resolve the gmac extended configuration. */
    gmac_extend_cfg_t const * gmac_extend_cfg = p_ctrl->p_ether_instance->p_cfg->p_extend;

    /** Control datum of management tag. */
    uint32_t control_data  = 0x0000U;
    uint32_t control_data2 = 0x00000000UL;

    /** Set ports for forwarding. */
    switch( p_frame->port )
    {
        /** Disable Forced Forwarding. */
    case ETHER_NETIF_PORT_SEND_PORT_ANY:
        control_data &= (uint32_t) ~GMAC_TX_MGTAG_DATA1_FORWARD_FORCE;
        break;

        /** Enable Forced Forwarding, and set the all ports as targets */
    case ETHER_NETIF_PORT_SEND_PORT_ALL:
        control_data |= (uint32_t) GMAC_TX_MGTAG_DATA1_FORWARD_FORCE;
        control_data2 |= (uint32_t) GMAC_TX_MGTAG_DATA2_PORT_MASK;
        break;

        /** Enable Forced Forwarding, and Set the specified ports. */
    default:
        control_data |= GMAC_TX_MGTAG_DATA1_FORWARD_FORCE;
        control_data2 |= ((p_frame->port << GMAC_TX_MGTAG_DATA2_PORT_SHIFT ) & GMAC_TX_MGTAG_DATA2_PORT_MASK );
        break;
    }

    /** Build management tag. */
    p_mgtag->control_tag[0]  = (uint8_t)( ( gmac_extend_cfg->p_ethsw_instance->p_cfg->specific_tag_id >> 8 ) & 0x00FF ) ;
    p_mgtag->control_tag[1]  = (uint8_t)( ( gmac_extend_cfg->p_ethsw_instance->p_cfg->specific_tag_id      ) & 0x00FF ) ;
    p_mgtag->control_data[0] = (uint8_t)( ( control_data >> 8 ) & 0xFFU );
    p_mgtag->control_data[1] = (uint8_t)( ( control_data      ) & 0xFFU );
    p_mgtag->control_data2[0] = (uint8_t)( ( control_data2 >> 24 ) & 0xFFUL );
    p_mgtag->control_data2[1] = (uint8_t)( ( control_data2 >> 16 ) & 0xFFUL );
    p_mgtag->control_data2[2] = (uint8_t)( ( control_data2 >> 8  ) & 0xFFUL );
    p_mgtag->control_data2[3] = (uint8_t)( ( control_data2       ) & 0xFFUL );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Fetch management tag information from received Ethernet frame
 * @param p_ctrl        Pointer to controller
 * @param p_frame       Pointer to Ethernet frame structure of network interface.
 * @param p_mgtag       Pointer to management tag to be fetched.
 * @retval USR_SUCCESS
 **********************************************************************************************************************/
static usr_err_t _fetch_rx_management_tag ( ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame, ether_mgtag_t const * p_mgtag )
{
    /** Currently unused parameter. */
    (void) p_ctrl;

    /** Load control data */
    uint32_t control_data = (uint32_t) ( p_mgtag->control_data[0] << 8U ) + ( p_mgtag->control_data[1] );

    /** Get port where the received frame was received from. */
    p_frame->port = (uint32_t) ( (0x01) << (control_data & GMAC_RX_MGTAG_DATA1_PORT_MASK) );

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Insert management tag from Ethernet frame to be sent.
 * @param p_frame       Pointer to Ethernet frame structure of network interface.
 * @param p_mgtag       Pointer to management tag to be inserted.
 * @retval USR_SUCCESS
 **********************************************************************************************************************/
static usr_err_t _insert_management_tag ( ether_netif_frame_t * const p_frame, ether_mgtag_t const * p_mgtag )
{
    /** Resolve the Ethernet frame as it does not have a management tag. */
    ether_frame_t * p_ether_frame = (ether_frame_t* ) p_frame->p_buffer;

    /** Resolve the Ethernet frame as it has a management tag. */
    ether_frame_mgtag_t * p_mgtag_frame = (ether_frame_mgtag_t *) p_frame->p_buffer;

    /** Shift the buffer after Ether Type field to insert management tag by overwriting. */
    memmove( &p_mgtag_frame->type, &p_ether_frame->type,
             p_frame->length - ( sizeof( p_ether_frame->dst_addr ) + sizeof( p_ether_frame->src_addr ) ) );

    /** Copy the management tag into Ethernet frame */
    memcpy( &p_mgtag_frame->mgtag, p_mgtag, sizeof(ether_mgtag_t) );

    /** Add the length of management tag. */
    p_frame->length =  p_frame->length + sizeof(ether_mgtag_t);

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Remove management tag from received Ethernet frame
 * @param p_frame       Pointer to Ethernet frame structure of network interface.
 * @param p_mgtag       Pointer to management tag to be removed.
 * @retval USR_SUCCESS
 **********************************************************************************************************************/
static usr_err_t _remove_management_tag ( ether_netif_frame_t * const p_frame, ether_mgtag_t * p_mgtag )
{
    /** Resolve the Ethernet frame as it has a management tag. */
    ether_frame_mgtag_t * p_mgtag_frame = (ether_frame_mgtag_t *) p_frame->p_buffer;

    /** Resolve the Ethernet frame as it does not have a management tag. */
    ether_frame_t * p_ether_frame = (ether_frame_t* ) p_frame->p_buffer;

    /** Copy the management tag from Ethernet frame */
    memcpy( p_mgtag, &p_mgtag_frame->mgtag, sizeof(ether_mgtag_t) );

    /** Shift the buffer after Ether Type field to remove management tag by overwriting. */
    memmove( &p_ether_frame->type, &p_mgtag_frame->type,
             p_frame->length - ( sizeof(p_mgtag_frame->dst_addr) + sizeof(p_mgtag_frame->src_addr) + sizeof(p_mgtag_frame->mgtag) ) );

    /** Subtract the length of management tag. */
    p_frame->length =  p_frame->length - sizeof(ether_mgtag_t);

    /** Return success code. */
    return USR_SUCCESS;
}
#endif // defined(BSP_MCU_GROUP_RZT2M)
