#include "handler.h"

# define THROUGHPUT_EXP_PAYLOAD_SIZE 4

void throughputRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLength = getPayloadLength(aMessage);

  /** Make sure that all packets that are sent in the throughput
   *  experiments have a payload of exactly 4 bytes.
   */
  assert(payloadLength == THROUGHPUT_EXP_PAYLOAD_SIZE);

  sendCoapResponse(aMessage, aMessageInfo);
  return;
}