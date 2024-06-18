#include "handler.h"

static PacketLossStats stats = {
  false,      // receivedFirstPacket
  { 0, 0 },   // start timeval
  0,          // nextSeqNumExpected
  0,          // packetsReceived
  0           // packetsExpected
};

double getElpasedUs(struct timeval start)
{
  return (double) timeDiffMicro(start, getTimevalNow());
}

void printPacketInfo(otMessage *aMessage,
                    const otMessageInfo *aMessageInfo,
                    double timeReceivedUs)
{
  uint32_t sequenceNum = 0;
  getPayload(aMessage, (void *) &sequenceNum);

  printRequest(aMessage, aMessageInfo);
  otLogNotePlat("Packet has sequence number %" PRIu32 ".", sequenceNum);
  otLogNotePlat("The packet has been received at ~%.5f seconds.",
                US_TO_SECONDS(timeReceivedUs));
  return;
}

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  if (!stats.receivedFirstPacket) {
    stats.receivedFirstPacket = true;
    stats.start = getTimevalNow();
  }

  double elapsedUs = getElpasedUs(stats.start);
  if (elapsedUs <= PACKET_LOSS_DURATION_US) {
    printPacketInfo(aMessage, aMessageInfo, elapsedUs);

    /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
     *  since the function will only ACK if the request is a GET or Confirmable.
     */
    sendCoapResponse(aMessage, aMessageInfo);
  }

  return;
}