/**
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * response handlers, used as a part of the OpenThread codebase. The CoAP and
 * CoAP secure source files can be found at:
 * https://github.com/UCSC-ThreadAscon/openthread/blob/main/src/cli/cli_coap.cpp#L1052
 * https://github.com/UCSC-ThreadAscon/openthread/blob/main/src/cli/cli_coap_secure.cpp#L814
*/
#include "workload.h"

void printPayloadInfo(otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  uint16_t payloadLen = getPayloadLength(aMessage);
  char payload[payloadLen];
  EmptyMemory(payload, payloadLen);
  getPayload(aMessage, payload);

  char senderAddr[OT_IP6_ADDRESS_STRING_SIZE];
  otIp6AddressToString(&(aMessageInfo->mPeerAddr), senderAddr,
                      OT_IP6_ADDRESS_STRING_SIZE);

#if CONFIG_EXPERIMENT_DEBUG
  otLogNotePlat("Response from %s of size %" PRIu16 " bytes.",
                senderAddr, payloadLen);
#endif
}

void defaultResponseCallback(void *aContext,
                             otMessage *aMessage,
                             const otMessageInfo *aMessageInfo,
                             otError aResult)
{
  if (aResult != OT_ERROR_NONE)
  {
    otLogWarnPlat("Response error: %s", otThreadErrorToString(aResult));
  }
  else 
  {
#if CONFIG_EXPERIMENT_DEBUG
    printPayloadInfo(aMessage, aMessageInfo);
#endif
  }
  return;
}