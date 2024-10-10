/**
 * This files holds the client implementation so that the border router can send
 * a CoAP request to the "[experiment name]-start" server in order to start
 * the Throughput or Packet Loss Confirmable experiment.
 * 
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * source code, used as a part of the OpenThread codebase. The CoAP and
 * CoAP secure source files can be found at:
 * https://github.com/UCSC-ThreadAscon/openthread/tree/main/src/cli
*/
#include "handler.h"

static otSockAddr socket;

otMessage* createCoapMessage()
{
  otMessage *newMessage = otCoapNewMessage(OT_INSTANCE, NULL);
  if (newMessage == NULL) {
    otLogCritPlat("Failed to create CoAP request.");
    return NULL;
  }
  return newMessage;
}

void createMessageInfo(otSockAddr *aSocket, otMessageInfo *aMessageInfo)
{
  aMessageInfo->mHopLimit = 0;  // default

  aMessageInfo->mPeerAddr = aSocket->mAddress;
  aMessageInfo->mPeerPort = aSocket->mPort;

  aMessageInfo->mSockAddr = *otThreadGetMeshLocalEid(OT_INSTANCE);
  aMessageInfo->mSockPort = SOCK_PORT;
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

#define CONFIRMABLE_STRING "Confirmable"
#define NONCONFIRMABLE_STRING "Non-Confirmable"
#define ACK_STRING "Acknowledgement"
#define RESET_STRING "Reset"

#define PrintMessage(coapTypeString, payloadSize, destString)   \
  otLogNotePlat("Sent a %s message of %d bytes to %s.",         \
                coapTypeString, payloadSize, destString);       \

void printMessageSent(otSockAddr *socket,
                      size_t payloadSize,
                      otCoapType type)
{
  char destString[OT_IP6_ADDRESS_STRING_SIZE];

  otIp6AddressToString(&(socket->mAddress),
                       destString,
                       OT_IP6_ADDRESS_STRING_SIZE);

  switch (type) {
    case OT_COAP_TYPE_CONFIRMABLE:
      PrintMessage(CONFIRMABLE_STRING, payloadSize, destString);
      break;
    case OT_COAP_TYPE_NON_CONFIRMABLE:
      PrintMessage(NONCONFIRMABLE_STRING, payloadSize, destString);
      break;
    case OT_COAP_TYPE_ACKNOWLEDGMENT:
      PrintMessage(ACK_STRING, payloadSize, destString);
      break;
    case OT_COAP_TYPE_RESET:
      PrintMessage(RESET_STRING, payloadSize, destString);
      break;
    default:
      otLogCritPlat("The response has an invalid CoAP message type.");
  }

  return;
}

void request(otSockAddr *socket,
             void *payload,
             size_t payloadSize,
             const char *uri,
             otCoapResponseHandler responseCallback,
             otCoapType type)
{
  otMessageInfo aMessageInfo;
  otMessage *aRequest;

  EmptyMemory(&aMessageInfo, sizeof(otMessageInfo));
  createMessageInfo(socket, &aMessageInfo);

  aRequest = createCoapMessage();

  createHeaders(aRequest, &aMessageInfo, uri, type);
  addPayload(aRequest, payload, payloadSize);
  send(aRequest, &aMessageInfo, responseCallback);

#if CONFIG_EXPERIMENT_DEBUG
  printMessageSent(socket, payloadSize, type);
#endif
  return;
}

void coapStart() {
  otError error = otCoapStart(OT_INSTANCE, SOCK_PORT);

  if (error != OT_ERROR_NONE) {
    otLogCritPlat("Failed to start COAP socket.");
  }
  else {
    otLogNotePlat("Started CoAP socket at port %d.", SOCK_PORT);
  }
  return;
}

otSockAddr createSocket(const char *recvAddrString)
{
  otSockAddr newSocket;
  otIp6Address recvAddr;

  EmptyMemory(&newSocket, sizeof(otSockAddr));
  EmptyMemory(&recvAddr, sizeof(otIp6Address));

  otIp6AddressFromString(recvAddrString, &recvAddr);

  newSocket.mAddress = recvAddr;
  newSocket.mPort = SOCK_PORT;

  return newSocket;
}

static inline void InitSocket(otSockAddr *socketPtr, const char* serverAddr)
{
  EmptyMemory(socketPtr, sizeof(otSockAddr));
  *socketPtr = createSocket(serverAddr);
  return;
}

void tpConfirmableStartExperiment(otChangedFlags changed_flags, void* ctx)
{
  OT_UNUSED_VARIABLE(ctx);
  static otDeviceRole s_previous_role = OT_DEVICE_ROLE_DISABLED;

  otInstance* instance = esp_openthread_get_instance();
  if (!instance)
  {
    return;
  }

  otDeviceRole role = otThreadGetDeviceRole(instance);
  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    /** Send a NON-Confirmable CoAP Request to the "/[experiment]-start"
     *  route in the FTD to begin the Throughput or Packet Loss experiment.
     *
     *  The payload will be empty. The purpose of the request is to tell
     *  the FTD to start sending the Throughput or Packet Loss experiment packets.
     */
    coapStart();
    InitSocket(&socket, CONFIG_FTD_IP_ADDRESS);
    request(socket, NULL, 0, THROUGHPUT_START_SERVER_URI,
            NULL, OT_COAP_TYPE_NON_CONFIRMABLE);
  }
  s_previous_role = role;
  return;
}