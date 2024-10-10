#pragma once

#include "workload.h"
#include "time_api.h"

#define MAX_PACKETS 1000

#define THROUGHPUT_START_SERVER_URI "throughput-start"
#define SOCK_PORT 5684
#define START_SERVER_PORT 5684

/**
 * I got the idea to use this specific print statement delimiter
 * from the ESP Thread Border Router SDK source code:
 * https://github.com/espressif/esp-thread-br/blob/main/components/esp_ot_br_server/src/esp_br_web.c#L1168
 */
#define PrintDelimiter() otLogNotePlat("<===========================================================>");
#define PrintCritDelimiter() otLogCritPlat("<===========================================================>");

static inline bool connected(otDeviceRole role)
{
  return (role == OT_DEVICE_ROLE_CHILD)  ||
         (role == OT_DEVICE_ROLE_ROUTER) ||
         (role == OT_DEVICE_ROLE_LEADER);
}

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);