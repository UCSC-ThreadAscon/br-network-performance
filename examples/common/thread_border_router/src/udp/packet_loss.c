#include "workload.h"
#include "handler.h"

static uint32_t numReceived;

void printPacketLoss()
{
  assert(numReceived <= MAX_PACKETS);

  uint32_t numPacketsLost = MAX_PACKETS - numReceived;
  double packetLoss = ((double) numPacketsLost) / MAX_PACKETS;

  PrintDelimiter();
  otLogNotePlat("Received: %" PRIu32 " packets", numReceived);
  otLogNotePlat("Packets Lost: %" PRIu32 " packets", numPacketsLost);
  otLogNotePlat("Expected: %d packets", MAX_PACKETS);
  otLogNotePlat("Packet Loss Ratio: %.15f", packetLoss);
  PrintDelimiter();
  return;
}

void plUdpRequestHandler(void *aContext,
                        otMessage *aMessage,
                        const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLength = getPayloadLength(aMessage);
  assert(payloadLength == PAYLOAD_SIZE_BYTES);
#if CONFIG_EXPERIMENT_DEBUG
  otLogNotePlat("Received a UDP packet with a length of %" PRIu16 " bytes.", payloadLength);
#endif

  numReceived += 1;
  printPacketLoss();
  return;
}