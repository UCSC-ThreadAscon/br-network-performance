#include "handler.h"

static PacketLossStats stats = {
  { 0, 0 },     // start timeval
  0,            // nextSeqNumExpected
  0,            // packetsReceived
  0,            // packetsExpected
  NotStarted    // state enum
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
  otLogNotePlat("------------------");
  printRequest(aMessage, aMessageInfo);
  uint32_t sequenceNum = getSequenceNum(aMessage);
  otLogNotePlat("The sequence number is %" PRIu32 ".", sequenceNum);
  double elapsedUs = getElpasedUs(stats.start);

  if (stats.state == NotStarted) {
    stats.state = Counting;
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
  else {
    if (stats.state == Counting) {
      otLogNotePlat("Expected packets: %" PRIu64 ".", stats.packetsExpected);
      otLogNotePlat("Packets Received: %" PRIu64 ".", stats.packetsReceived);

      double packetLossRatio = ((double) stats.packetsReceived) /
                               ((double) stats.packetsExpected); 
      otLogNotePlat("Packet loss ratio: %.5f.", packetLossRatio);

      stats.state = DisplayedResults;
    }
    else if (stats.state == DisplayedResults) {
      /** Packet loss has already been counted and displayed. The server
       *  will not be doing anymore packet loss calculations.
       */
      stats.state = Finished;
    }
  }

  otLogNotePlat("------------------");
  return;
}