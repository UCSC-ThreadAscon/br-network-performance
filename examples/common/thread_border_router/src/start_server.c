#include "workload.h"
#include "handler.h"
#include "independent_variables.h"

static otCoapResource experimentRoute;
static otSockAddr udpStartSockAddr;

void expStartUdpExperiment(otDeviceRole role)
{
  InitSockAddr(&udpStartSockAddr, CONFIG_FTD_IP_ADDRESS);
  bool udpSendFlag = true;

  PrintDelimiter();
  otLogNotePlat("Notifying FTD to start the Throughput UDP experiment trial.");
  PrintDelimiter();

  request(&udpStartSockAddr, &udpSendFlag, sizeof(bool), "throughput-udp",
          tpUdpStartServer, OT_COAP_TYPE_CONFIRMABLE);
  return;
}

void expStartCoapServer(void) 
{
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

  otInstance* instance = esp_openthread_get_instance();
  if (!instance)
  {
    return;
  }

  otDeviceRole role = otThreadGetDeviceRole(instance);
  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    coapStart();
    PrintDelimiter();

#if NO_EXPERIMENT
  otLogNotePlat("No experiments to set up.");
  otLogNotePlat("Edit the EXPERIMENT flag in `idf.py menuconfig` to choose which");
  otLogNotePlat("experiment the CoAP server will run.");
#elif (EXPERIMENT_THROUGHPUT_CONFIRMABLE || EXPERIMENT_PACKET_LOSS_CONFIRMABLE)
  expStartCoapServer();
#elif EXPERIMENT_THROUGHPUT_UDP
  expStartUdpExperiment(role);  
#endif

  printCipherSuite();
  printTxPower();
  PrintDelimiter();
  }
  s_previous_role = role;
  return;
}