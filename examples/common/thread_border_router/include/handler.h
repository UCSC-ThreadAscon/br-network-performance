#pragma once

#include "workload.h"
#include "time_api.h"

#define PACKET_LOSS_MAX_PACKETS 1000

#define PrintDelimiter() otLogNotePlat("---------------------------");

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);