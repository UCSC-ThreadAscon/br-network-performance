#pragma once

#include "workload.h"
#include "time_api.h"

typedef enum PacketLossExpStatus
{
  NoStarted,
  Calculating,
  ShowCalculations,
  Finished
} PacketLossExpStatus; 

typedef struct PacketLossStats
{
  bool receivedFirstPacket;
  struct timeval start;
  uint32_t nextSeqNumExpected;
  uint64_t packetsReceived;
  uint64_t packetsExpected;
} PacketLossStats;

#define PACKET_LOSS_DURATION_SECONDS 60
#define PACKET_LOSS_DURATION_US SECONDS_TO_US(PACKET_LOSS_DURATION_SECONDS)

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);