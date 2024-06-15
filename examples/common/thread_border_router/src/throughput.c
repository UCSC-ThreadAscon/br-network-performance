#include "handler.h"

/**
 * TODO:
 *    1. When the FIRST packet p_1 is received, grab the START TIME.
 *       Initialize a counter to record how many packets have been
 *       received so far.
 *
 *    2. For every p_i, for 1 < i < SAMPLE_SIZE_PACKETS, increment
 *       the counter.
 *
 *    3. for p_SAMPLE_SIZE_PACKETS, grab the END TIME. Using both the
 *       START and END TIME, record the throughput.
 */
void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  static uint32_t numPackets = 0;
  static struct timeval startTime;
  static struct timeval endTime;

  numPackets += 1;

  /**
   * First packet is received. Initialize static variables,
   * and grab the start time.
   */
  if (numPackets == 1) {
    EmptyMemory(&startTime, sizeof(struct timeval));
    EmptyMemory(&endTime, sizeof(struct timeval));

    startTime = getTimevalNow();
    otLogNotePlat("Received First Packet!");
  }

  printRequest(aMessage, aMessageInfo);

  /**
   * Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   * since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}