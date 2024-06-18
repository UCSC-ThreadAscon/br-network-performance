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

uint32_t getSequenceNum(otMessage *aMessage) {
  uint32_t sequenceNum = 0;
  getPayload(aMessage, (void *) &sequenceNum);
  return sequenceNum;
}

void printPacketInfo(otMessage *aMessage,
                    const otMessageInfo *aMessageInfo,
                    uint32_t sequenceNum,
                    double timeReceivedUs)
{
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
  uint32_t sequenceNum = getSequenceNum(aMessage);
  double elapsedUs = getElpasedUs(stats.start);

  if (!stats.receivedFirstPacket) {
    stats.receivedFirstPacket = true;
    stats.start = getTimevalNow();

    stats.packetsExpected = 1;
    stats.packetsReceived = 1;
    stats.nextSeqNumExpected = sequenceNum + 1;
  }

  if (elapsedUs <= PACKET_LOSS_DURATION_US) {
    printPacketInfo(aMessage, aMessageInfo, sequenceNum, elapsedUs);

    if (sequenceNum == stats.nextSeqNumExpected) {
      // We got the packet we expected - there is no packet loss.
      stats.packetsExpected += 1;
      stats.packetsReceived += 1;
      stats.nextSeqNumExpected = sequenceNum + 1;
    }
    else if (sequenceNum > stats.nextSeqNumExpected) {
      // We lost a packet with sequence number "sequenceNum".
      stats.packetsExpected += 1;
      stats.nextSeqNumExpected = sequenceNum + 1;
    }
    else if (sequenceNum < stats.nextSeqNumExpected) {
      // This packet arrived late. You assumed it was lost earlier,
      // but actually, it is not lost.
      stats.packetsReceived += 1;
    }

    /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
     *  since the function will only ACK if the request is a GET or Confirmable.
     */
    sendCoapResponse(aMessage, aMessageInfo);
  }

  return;
}