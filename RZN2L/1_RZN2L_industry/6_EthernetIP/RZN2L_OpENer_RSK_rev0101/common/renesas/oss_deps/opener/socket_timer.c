/*******************************************************************************
 * Copyright (c) 2016, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/**
 * Include own header
 */
#include "socket_timer.h"

/**
 * Include OpENer libraries
 */
#include "trace.h"

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief Sets socket of a Socket Timer
 * @param socket_timer Socket Timer to be set
 * @param socket Socket handle
 **********************************************************************************************************************/
void SocketTimerSetSocket (SocketTimer * const socket_timer, const int socket)
{
    socket_timer->socket = socket;
    OPENER_TRACE_INFO("Adds socket %d to socket timers\n", socket);
}

/***********************************************************************************************************************
 * @brief Sets time stamp entry of the Socket Timer
 * @param socket_timer Socket Timer to be set
 * @param actual_time Time stamp
 **********************************************************************************************************************/
void SocketTimerSetLastUpdate (SocketTimer * const socket_timer, const MilliSeconds actual_time)
{
    if (NULL != socket_timer)
    {
        socket_timer->last_update = actual_time;
        OPENER_TRACE_INFO("Sets time stamp for socket %d\n", socket_timer->socket);
    }
}

/***********************************************************************************************************************
 * @brief Gets time stamp of the last update
 * @param socket_timer Socket Timer to be set
 * @return Last update field value
 **********************************************************************************************************************/
MilliSeconds SocketTimerGetLastUpdate (SocketTimer * const socket_timer)
{
    return socket_timer->last_update;
}

/***********************************************************************************************************************
 * @brief Clears a Socket Timer entry
 * @param socket_timer Socket Timer to be cleared
 **********************************************************************************************************************/
void SocketTimerClear (SocketTimer * const socket_timer)
{
    socket_timer->socket      = kEipInvalidSocket;
    socket_timer->last_update = 0;
}

/***********************************************************************************************************************
 * @brief Initializes an array of Socket Timer entries
 * @param array_of_socket_timers The array of Socket Timer entries to be initialized
 * @param array_length the length of the array
 **********************************************************************************************************************/
void SocketTimerArrayInitialize (SocketTimer * const array_of_socket_timers, const size_t array_length)
{
    for (size_t i = 0; i < array_length; ++i)
    {
        SocketTimerClear(&array_of_socket_timers[i]);
    }
}

/***********************************************************************************************************************
 * @brief Get the Socket Timer entry with the spezified socket value
 * @param array_of_socket_timers The Socket Timer array
 * @param array_length The Socket Timer array length
 * @param socket The socket value to be searched for
 * @return The Socket Timer of found, otherwise NULL
 **********************************************************************************************************************/
SocketTimer * SocketTimerArrayGetSocketTimer (SocketTimer * const array_of_socket_timers,
                                              const size_t        array_length,
                                              const int           socket)
{
    for (size_t i = 0; i < array_length; ++i)
    {
        if (socket == array_of_socket_timers[i].socket)
        {
            return &array_of_socket_timers[i];
        }
    }

    return NULL;
}

/***********************************************************************************************************************
 * @brief Get an empty Socket Timer entry
 * @param array_of_socket_timers The Socket Timer array
 * @param array_length The Socket Timer array length
 * @return An empty Socket Timer entry, or NULL if non is available
 **********************************************************************************************************************/
SocketTimer * SocketTimerArrayGetEmptySocketTimer (SocketTimer * const array_of_socket_timers,
                                                   const size_t        array_length)
{
    return SocketTimerArrayGetSocketTimer(array_of_socket_timers, array_length, kEipInvalidSocket);
}
