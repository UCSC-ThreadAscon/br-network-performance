#include "workload.h"
#include "handler.h"

void tpUdpRequestHandler(void *aContext,
                        otMessage *aMessage,
                        const otMessageInfo *aMessageInfo)
{
  otLogNotePlat("Received UDP packet of %d bytes.", getPayloadLength(aMessage));
  return;
}