#include "handler.h"

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo)
{
  printRequest(aMessage, aMessageInfo);
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}