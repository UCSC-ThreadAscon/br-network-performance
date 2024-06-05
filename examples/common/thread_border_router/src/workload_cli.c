#include "workload.h"

static otCoapResource *throughput;

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

  throughput = calloc(1, sizeof(otCoapResource));
  createResource(throughput, Throughput);

  otLogNotePlat("Created throughput server at /%s.", throughput->mUriPath);

  return OT_ERROR_NONE;
}

otError expServerFree(void* aContext, uint8_t argsLength, char* aArgs[])
{
  otCoapRemoveResource(OT_INSTANCE, throughput);
  otCoapStop(OT_INSTANCE);
  free(throughput);
  otLogNotePlat("Closed throughput server.");
  return OT_ERROR_NONE;
}