#include "workload.h"
#include "handler.h"

static otCoapResource *confirmableUri;
static otCoapResource *nonConfirmableUri;

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

  createResource(confirmableUri, Confirmable, "Throughput Confirmable",
                 throughputRequestHandler);
  createResource(nonConfirmableUri, NonConfirmable, "Throughput Non-Confirmable",
                 throughputRequestHandler);

  return OT_ERROR_NONE;
}

otError expServerFree(void* aContext, uint8_t argsLength, char* aArgs[])
{
  resourceDestructor(confirmableUri);
  resourceDestructor(nonConfirmableUri);

  otCoapStop(OT_INSTANCE);
  return OT_ERROR_NONE;
}