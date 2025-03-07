#include "experiment_common.h"

void tpConRequestHandler(void* aContext,
                         otMessage *aMessage,
                         const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLength = getPayloadLength(aMessage);

  /** Make sure that all packets that are sent in the throughput
   *  experiments have a payload of exactly 4 bytes.
   */
  assert(payloadLength == PAYLOAD_SIZE_BYTES);

  sendCoapResponse(aMessage, aMessageInfo);
  return;
}