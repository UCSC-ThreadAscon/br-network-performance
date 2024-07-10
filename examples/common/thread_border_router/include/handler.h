#pragma once

#include "workload.h"
#include "time_api.h"

#define PACKET_LOSS_MAX_PACKETS 1000

#define PrintDelimiter() otLogNotePlat("---------------------------");

#define PACKET_LOSS_DURATION_SECONDS 60
#define PACKET_LOSS_DURATION_US SECONDS_TO_US(PACKET_LOSS_DURATION_SECONDS)

#define PrintPacketLossResults(stats, ratio)                                 \
  otLogNotePlat("Expected packets: %" PRIu32 ".", stats.packetsExpected);    \
  otLogNotePlat("Packets Received: %" PRIu32 ".", stats.packetsReceived);    \
  otLogNotePlat("Packet loss ratio: %.5f.", ratio);                          \

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo);

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo);