#include "handler.h"

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}