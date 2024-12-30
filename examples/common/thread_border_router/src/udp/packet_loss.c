#include "workload.h"
#include "handler.h"

#define EXPECTED_MAX_PACKETS 1000 * 1000

static uint32_t numReceived;

void printPacketLoss()
{
  uint32_t numPacketsLost = EXPECTED_MAX_PACKETS - numReceived;
  double packetLoss = ((double) numPacketsLost) / EXPECTED_MAX_PACKETS;

  PrintDelimiter();
  otLogNotePlat("Received: %" PRIu32 " packets", numReceived);
  otLogNotePlat("Packets Lost: %" PRIu32 " packets", numPacketsLost);
  otLogNotePlat("Expected: %d packets", EXPECTED_MAX_PACKETS);
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