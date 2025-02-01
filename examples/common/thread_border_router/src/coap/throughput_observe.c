#include "workload.h"
#include "experiment_common.h"

static Subscription subscription;

void tpObserveResponseCallback(void *aContext,
                               otMessage *aMessage,
                               const otMessageInfo *aMessageInfo,
                               otError aResult)
{
  assertNotification(aMessage, &subscription);
  return;
}

/**
 * TODO: Get the Border Router to unsubscribe and calculate the throughput after
 *       receiving 1000 Non-Confirmable packets.
 */
void tpObserveMain()
{
  resetTrials();
  coapStart();
  InitSockAddr(&(subscription.sockAddr), OBSERVE_SERVER_IP_ADDRESS);

  PrintDelimiter();
  otLogNotePlat("Starting the Throughput Observe experiment trial!");
  PrintDelimiter();

  observeRequest(&subscription, OBSERVE_SERVER_URI, tpObserveResponseCallback,
                 OT_COAP_TYPE_CONFIRMABLE);
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