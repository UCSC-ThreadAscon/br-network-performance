#include "workload.h"
#include "experiment_common.h"
#include "time_api.h"

#define TOTAL_PACKETS_TO_RECEIVE 10
#define PAYLOAD_SIZE sizeof(Fahrenheit)
#define EXPECTED_TOTAL_BYTES (TOTAL_PACKETS_TO_RECEIVE * PAYLOAD_SIZE)

static Subscription subscription;

static uint32_t totalBytes;

void tpObserveCancelCallback(void *aContext,
                             otMessage *aMessage,
                             const otMessageInfo *aMessageInfo,
                             otError aResult)
{
  uint64_t token = 0;
  memcpy(&token, otCoapMessageGetToken(aMessage), otCoapMessageGetTokenLength(aMessage)); 
  assert(token == subscription.token);

  otLogNotePlat("Cancelled subscription 0x%llx.", subscription.token);
  EmptyMemory(&subscription, sizeof(Subscription));
  return;
}

/**
 * TODO: Get the Border Router to unsubscribe and calculate the throughput after
 *       receiving 10 Non-Confirmable packets.
 */
void tpObserveResponseCallback(void *aContext,
                               otMessage *aMessage,
                               const otMessageInfo *aMessageInfo,
                               otError aResult)
{
  assertNotification(aMessage, &subscription);
  printObserveNotification(aMessage, &subscription);

  totalBytes += getPayloadLength(aMessage);
  assert(totalBytes <= EXPECTED_TOTAL_BYTES);

  if (totalBytes == EXPECTED_TOTAL_BYTES)
  {
    /** The throughput formula is:
     *
     *       TOTAL PACKETS TO RECEIVE * PAYLOAD SIZE
     *      ----------------------------------------- bytes/time
     *                   t_end - t_start
     */
    otLogNotePlat("Received a total of %" PRIu32 " bytes.", totalBytes);

    observeRequest(&subscription, OBSERVE_SERVER_URI, tpObserveCancelCallback,
                  OT_COAP_TYPE_CONFIRMABLE, OBSERVE_CANCEL);
  }

  return;
}

void tpObserveMain()
{
  resetTrials();
  coapStart();
  InitSockAddr(&(subscription.sockAddr), OBSERVE_SERVER_IP_ADDRESS);

  PrintDelimiter();
  otLogNotePlat("Starting the Throughput Observe experiment trial!");
  PrintDelimiter();

  observeRequest(&subscription, OBSERVE_SERVER_URI, tpObserveResponseCallback,
                 OT_COAP_TYPE_CONFIRMABLE, OBSERVE_SUBSCRIBE);
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