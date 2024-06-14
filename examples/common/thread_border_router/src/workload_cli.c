#include "workload.h"

static otCoapResource *confirmable;
static otCoapResource *nonConfirmable;

void startCoapServer(uint16_t port) {
  otError error = otCoapStart(OT_INSTANCE, port);

  if (error != OT_ERROR_NONE) {
    otLogCritPlat("Failed to start COAP server.");
  } else {
    otLogNotePlat("Started CoAP server at port %d.", port);
  }
  return;
}

otError expServerStart(void* aContext, uint8_t argsLength, char* aArgs[]) 
{
  checkConnection(OT_INSTANCE);
  startCoapServer(OT_DEFAULT_COAP_PORT);

  createResource(confirmable, Confirmable, "Throughput Confirmable");
  createResource(nonConfirmable, NonConfirmable, "Throughput Non-Confirmable");

  return OT_ERROR_NONE;
}

otError expServerFree(void* aContext, uint8_t argsLength, char* aArgs[])
{
  resourceDestructor(confirmable);
  resourceDestructor(nonConfirmable);

  otCoapStop(OT_INSTANCE);
  return OT_ERROR_NONE;
}