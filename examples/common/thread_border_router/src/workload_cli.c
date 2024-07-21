#include "workload.h"
#include "handler.h"
#include "independent_variables.h"

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
  PrintDelimiter();
  checkConnection(OT_INSTANCE);
  startCoapServer(OT_DEFAULT_COAP_PORT);

  experimentRoute = calloc(1, sizeof(otCoapResource));

#if NO_EXPERIMENT
  otLogNotePlat("No experiments to set up.");
  otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
  otLogNotePlat("experiment the CoAP server will run.");
#elif EXPERIMENT_THROUGHPUT_CONFIRMABLE
  createResource(experimentRoute, ThroughputConfirmable, "Throughput Confirmable",
                 throughputRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_CONFIRMABLE
  createResource(experimentRoute, PacketLossConfirmable, "Packet Loss Confirmable",
                 packetLossRequestHandler);
#endif

  printCipherSuite();
  printTxPower();
  PrintDelimiter();
  return OT_ERROR_NONE;
}

otError expServerFree(void* aContext, uint8_t argsLength, char* aArgs[])
{
  resourceDestructor(experimentRoute);
  otCoapStop(OT_INSTANCE);
  return OT_ERROR_NONE;
}