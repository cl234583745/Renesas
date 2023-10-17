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
#include "um_opener_port_api.h"
#include "um_opener_port_cfg.h"
#include "um_opener_port.h"
#include "um_opener_port_internal.h"
#include "um_common_api.h"

#include "devicedata.h"
#include "opener_api.h"
#include "cipidentity.h"
#include "cipstring.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static usr_err_t _open_cip_identity_instance( opener_port_cip_ctrl_t * const p_ctrl );
static usr_err_t _init_cip_identity_instance_attributes( opener_port_cip_ctrl_t * const p_ctrl );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_cip_open( opener_port_cip_ctrl_t * const p_ctrl, opener_port_cip_user_device_cfg_t const * const p_cip_device_cfg )
{
	/** Error codes*/
    usr_err_t usr_err;
    EipStatus eip_err;

    /** Store the address of configuration. */
    p_ctrl->p_cip_device_cfg = p_cip_device_cfg;

    /** Initialize the CIP Layer. */
    eip_err = CipStackInit( p_ctrl->p_cip_device_cfg->incarnation_id );
    USR_ERROR_RETURN( kEipStatusOk == eip_err, USR_ERR_NOT_INITIALIZED );

    /** Open Identity object. */
    usr_err = _open_cip_identity_instance( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Initialize identity object attributes. */
    usr_err = _init_cip_identity_instance_attributes( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the controller
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED      Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_cip_close( opener_port_cip_ctrl_t * const p_ctrl )
{
    (void) p_ctrl;

    /** Close remaining sessions and connections, clean up used data */
    (void) ShutdownCipStack();

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Apply module state to device status of identity object
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_cip_set_device_status_from_module_state( opener_port_cip_ctrl_t * const p_ctrl,
                                                                  opener_port_module_state_t current_state,
                                                                  opener_port_module_state_t new_state )
{
    /** Resolve type */
    CipIdentityObject * p_identity_cip_instance = p_ctrl->p_identity_cip_instance;

    /** Clear flags indicating old state */
    switch( current_state )
    {
    case OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL:
        CipIdentityClearStatusFlags(kConfigured);
        break;
    case OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT:
        CipIdentityClearStatusFlags(kMajorRecoverableFault);
        break;
    default:
      break;
    }

    /** Apply new state to identity object attribute 5. */
    switch( new_state )
    {
    case OPENER_PORT_MODULE_STATE_NO_POWER:
        p_identity_cip_instance->state = kStateNonExistent;
        break;

    case OPENER_PORT_MODULE_STATE_SELF_TEST:
        p_identity_cip_instance->state = kStateSelfTesting;
        CipIdentitySetExtendedDeviceStatus(kSelftestingUnknown);
        break;

    case OPENER_PORT_MODULE_STATE_STANDBY:
        p_identity_cip_instance->state = kStateStandby;
        break;

    case OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL:
        p_identity_cip_instance->state = kStateOperational;
        break;

    case OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT:
        p_identity_cip_instance->state = kStateMajorRecoverableFault;
        CipIdentitySetStatusFlags(kMajorRecoverableFault);
        CipIdentitySetExtendedDeviceStatus(kMajorFault);
        break;

    case OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT:
        p_identity_cip_instance->state = kStateMajorUnrecoverableFault;
        CipIdentitySetStatusFlags(kMajorRecoverableFault);
        CipIdentitySetExtendedDeviceStatus(kMajorFault);
        break;

    default:
        break;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Apply module state to device status of identity object
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_cip_set_device_status_from_network_state( opener_port_cip_ctrl_t * const p_ctrl,  opener_port_network_state_t state )
{
    /** Unused parameter */
    (void) p_ctrl;

    /** Convert network state to device status. */
    switch( state )
    {
    case OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS:
        CipIdentitySetExtendedDeviceStatus(kNoIoConnectionsEstablished);
        break;

    case OPENER_PORT_NETWORK_STATE_CONNECTED:
        CipIdentitySetExtendedDeviceStatus(kAtLeastOneIoConnectionInRunMode);
        break;

    default:
        break;
    }

    /** Return success code */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Open CIP Identity object instance
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _open_cip_identity_instance( opener_port_cip_ctrl_t * const p_ctrl )
{
    /** Set the pointer of identity object instance  */
    p_ctrl->p_identity_cip_instance = &g_identity;

    /** Return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Initialize CIP Identity object instance attributes.
 *
 * NOTE:
 *  This function initialize all attributes even if the attributes have already initialized on definition or on CipStackInit() function.
 *  This function should be called after the CIP stack initialization to overwrite attributes by the CIP initialization.
 *
 * @retval USR_SUCCESS      Process has been done successfully.
 **********************************************************************************************************************/
static usr_err_t _init_cip_identity_instance_attributes( opener_port_cip_ctrl_t * const p_ctrl )
{
    /** Resolve type */
    CipIdentityObject * p_identity_cip_instance = p_ctrl->p_identity_cip_instance;

    /** Set the vendor ID (#1) */
    p_identity_cip_instance->vendor_id = OPENER_DEVICE_VENDOR_ID;

    /** Set the device type (#2) */
    p_identity_cip_instance->device_type = OPENER_DEVICE_TYPE;

    /** Set the product code (#3) */
    p_identity_cip_instance->product_code = OPENER_DEVICE_PRODUCT_CODE;

    /** Set the revisions (#4) */
    p_identity_cip_instance->revision.major_revision = OPENER_DEVICE_MAJOR_REVISION;
    p_identity_cip_instance->revision.minor_revision = OPENER_DEVICE_MINOR_REVISION;

    /** Initialize Status (#5) which will be updated by calling APIs */
    (void) SetDeviceStatus(0);
    (void) CipIdentitySetExtendedDeviceStatus( kSelftestingUnknown );

    /** Set the device serial number (#6) */
    (void) SetDeviceSerialNumber( (EipUint32) p_ctrl->p_cip_device_cfg->device_serial_number );

    /** Set the device name (#7) */
    p_identity_cip_instance->product_name.length = sizeof(OPENER_DEVICE_NAME) - 1;
    p_identity_cip_instance->product_name.string = (EipByte*) OPENER_DEVICE_NAME;

    /** Initialize the state (#8) which will be update by calling APIs. */
    p_identity_cip_instance->state = kStateNonExistent;

    /** Return success code */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Definitions of functions declared in opener_api.h
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief Allocate memory for the CIP stack
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
void * CipCalloc (size_t number_of_elements, size_t size_of_element)
{
    /** Allocate heap memory by your heap management. */
    void * p = pvPortMalloc(size_of_element * number_of_elements);

    /** Calloc actually clear get the heap region. */
    memset( p, 0, size_of_element * number_of_elements );

    /** return the heap. */
    return p;
}

/***********************************************************************************************************************
 * @brief Free memory allocated by the OpENer
 * Please see the detail in opener_api.h
 **********************************************************************************************************************/
void CipFree (void * data)
{
    /** Free by your heap management. */
    vPortFree(data);
}
