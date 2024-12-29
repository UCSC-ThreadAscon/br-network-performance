#include "workload.h"
#include "handler.h"
#include "independent_variables.h"

#include <openthread/thread_ftd.h>

static otCoapResource experimentRoute;

static otUdpSocket udpSocket;
static otSockAddr udpSockAddr;

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

void expStartUdpServer(otDeviceRole role)
{
  EmptyMemory(&udpSocket, sizeof(otUdpSocket));
  handleError(otUdpOpen(OT_INSTANCE, &udpSocket, tpUdpRequestHandler, NULL),
              "Failed to open UDP socket.");

  udpSockAddr.mAddress = *otThreadGetMeshLocalEid(OT_INSTANCE);
  udpSockAddr.mPort = UDP_SOCK_PORT;
  handleError(otUdpBind(OT_INSTANCE, &udpSocket, &udpSockAddr, OT_NETIF_THREAD),
              "Failed to set up UDP server.");
  
  otLogNotePlat("Created UDP server at port %d.", UDP_SOCK_PORT);
  return;
}

void expStartCoapServer(void) 
{
  startCoapServer(OT_DEFAULT_COAP_PORT);

#if EXPERIMENT_THROUGHPUT_CONFIRMABLE
  createResource(&experimentRoute, ThroughputConfirmable, "Throughput Confirmable",
                 tpConRequestHandler);
#elif EXPERIMENT_PACKET_LOSS_CONFIRMABLE
  createResource(&experimentRoute, PacketLossConfirmable, "Packet Loss Confirmable",
                 plConRequestHandler);
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

  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    printNetworkKey();
#if !EXPERIMENT_THROUGHPUT_UDP
    otError error = otThreadBecomeLeader(OT_INSTANCE);
    if (error == OT_ERROR_NONE)
    {
      otLogNotePlat("Successfully attached to the Thread Network as the leader.");
    }
    else
    {
      PrintCritDelimiter();
      otLogCritPlat("Failed to become the Leader of the Thread Network.");
      otLogCritPlat("Going to restart.");
      PrintCritDelimiter();

      esp_restart();
    }
#else
    if (role == OT_DEVICE_ROLE_LEADER)
    {
      PrintCritDelimiter();
      otLogCritPlat("Border Router failed to attach to the Thread network lead by the FTD.");
      otLogCritPlat("Going to restart the current experiment trial.");
      PrintCritDelimiter();

      esp_restart();
    }
#endif

    PrintDelimiter();
#if NO_EXPERIMENT
    otLogNotePlat("No experiments to set up.");
    otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
    otLogNotePlat("experiment the CoAP server will run.");
#elif (EXPERIMENT_THROUGHPUT_CONFIRMABLE || EXPERIMENT_PACKET_LOSS_CONFIRMABLE)
    expStartCoapServer();
#elif EXPERIMENT_THROUGHPUT_UDP
    expStartUdpServer(role);  
#endif
    printCipherSuite();
    printTxPower();
    PrintDelimiter();
  }
  s_previous_role = role;
  return;
}