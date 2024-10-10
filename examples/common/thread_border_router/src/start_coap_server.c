#include "workload.h"
#include "handler.h"
#include "independent_variables.h"

static otCoapResource experimentRoute;

static inline bool connected(otDeviceRole role)
{
  return (role == OT_DEVICE_ROLE_CHILD)  ||
         (role == OT_DEVICE_ROLE_ROUTER) ||
         (role == OT_DEVICE_ROLE_LEADER);
}

void startCoapServer(uint16_t port)
{
  otError error = otCoapStart(OT_INSTANCE, port);

  if (error != OT_ERROR_NONE) {
    otLogCritPlat("Failed to start COAP server.");
  } else {
    otLogNotePlat("Started CoAP server at port %d.", port);
  }
  return;
}

otError expServerStart(void) 
{
  PrintDelimiter();
  startCoapServer(OT_DEFAULT_COAP_PORT);

#if NO_EXPERIMENT
  otLogNotePlat("No experiments to set up.");
  otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
  otLogNotePlat("experiment the CoAP server will run.");
#elif EXPERIMENT_THROUGHPUT_CONFIRMABLE
  createResource(&experimentRoute, ThroughputConfirmable, "Throughput Confirmable",
                 throughputRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_CONFIRMABLE
  createResource(&experimentRoute, PacketLossConfirmable, "Packet Loss Confirmable",
                 packetLossRequestHandler);
#endif

  printCipherSuite();
  printTxPower();
  PrintDelimiter();
  return OT_ERROR_NONE;
}


/**
 * The code for the Experimental Setup server  start callback function comes from
 * the ESP-IDF OpenThread SED state change callback example function:
 * https://github.com/UCSC-ThreadAscon/esp-idf/blob/master/examples/openthread/ot_sleepy_device/deep_sleep/main/esp_ot_sleepy_device.c#L73
 */
void expServerStartCallback(otChangedFlags changed_flags, void* ctx)
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
    /** Start the CoAP server for the Throughput or Packet Loss experiment.
     */
    expServerStart();
  }
  s_previous_role = role;
  return;
}