#include "workload.h"

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

  otCoapMessageInit(aRequest, type, OT_COAP_CODE_POST);
  otCoapMessageGenerateToken(aRequest, OT_COAP_DEFAULT_TOKEN_LENGTH);

  error = otCoapMessageAppendUriPathOptions(aRequest, uri);
  HandleMessageError("append uri options", aRequest, error);

  return;
}

void addPayload(otMessage *aRequest,
                void *payload,
                size_t payloadSize)
{
  otError error = OT_ERROR_NONE;

  error = otCoapMessageSetPayloadMarker(aRequest);
  HandleMessageError("set payload marker", aRequest, error);

  error = otMessageAppend(aRequest, payload, payloadSize);
  HandleMessageError("message append", aRequest, error);

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

void observeRequest(otSockAddr *sockAddr,
                    void *payload,
                    size_t payloadSize,
                    const char *uri,
                    otCoapResponseHandler responseCallback,
                    otCoapType type)
{
  otMessageInfo aMessageInfo;
  otMessage *aRequest;

  EmptyMemory(&aMessageInfo, sizeof(otMessageInfo));
  createMessageInfo(sockAddr, &aMessageInfo);

  aRequest = createCoapMessage();

  createHeaders(aRequest, &aMessageInfo, uri, type);
  addPayload(aRequest, payload, payloadSize);
  send(aRequest, &aMessageInfo, responseCallback);
  return;
}
