/* 
 * All of the code in this file is based upon the both CoAP and CoAP secure
 * source code used as a part of the OpenThread codebase. The CoAP and
 * CoAP secure source files can be found at:
 * https://github.com/UCSC-ThreadAscon/openthread/tree/main/src/cli
*/
#include "workload.h"

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