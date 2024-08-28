#pragma once

#include "workload.h"
#include "time_api.h"

#define PACKET_LOSS_MAX_PACKETS 1000

/**
 * I got the idea to use this specific print statement delimiter
 * from the ESP Thread Border Router SDK source code:
 * https://github.com/espressif/esp-thread-br/blob/main/components/esp_ot_br_server/src/esp_br_web.c#L1166
 */
#define PrintDelimiter() otLogNotePlat("<===========================================================>");
#define PrintCritDelimiter() otLogCritPlat("<===========================================================>");

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);