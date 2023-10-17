/*******************************************************************************
 * Copyright (c) 2016, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#ifndef SRC_PORTS_SOCKET_TIMER_H_
#define SRC_PORTS_SOCKET_TIMER_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/**
 * Include OpENer libraries
 */
#include "typedefs.h"                  /* For MilliSecondes */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief Data structure to store last usage times for sockets
 */
typedef struct socket_timer
{
    int          socket;               /**< key */
    MilliSeconds last_update;          /**< time stop of last update */
} SocketTimer;

/***********************************************************************************************************************
 * Exported global functions
 ***********************************************************************************************************************/
void          SocketTimerSetSocket(SocketTimer * const socket_timer, const int socket);
void          SocketTimerSetLastUpdate(SocketTimer * const socket_timer, const MilliSeconds actual_time);
MilliSeconds  SocketTimerGetLastUpdate(SocketTimer * const socket_timer);
void          SocketTimerClear(SocketTimer * const socket_timer);
void          SocketTimerArrayInitialize(SocketTimer * const array_of_socket_timers, const size_t array_length);
SocketTimer * SocketTimerArrayGetSocketTimer(SocketTimer * const array_of_socket_timers,
                                             const size_t        array_length,
                                             const int           socket);
SocketTimer * SocketTimerArrayGetEmptySocketTimer(SocketTimer * const array_of_socket_timers,
                                                  const size_t        array_length);

#endif                                 /* SRC_PORTS_SOCKET_TIMER_H_ */
