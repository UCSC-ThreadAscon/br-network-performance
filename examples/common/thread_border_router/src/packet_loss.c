#include "handler.h"

static PacketLossStats stats =
{
  false,      // receivedFirstPacket
  0,          // start
  0,          // nextSeqNumExpected
  0,          // packetsReceived
  0           // packetsExpected
};

/**
 * TODO:
 *  1. When receiving the first packet, record the START TIME.
 *
 *  2. For each packet received afterwards, print out the time in which they were received
 *     RELATIVE to the START TIME.
 *
 *  3. For the first packet received AFTER 30 seconds, STOP counting packets.
 */
void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  if (!stats.receivedFirstPacket) {
    stats.receivedFirstPacket = true;
    stats.start = getTimevalNow();
  }

  printRequest(aMessage, aMessageInfo);
  uint32_t sequenceNum = 0;
  getPayload(aMessage, (void *) &sequenceNum);
  otLogNotePlat("Packet has sequence number %" PRIu32 ".", sequenceNum);

  /**
   * Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   * since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}