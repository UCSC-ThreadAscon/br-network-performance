#pragma once

#include "workload.h"
#include "time_api.h"

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void initThroughputRequestHandler(void);
void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);