#include "handler.h"

void plConRequestHandler(void* aContext,
                         otMessage *aMessage,
                         const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLength = getPayloadLength(aMessage);

  /** Make sure that all packets that are sent in the packet loss
   *  experiments have a payload of exactly 4 bytes.
   */
  assert(payloadLength == PAYLOAD_SIZE_BYTES);

  sendCoapResponse(aMessage, aMessageInfo);
  return;
}