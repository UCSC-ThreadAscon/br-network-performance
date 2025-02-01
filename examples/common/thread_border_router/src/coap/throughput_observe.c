#include "common.h"

static otSockAddr sockAddr;

/**
 * TODO: Follow the `ProcessRequest()` example to figure out how to SET UP
 *       a CoAP observe subscription:
 *       https://github.com/openthread/openthread/blob/main/src/cli/cli_coap.cpp#L571
 *
 * TODO: Try out the OpenThread default request handler to see if you can capture
 *       CoAP observe notifications.
 *
 * TODO: Get the Border Router to unsubscribe and calculate the throughput after
 *       receiving 1000 Non-Confirmable packets.
 */
void tpObserveMain()
{
  resetTrials();
  coapStart();
  InitSockAddr(&sockAddr, OBSERVE_SERVER_IP_ADDRESS);

  PrintDelimiter();
  otLogNotePlat("Starting the Throughput Observe experiment trial!");
  PrintDelimiter();

  return;
}

/**
 * The code for the Throughput Observe Server start callback function comes 
 * from the ESP-IDF OpenThread SED state change callback example function:
 * https://github.com/UCSC-ThreadAscon/esp-idf/blob/master/examples/openthread/ot_sleepy_device/deep_sleep/main/esp_ot_sleepy_device.c#L73
 */
void tpObserveStartCallback(otChangedFlags changed_flags, void* ctx)
{
  OT_UNUSED_VARIABLE(ctx);
  static otDeviceRole s_previous_role = OT_DEVICE_ROLE_DISABLED;

  otInstance* instance = esp_openthread_get_instance();
  if (!instance) { return; }

  otDeviceRole role = otThreadGetDeviceRole(instance);
  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    tpObserveMain();
  }
  s_previous_role = role;
  return;
}