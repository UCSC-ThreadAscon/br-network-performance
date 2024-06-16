#include "workload.h"
#include "handler.h"

static otCoapResource *experimentRoute;

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

#if NO_EXPERIMENT
  otLogNotePlat("No experiments to set up.");
  otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
  otLogNotePlat("experiment the CoAP server will run.");
#elif EXPERIMENT_THROUGHPUT_CONFIRMABLE
  createResource(experimentRoute, ThroughputConfirmable, "Throughput Confirmable",
                 throughputRequestHandler);
#elif EXPERIMENT_THROUGHPUT_NONCONFIRMABLE
  createResource(experimentRoute, ThroughputNonConfirmable, "Throughput Non-Confirmable",
                 throughputRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_CONFIRMABLE
  createResource(experimentRoute, PacketLossConfirmable, "Packet Loss Confirmable",
                 defaultRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_NONCONFIRMABLE
  createResource(experimentRoute, PacketLossNonConfirmable, "Packet Loss Non-Confirmable",
                 defaultRequestHandler);
#endif
  return OT_ERROR_NONE;
}

otError expServerFree(void* aContext, uint8_t argsLength, char* aArgs[])
{
  resourceDestructor(experimentRoute);
  otCoapStop(OT_INSTANCE);
  return OT_ERROR_NONE;
}