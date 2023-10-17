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

#ifndef UM_COMMON_API_H
#define UM_COMMON_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Common error codes */
typedef enum e_usr_err
{
    USR_SUCCESS 				= 0,
	USR_ERR_ABORTED				,   /** Aborted */
    USR_ERR_ASSERTION           ,   /** Assertion error */

    USR_ERR_NOT_INITIALIZED     ,   /** Not initialized */
	USR_ERR_ALREADY_OPEN		,   /** Already open    */
	USR_ERR_ALREADY_RUNNING     ,   /** Already running */

	USR_ERR_IN_USE				,   /** Resource is lacked or locked */
	USR_ERR_NOT_ENABLED			,   /** Resource is not enabled */
	USR_ERR_NOT_FOUND           ,   /** Resource is not found */

	USR_ERR_TIMEOUT             ,   /** Resource is timeout */

} usr_err_t;

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Return with error */
#define USR_ERROR_RETURN(a, err)                        \
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
        else                                            \
        {                                               \
            return err;                                 \
        }                                               \
    }
#define USR_ERROR_RETURN_VOID(a)                        \
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
        else                                            \
        {                                               \
            return ;                                    \
        }                                               \
    }

/** Assertion */
#define USR_ASSERT(a)    USR_ERROR_RETURN((a), USR_ERR_ASSERTION)

/** Heap Allocation */
#define USR_HEAP_ALLOCATE( pv, size )	    			\
	pv = pvPortMalloc( size );							\
	while( NULL == pv ) 								\
	{													\
		pv = pvPortMalloc( size );					    \
        vTaskDelay(1);									\
    }

/** Heap release */
#define USR_HEAP_RELEASE( pv )							\
    if( pv )											\
    {													\
    	vPortFree( pv );								\
    	pv = NULL;										\
    }

/** Return with release heap */
#define USR_HEAP_ERROR_RETURN(pv, a, err)               \
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
        else                                            \
        {                                               \
            USR_HEAP_RELEASE( pv )                      \
            return err;                                 \
        }                                               \
    }

/** Mutex lock and unlock */
#define USR_LOCK(mutex)			while( pdFALSE == xSemaphoreTake(mutex,1) )
#define USR_UNLOCK(mutex)		( (void) xSemaphoreGive(mutex) )

/** Lock section start */
#define USR_LOCK_SECTION_START(mutex)					\
	USR_LOCK(mutex);									\
	do {

/** Lock section end */
#define USR_LOCK_SECTION_END(mutex)						\
	} while (0);										\
	(void) USR_UNLOCK(mutex)							\

/** Error return in lock section */
#define USR_LOCK_ERROR_RETURN(mutex, a, err)			\
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0; /* Do nothing */ 					\
        }                                               \
        else                                            \
        {                                               \
            (void) xSemaphoreGive( mutex );	            \
            return err;                                 \
        }                                               \
    }

#endif
