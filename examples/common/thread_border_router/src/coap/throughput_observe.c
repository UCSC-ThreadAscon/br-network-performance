#include "common.h"

static otSockAddr sockAddr;


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
    otLogNotePlat("TO-DO: Create the implementation for the Throughput Observe experiments.");
  }
  s_previous_role = role;
  return;
}