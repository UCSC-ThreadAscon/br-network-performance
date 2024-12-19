#include "workload.h"
#include "handler.h"

#define EXPECTED_TOTAL_BYTES 4000

static uint32_t packetsRecv;
static uint32_t totalBytes;
static struct timeval startTime;
static struct timeval endTime;

static otUdpSocket udpSocket;
static otSockAddr udpSockAddr;

void tpUdpStartServer(void *aContext,
                      otMessage *aMessage,
                      const otMessageInfo *aMessageInfo,
                      otError aResult)
{
  if (aResult != OT_ERROR_NONE) {
    EmptyMemory(&udpSocket, sizeof(otUdpSocket));

    handleError(otUdpOpen(OT_INSTANCE, &udpSocket, callback, tpUdpRequestHandler),
                "Failed to open UDP socket.");

    udpSockAddr.mAddress = *otThreadGetMeshLocalEid(OT_INSTANCE);
    udpSockAddr.mPort = UDP_SOCK_PORT;
    handleError(otUdpBind(OT_INSTANCE, &udpSocket, &udpSockAddr, OT_NETIF_THREAD),
                "Failed to set up UDP server.");
    
    otLogNotePlat("Created UDP server at port %d.", UDP_SOCK_PORT);
  }
  else {
    PrintCritDelimiter();
    otLogCritPlat("Border Router failed to attach to the Thread network lead by the FTD.");
    otLogCritPlat("Going to restart the current experiment trial.");
    PrintCritDelimiter();

    esp_restart();
  }
  return;
}

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
    otLogNotePlat("Starting the Throughput Confirmable experiment trial!");
    PrintDelimiter();

    startTime = getTimevalNow();
  }

  if (packetsRecv < MAX_PACKETS)
  {
    packetsRecv += 1;
    totalBytes += payloadLength;

    if (packetsRecv == MAX_PACKETS)
    {
      assert(totalBytes == EXPECTED_TOTAL_BYTES);

      /** The throughput formula is:
       *
       *           MAX_PACKETS * PAYLOAD_SIZE_BYTES
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

      startNextTrial();
      return;
    }
  }
  return;
}