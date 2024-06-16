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
    case ThroughputConfirmable:
      resource->mUriPath = THROUGHPUT_CONFIRMABLE_URI;
      break;
    case ThroughputNonConfirmable:
      resource->mUriPath = THROUGHPUT_NONCONFIRMABLE_URI;
      break;
    case PacketLossConfirmable:
      resource->mUriPath = PACKET_LOSS_CONFIRMABLE_URI;
      break;
    case PacketLossNonConfirmable:
      resource->mUriPath = PACKET_LOSS_NONCONFIRMABLE_URI;
      break;
    default:
      otLogCritPlat("Failed to create resource: test not defined.");
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