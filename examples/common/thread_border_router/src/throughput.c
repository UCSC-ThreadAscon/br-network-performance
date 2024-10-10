#include "handler.h"

static uint32_t packetNum;
static uint32_t totalBytes;
static struct timeval startTime;
static struct timeval endTime;

static inline void resetStaticVariables()
{
  packetNum = 0;
  totalBytes = 0;
  EmptyMemory(&startTime, sizeof(struct timeval));
  EmptyMemory(&endTime, sizeof(struct timeval));
  return;
}

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  if (packetNum < MAX_PACKETS)
  {
    packetNum += 1;
    totalBytes += getPayloadLength(aMessage);

    if (packetNum == 1) {
      otLogNotePlat("Received the first packet! Starting the Throughput Experimental Trial!");
      startTime = getTimevalNow();
    }

#if CONFIG_EXPERIMENT_DEBUG
    otLogNotePlat("Received packet number %" PRIu32 ".", packetNum);
    otLogNotePlat("Received %" PRIu32 "bytes so far", totalBytes);
    printRequest(aMessage, aMessageInfo);
#endif

    if (packetNum == MAX_PACKETS)
    {
      /** The throughput formula is:
       *
       *      MAX_PACKETS * PAYLOAD_SIZE_BYTES
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

      resetStaticVariables();
    }

    sendCoapResponse(aMessage, aMessageInfo);
  }
  return;
}