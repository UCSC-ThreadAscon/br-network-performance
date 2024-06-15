/**
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * source code used as a part of the OpenThread codebase.
 *
 * https://github.com/UCSC-ThreadAscon/openthread/tree/main/src/cli
*/
#include "workload.h"
#include "handler.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

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

#define CONFIRMABLE_STRING "Confirmable"
#define NONCONFIRMABLE_STRING "Non-Confirmable"
#define ACK_STRING "Acknowledgement"
#define RESET_STRING "Reset"

#define PrintMessage(coapTypeString, length, sender)          \
  otLogNotePlat("Received %" PRIu32 " bytes, %s, from %s.",   \
                length, coapTypeString, sender);              \

void printRequest(otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  uint32_t length = getPayloadLength(aMessage);

  char sender[OT_IP6_ADDRESS_STRING_SIZE];
  EmptyMemory(sender, OT_IP6_ADDRESS_STRING_SIZE);
  getPeerAddrString(aMessageInfo, sender);

  switch (otCoapMessageGetType(aMessage))
  {
    case OT_COAP_TYPE_CONFIRMABLE:
      PrintMessage(CONFIRMABLE_STRING, length, sender);
      break;
    case OT_COAP_TYPE_NON_CONFIRMABLE:
      PrintMessage(NONCONFIRMABLE_STRING, length, sender);
      break;
    case OT_COAP_TYPE_ACKNOWLEDGMENT:
      PrintMessage(ACK_STRING, length, sender);
      break;
    case OT_COAP_TYPE_RESET:
      PrintMessage(RESET_STRING, length, sender);
      break;
    default:
      otLogCritPlat("The request has an invalid CoAP message type.");
  }
  return;
}

otError createResource(otCoapResource *resource,
                       Test test,
                       const char *resourceName,
                       otCoapRequestHandler requestHandler)
{
  resource = calloc(1, sizeof(otCoapResource));
  resource->mNext = NULL;
  resource->mContext = NULL;
  resource->mHandler = requestHandler;

  switch (test) {
    case Confirmable:
      resource->mUriPath = THROUGHPUT_CONFIRMABLE_URI;
      break;
    case NonConfirmable:
      resource->mUriPath = THROUGHPUT_NONCONFIRMABLE_URI;
      break;
    case PacketLoss:
      resource->mUriPath = PACKET_LOSS_URI;
      break;
    case Delay:
      resource->mUriPath = DELAY_URI;
      break;
    default:
      otLogCritPlat("Failed to create resource.");
      resource->mUriPath = "";
  }

  otCoapAddResource(OT_INSTANCE, resource);
  otLogNotePlat("Created %s server at '%s'.", resourceName, resource->mUriPath);
  return OT_ERROR_NONE;
}

void resourceDestructor(otCoapResource *resource)
{
  otLogNotePlat("Closing '%s'", resource->mUriPath);
  otCoapRemoveResource(OT_INSTANCE, resource);
  free(resource);
  return;
}