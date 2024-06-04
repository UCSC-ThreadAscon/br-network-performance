/**
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * source code used as a part of the OpenThread codebase.
 *
 * https://github.com/UCSC-ThreadAscon/openthread/tree/main/src/cli
*/
#include "workload.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define PRINT_UPTIME 1
#define PRINT_PAYLOADS 0

void getPeerAddrString(const otMessageInfo *aMessageInfo, char *ipString) {
  otIp6AddressToString(&(aMessageInfo->mPeerAddr), ipString,
                       OT_IP6_ADDRESS_STRING_SIZE);
  return;
}

/**
 * This function is a modified version of `HandleRequest()` from the OpenThread CLI
 * Secure CoAP source code:
 * https://github.com/UCSC-ThreadAscon/openthread/blob/main/src/cli/cli_coap_secure.cpp#L814
*/
void sendCoapResponse(otMessage *aRequest, const otMessageInfo *aRequestInfo)
{
  otMessage *aResponse = NULL;
  otCoapCode status = OT_COAP_CODE_EMPTY;

  bool isConfirmable = otCoapMessageGetType(aRequest) == OT_COAP_TYPE_CONFIRMABLE;
  bool isGet = otCoapMessageGetCode(aRequest) == OT_COAP_CODE_GET;

  if (isConfirmable || isGet) {
    if (isGet) {
      status = OT_COAP_CODE_CONTENT;
    }
    else {
      status = OT_COAP_CODE_VALID;
    }

    aResponse = otCoapNewMessage(OT_INSTANCE, NULL);
    if (aResponse == NULL) {
      otLogCritPlat("Failed to initialize a new message for CoAP response.");
    }

    otError error = otCoapMessageInitResponse(aResponse, aRequest,
                                              OT_COAP_TYPE_ACKNOWLEDGMENT,
                                              status);
    HandleMessageError("coap message init response", aResponse, error);

    error = otCoapSendResponse(OT_INSTANCE, aResponse, aRequestInfo);
    HandleMessageError("send response", aResponse, error);
  }

  return;
}

void defaultRequestHandler(void* aContext,
                           otMessage *aMessage,
                           const otMessageInfo *aMessageInfo)
{
  uint32_t length = getPayloadLength(aMessage);

  char sender[OT_IP6_ADDRESS_STRING_SIZE];
  EmptyMemory(sender, OT_IP6_ADDRESS_STRING_SIZE);
  getPeerAddrString(aMessageInfo, sender);

  otLogNotePlat("Received %" PRIu32 " bytes from %s", length, sender);

  sendCoapResponse(aMessage, aMessageInfo);
  return;
}

otError createResource(otCoapResource *resource, Test test)
{
  resource->mNext = NULL;
  resource->mContext = NULL;

  switch (test) {
    case Throughput:
      resource->mUriPath = THROUGHPUT_URI;
      break;
    case PacketLoss:
      resource->mUriPath = PACKET_LOSS_URI;
      break;
    case Delay:
      resource->mUriPath = DELAY_URI;
      break;
    default:
      otLogCritPlat("Failed to create resource: test does not exist.");
      resource->mUriPath = "";
  }

  return OT_ERROR_NONE;
}