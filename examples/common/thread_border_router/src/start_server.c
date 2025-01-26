#include "workload.h"
#include "handler.h"
#include "independent_variables.h"

#include <openthread/thread_ftd.h>

static otCoapResource experimentRoute;

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

void expStartCoapServer(void) 
{
  startCoapServer(OT_DEFAULT_COAP_PORT);

#if EXPERIMENT_THROUGHPUT_CONFIRMABLE
  createResource(&experimentRoute, ThroughputConfirmable, "Throughput Confirmable",
                 tpConRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_OBSERVE
  createResource(&experimentRoute, PacketLossConfirmable, "Packet Loss Confirmable",
                 plConRequestHandler);
#elif EXPERIMENT_THROUGHPUT_OBSERVE
  otLogNotePlat("TO-DO: Create the implementation for the Throughput Observe experiments.");
#else
  OT_UNUSED_VARIABLE(experimentRoute);
#endif
  return;
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

  if (!OT_INSTANCE) { return; }
  otDeviceRole role = otThreadGetDeviceRole(OT_INSTANCE);

  SET_MAX_LEADER_WEIGHT();

  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    printNetworkKey();

    otError error = otThreadBecomeLeader(OT_INSTANCE);
    if (error == OT_ERROR_NONE)
    {
      otLogNotePlat("Successfully attached to the Thread Network as the leader.");
    }
    else
    {
      PrintCritDelimiter();
      otLogCritPlat("Failed to become the Leader of the Thread Network.");
      otLogCritPlat("Reason: %s", otThreadErrorToString(error));
      otLogCritPlat("Going to restart.");
      PrintCritDelimiter();

      esp_restart();
    }

    PrintDelimiter();
#if NO_EXPERIMENT
    otLogNotePlat("No experiments to set up.");
    otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
    otLogNotePlat("experiment the CoAP server will run.");
#else
    expStartCoapServer();
#endif
    printCipherSuite();
    printTxPower();
    PrintDelimiter();
  }
  s_previous_role = role;
  return;
}