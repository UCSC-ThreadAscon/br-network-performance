#include "workload.h"
#include "handler.h"

static uint32_t packetsRecv;
static uint32_t totalBytes;
static struct timeval startTime;
static struct timeval endTime;

#define EXPECTED_TOTAL_BYTES (UDP_MAX_PACKETS * PAYLOAD_SIZE_BYTES)

void tpUdpRequestHandler(void *aContext,
                        otMessage *aMessage,
                        const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLength = getPayloadLength(aMessage);
  assert(payloadLength == PAYLOAD_SIZE_BYTES);
#if CONFIG_EXPERIMENT_DEBUG
  otLogNotePlat("Received a UDP packet with a length of %" PRIu16 " bytes.", payloadLength);
#endif

  if (packetsRecv == 0)
  {
    assert(totalBytes == 0);
    resetTrials();

    PrintDelimiter();
    otLogNotePlat("Starting the Throughput UDP experiment!");
    PrintDelimiter();

    startTime = getTimevalNow();
  }

  if (packetsRecv < UDP_MAX_PACKETS)
  {
    packetsRecv += 1;
    totalBytes += payloadLength;

    if (packetsRecv == UDP_MAX_PACKETS)
    {
      assert(totalBytes == EXPECTED_TOTAL_BYTES);

      /** The throughput formula is:
       *
       *        UDP_MAX_PACKETS * PAYLOAD_SIZE_BYTES
       *      -----------------------------------------   bytes/time
       *                    t_end - t_start
       * 
       */
      endTime = getTimevalNow();

      uint64_t usElapsed = timeDiffUs(startTime, endTime);

      double denominatorUs = (double) usElapsed;
      double denominatorMs = US_TO_MS((double) denominatorUs);
      double denominatorSecs = US_TO_SECONDS((double) denominatorUs);

      double throughputSecs = (double) totalBytes / denominatorSecs;
      double throughputMs = (double) totalBytes / denominatorMs;
      double throughputUs = (double) totalBytes / denominatorUs;

      /**
       * I found that doubles have 15 digits of precision from:
       * https://stackoverflow.com/a/2386882/6621292
       */
      PrintDelimiter();
      otLogNotePlat("The throughput is:");
      otLogNotePlat("%.15f bytes/second, or", throughputSecs);
      otLogNotePlat("%.15f bytes/ms, or", throughputMs);
      otLogNotePlat("%.15f bytes/us.", throughputUs);
      PrintDelimiter();

      PrintDelimiter();
      otLogNotePlat("Time Elapsed:");
      otLogNotePlat("%.15f seconds.", denominatorSecs);
      otLogNotePlat("%.15f ms, or", denominatorMs);
      otLogNotePlat("%" PRIu64 " us.", usElapsed);
      otLogNotePlat("Start Timestamp: %" PRIu64 "", toUs(startTime));
      otLogNotePlat("End Timestamp: %" PRIu64 "", toUs(endTime));
      PrintDelimiter();

      PrintDelimiter();
      otLogNotePlat("Total Received: %" PRIu32 " bytes", totalBytes);
      otLogNotePlat("Number of packets received: %" PRIu32 "", packetsRecv);
      PrintDelimiter();

      otLogNotePlat("Finished running the Throughput UDP experiment.");
      return;
    }
  }
  return;
}