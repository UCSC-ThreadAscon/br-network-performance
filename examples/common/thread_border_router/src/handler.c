#include "handler.h"

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo)
{
  printRequest(aMessage, aMessageInfo);
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  otLogNotePlat("Hello from the Throughput Request Handler!");
  // printRequest(aMessage, aMessageInfo);

  /**
   * Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   * since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}