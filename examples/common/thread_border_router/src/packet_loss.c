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

/**
 * You get packet "p_j", the packet you expect to get next "p_i".
 * That means you’re missing packets "p_k", for i <= k < j. That's
 * "j - i" packets lost.
 */
uint32_t getPacketsLost(uint32_t seqNumCur, PacketLossStats stats)
{
  uint32_t seqNumExpected = stats.nextSeqNumExpected;
  uint32_t packetsLost = seqNumCur - seqNumExpected;

  otLogNotePlat("Expected Packet %" PRIu32 ".", seqNumExpected);
  otLogNotePlat("Got Packet %" PRIu32 ".", seqNumCur);
  otLogNotePlat("Packets Lost: %" PRIu32 ".", packetsLost);
  return packetsLost;
}

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  if (stats.state == Finished) { return; }
  PrintDelimiter();

  if (stats.state == NotStarted) {
    stats.start = getTimevalNow();

    stats.packetsExpected = 1;
    stats.packetsReceived = 1;
    stats.nextSeqNumExpected = getSequenceNum(aMessage) + 1;
    stats.state = Counting;
  }

  if (stats.state == Counting) {
    uint32_t sequenceNum = getSequenceNum(aMessage);
    double elapsedUs = getElpasedUs(stats.start);

    if (elapsedUs <= PACKET_LOSS_DURATION_US) {
      printPacketInfo(aMessage, aMessageInfo, sequenceNum, elapsedUs);

      // No packet loss.
      if (sequenceNum == stats.nextSeqNumExpected) {
        stats.packetsExpected += 1;
        stats.packetsReceived += 1;
        stats.nextSeqNumExpected = sequenceNum + 1;
      }

      // Packet loss.
      else if (sequenceNum > stats.nextSeqNumExpected) {
        // Expected to receive the packets lost.
        stats.packetsExpected += getPacketsLost(sequenceNum, stats);

        // We expected AND received the current packet.
        stats.packetsExpected += 1;
        stats.packetsReceived += 1;

        stats.nextSeqNumExpected = sequenceNum + 1;
      }

      /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
       *  since the function will only ACK if the request is a GET or Confirmable.
       */
      sendCoapResponse(aMessage, aMessageInfo);
    }
    else {
      stats.state = DisplayedResults;
    }
  }

  if (stats.state == DisplayedResults) {
    double packetLossRatio = ((double) stats.packetsReceived) /
                             ((double) stats.packetsExpected); 
    PrintPacketLossResults(stats, packetLossRatio);
    stats.state = Finished;
  }

  PrintDelimiter();
  return;
}