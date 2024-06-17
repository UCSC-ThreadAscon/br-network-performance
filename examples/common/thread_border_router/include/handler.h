#pragma once

#include "workload.h"
#include "time_api.h"

typedef struct PacketLossStats
{
  bool receivedFirstPacket;
  struct timeval start;
  uint32_t nextSeqNumExpected;
  uint64_t packetsReceived;
  uint64_t packetsExpected;
} PacketLossStats;

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);