/**
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * source code, used as a part of the OpenThread codebase. The CoAP and
 * CoAP secure source files can be found at:
 * https://github.com/UCSC-ThreadAscon/openthread/tree/main/src/cli
*/
#include "workload.h"

#include <string.h>

otMessage* createCoapMessage()
{
  otMessage *newMessage = otCoapNewMessage(OT_INSTANCE, NULL);
  if (newMessage == NULL) {
    otLogCritPlat("Failed to create CoAP request.");
    return NULL;
  }
  return newMessage;
}

void createMessageInfo(otSockAddr *aSockAddr, otMessageInfo *aMessageInfo)
{
  aMessageInfo->mHopLimit = 0;  // default

  aMessageInfo->mPeerAddr = aSockAddr->mAddress;
  aMessageInfo->mPeerPort = aSockAddr->mPort;

  aMessageInfo->mSockAddr = *otThreadGetMeshLocalEid(OT_INSTANCE);
  aMessageInfo->mSockPort = COAP_SOCK_PORT;
  return;
}

void createHeaders(otMessage *aRequest,
                   otMessageInfo *aMessageInfo,
                   const char *uri,
                   otCoapType type)
{
  otError error = OT_ERROR_NONE;

  otCoapMessageInit(aRequest, type, OT_COAP_CODE_GET);
  otCoapMessageGenerateToken(aRequest, OT_COAP_DEFAULT_TOKEN_LENGTH);

  error = otCoapMessageAppendUriPathOptions(aRequest, uri);
  HandleMessageError("append uri options", aRequest, error);

  return;
}

void saveSubscriptionToken(otMessage *aRequest, Subscription *subscription)
{
  subscription->tokenLength = otCoapMessageGetTokenLength(aRequest);
  memcpy(&(subscription->token), otCoapMessageGetToken(aRequest), subscription->tokenLength);
  return;
}

void makeObserveRequest(otMessage *aRequest)
{
  otError error = otCoapMessageAppendObserveOption(aRequest, OBSERVE_SUBSCRIBE);
  HandleMessageError("append observe option", aRequest, error);
  return;
}

void send(otMessage *aRequest,
          otMessageInfo *aMessageInfo,
          otCoapResponseHandler responseCallback)
{
  otError error = otCoapSendRequest(OT_INSTANCE, aRequest, aMessageInfo,
                                    responseCallback, NULL);
  HandleMessageError("send request", aRequest, error);
  return;
}

void observeRequest(Subscription *subscription,
                    const char *uri,
                    otCoapResponseHandler responseCallback,
                    otCoapType type)
{
  otMessageInfo aMessageInfo;
  otMessage *aRequest;

  EmptyMemory(&aMessageInfo, sizeof(otMessageInfo));
  createMessageInfo(&(subscription->sockAddr), &aMessageInfo);

  aRequest = createCoapMessage();
  createHeaders(aRequest, &aMessageInfo, uri, type);

  saveSubscriptionToken(aRequest, subscription);
  makeObserveRequest(aRequest);

  send(aRequest, &aMessageInfo, responseCallback);
  return;
}
