#include "handler.h"

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  static uint32_t packetNum = 0;
  static struct timeval startTime;
  static struct timeval endTime;

  if (packetNum < SAMPLE_SIZE_PACKETS) {
    packetNum += 1;

    if (packetNum == 1) {
      EmptyMemory(&startTime, sizeof(struct timeval));
      EmptyMemory(&endTime, sizeof(struct timeval));
      startTime = getTimevalNow();
    }

    otLogNotePlat("Received packet number %" PRIu32 ".", packetNum);
    printRequest(aMessage, aMessageInfo);

    if (packetNum == SAMPLE_SIZE_PACKETS) {
      /** The throughput formula is:
       *
       *      SAMPLE_SIZE_PACKETS * PAYLOAD_SIZE_BYTES
       *      -----------------------------------------   bytes/time
       *                    t_end - t_start
       * 
       */
      endTime = getTimevalNow();

      double denominatorUs = timeDiffMicro(startTime, endTime);
      double denominatorMs = US_TO_MS(numeratorUs);
      double denominatorSecs = US_TO_SECONDS(numeratorUs);
      double numerator = SAMPLE_SIZE_PACKETS * PAYLOAD_SIZE_BYTES;

      double throughputSecs = numerator / denominatorSecs;
      double throughputMs = numerator / denominatorMs;
      double throughputUs = numerator / denominatorUs;

      otLogNotePlat("The throughput is:");
      otLogNotePlat("%.7f bytes/second, or", throughputSecs);
      otLogNotePlat("%.7f bytes/ms, or", throughputMs);
      otLogNotePlat("%.7f bytes/us.", throughputUs);
    }

    /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
     *  since the function will only ACK if the request is a GET or Confirmable.
     */
    sendCoapResponse(aMessage, aMessageInfo);
  }
  return;
}