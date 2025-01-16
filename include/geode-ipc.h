#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Send IPC message. Response needs to be freed.
 * Message is NULL if Geode IPC could not be connected to.
 */
char* geode_ipc_send(char const* message);


/**
 * Send IPC message with a timeout. Response needs to be freed.
 * Message is NULL if Geode IPC could not be connected to.
 */
char* geode_ipc_send_timeout(char const* message, double timeout);

#ifdef __cplusplus
}
#endif