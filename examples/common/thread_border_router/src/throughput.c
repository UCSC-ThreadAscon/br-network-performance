#include "handler.h"

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  static uint32_t packetNum = 0;
  static uint32_t totalBytes = 0;
  static struct timeval startTime;
  static struct timeval endTime;

  if (packetNum < SAMPLE_SIZE_PACKETS) {
    packetNum += 1;
    totalBytes += getPayloadLength(aMessage);

    if (packetNum == 1) {
      otLogNotePlat("Received the first packet! Starting throughput experiment trial.");

      EmptyMemory(&startTime, sizeof(struct timeval));
      EmptyMemory(&endTime, sizeof(struct timeval));
      startTime = getTimevalNow();
    }

#if CONFIG_EXPERIMENT_DEBUG
    otLogNotePlat("Received packet number %" PRIu32 ".", packetNum);
    otLogNotePlat("Recevied %" PRIu32 "bytes so far", totalBytes);
    printRequest(aMessage, aMessageInfo);
#endif

    if (packetNum == SAMPLE_SIZE_PACKETS)
    {
      /** The throughput formula is:
       *
       *      SAMPLE_SIZE_PACKETS * PAYLOAD_SIZE_BYTES
       *      -----------------------------------------   bytes/time
       *                    t_end - t_start
       * 
       */
      endTime = getTimevalNow();

      double denominatorUs = timeDiffUs(startTime, endTime);
      double denominatorMs = US_TO_MS(denominatorUs);
      double denominatorSecs = US_TO_SECONDS(denominatorUs);

      double throughputSecs = totalBytes / denominatorSecs;
      double throughputMs = totalBytes / denominatorMs;
      double throughputUs = totalBytes / denominatorUs;

      PrintDelimiter();
      otLogNotePlat("The throughput is:");
      otLogNotePlat("%.7f bytes/second, or", throughputSecs);
      otLogNotePlat("%.7f bytes/ms, or", throughputMs);
      otLogNotePlat("%.7f bytes/us.", throughputUs);
      otLogNotePlat("Duration: %.7f seconds", denominatorSecs);
      otLogNotePlat("Total Received: %" PRIu32 " bytes", totalBytes);
      PrintDelimiter();
    }

    /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
     *  since the function will only ACK if the request is a GET or Confirmable.
     */
    sendCoapResponse(aMessage, aMessageInfo);
  }
  return;
}