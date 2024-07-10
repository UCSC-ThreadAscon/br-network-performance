#include "handler.h"

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  static uint32_t received = 0;
  received += 1;

  PrintDelimiter();
  printRequest(aMessage, aMessageInfo);
  otLogNotePlat("Received: %" PRIu32 " packets.", received);
  PrintDelimiter();

  /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   *  since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}