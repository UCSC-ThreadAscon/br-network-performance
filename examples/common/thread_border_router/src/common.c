#include "workload.h"

#include <openthread/thread.h>
#include <openthread/platform/radio.h>

#include <assert.h>

void coapStart() {
  otError error = otCoapStart(OT_INSTANCE, COAP_SOCK_PORT);

  if (error != OT_ERROR_NONE) {
    otLogCritPlat("Failed to start COAP service.");
  }
  else {
    otLogNotePlat("Started CoAP service at port %d.", COAP_SOCK_PORT);
  }
  return;
}

otSockAddr createSockAddr(const char *recvAddrString)
{
  otSockAddr newSockAddr;
  otIp6Address recvAddr;

  EmptyMemory(&newSockAddr, sizeof(otSockAddr));
  EmptyMemory(&recvAddr, sizeof(otIp6Address));

  otIp6AddressFromString(recvAddrString, &recvAddr);

  newSockAddr.mAddress = recvAddr;
  newSockAddr.mPort = COAP_SOCK_PORT;

  return newSockAddr;
}

uint16_t getPayloadLength(const otMessage *aMessage) {
  return otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
}

/**
 * Based upon the OpenThread CLI UDP Handler code.
 * https://github.com/openthread/openthread/blob/main/src/cli/cli_udp.cpp#L461
 *
 * According to the handler code, `otMessageGetOffset()` returns the end
 * of the Message header, while `otMessageGetLength()` returns the size
 * of the Message header, payload included.
*/
void getPayload(const otMessage *aMessage, void* buffer) {
  uint16_t offset = otMessageGetOffset(aMessage);
  uint16_t length = getPayloadLength(aMessage);

  uint16_t bytesRead = otMessageRead(aMessage, offset, buffer, length);
  assert(bytesRead == length);
  return;
}
