#include "handler.h"

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  printRequest(aMessage, aMessageInfo);

  uint32_t sequenceNum = 0;
  getPayload(aMessage, (void *) &sequenceNum);

  otLogNotePlat("Packet has sequence number %" PRIu32 ".", sequenceNum);

  /**
   * Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   * since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}