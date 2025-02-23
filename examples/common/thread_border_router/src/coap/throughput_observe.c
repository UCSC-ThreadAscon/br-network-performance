#include "workload.h"
#include "experiment_common.h"
#include "time_api.h"
#include "independent_variables.h"

#define EXPECTED_TOTAL_BYTES (OBSERVE_MAX_PACKETS * OBSERVE_PAYLOAD_SIZE_BYTES)

static Subscription subscription;

static uint32_t totalBytes;
static uint32_t numReceived;

static struct timeval startTime;
static struct timeval endTime;

static bool trialFinished;

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

  startNextTrial();
  return;
}

void tpObserveResponseCallback(void *aContext,
                               otMessage *aMessage,
                               const otMessageInfo *aMessageInfo,
                               otError aResult)
{
  if (aResult != OT_ERROR_NONE)
  {
    PrintCritDelimiter();
    otLogCritPlat("CoAP Observe Throughput experiment has failed. Reason: %s", otThreadErrorToString(aResult));
    otLogCritPlat("Going to restart the current experiment trial.");
    PrintCritDelimiter();

    esp_restart();
  }
  else
  {
    if (trialFinished) { trialFinishedHandler(aMessage, aMessageInfo); return; }

    assertNotification(aMessage, &subscription);

    numReceived += 1;
    totalBytes += getPayloadLength(aMessage);
    assert(totalBytes <= EXPECTED_TOTAL_BYTES);

    printObserveNotification(aMessage, &subscription, numReceived);

    if (totalBytes == EXPECTED_TOTAL_BYTES)
    {
      /** The throughput formula is:
       *
       *       TOTAL PACKETS TO RECEIVE * PAYLOAD SIZE
       *      ----------------------------------------- bytes/time
       *                   t_end - t_start
       */
      endTime = getTimevalNow();

      uint64_t usElapsed = timeDiffUs(startTime, endTime);

      double denominatorUs = (double) usElapsed;
      double denominatorMs = US_TO_MS((double) denominatorUs);
      double denominatorSecs = US_TO_SECONDS((double) denominatorUs);

      double throughputSecs = (double) totalBytes / denominatorSecs;
      double throughputMs = (double) totalBytes / denominatorMs;
      double throughputUs = (double) totalBytes / denominatorUs;

      /**
       * I learned that doubles have 15 digits of precision from:
       * https://stackoverflow.com/a/2386882/6621292
       */
      PrintDelimiter();
      otLogNotePlat("The throughput is:");
      otLogNotePlat("%.15f bytes/second, or", throughputSecs);
      otLogNotePlat("%.15f bytes/ms, or", throughputMs);
      otLogNotePlat("%.15f bytes/us.", throughputUs);
      PrintDelimiter();

      PrintDelimiter();
      otLogNotePlat("Time Elapsed:");
      otLogNotePlat("%.15f seconds.", denominatorSecs);
      otLogNotePlat("%.15f ms, or", denominatorMs);
      otLogNotePlat("%" PRIu64 " us.", usElapsed);
      otLogNotePlat("Start Timestamp: %" PRIu64 "", toUs(startTime));
      otLogNotePlat("End Timestamp: %" PRIu64 "", toUs(endTime));
      PrintDelimiter();

      PrintDelimiter();
      otLogNotePlat("Total Received: %" PRIu32 " bytes", totalBytes);
      otLogNotePlat("Number of packets received: %" PRIu32 "", numReceived);
      PrintDelimiter();

      trialFinished = true;
      observeRequest(&subscription, OBSERVE_SERVER_URI, tpObserveCancelCallback,
                    OT_COAP_TYPE_CONFIRMABLE, OBSERVE_CANCEL);
    }
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

  startTime = getTimevalNow();
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
    setTxPower();
    tpObserveMain();

    printNetworkKey();
    PrintDelimiter();
    printCipherSuite();
    printTxPower();
    PrintDelimiter();
  }
  s_previous_role = role;
  return;
}