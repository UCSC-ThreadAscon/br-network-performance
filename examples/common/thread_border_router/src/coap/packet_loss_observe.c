#include "workload.h"
#include "experiment_common.h"
#include "time_api.h"
#include "independent_variables.h"

Subscription subscription;
static uint32_t numReceived;

static bool trialFinished;

/**
 * I learned that doubles have 15 digits of precision from:
 * https://stackoverflow.com/a/2386882/6621292
 */
void printPacketLoss()
{
  uint32_t numPacketsLost = OBSERVE_MAX_PACKETS - numReceived;
  double packetLoss = ((double) numPacketsLost) / OBSERVE_MAX_PACKETS;

  PrintDelimiter();
  otLogNotePlat("Received: %" PRIu32 " packets", numReceived);
  otLogNotePlat("Packets Lost: %" PRIu32 " packets", numPacketsLost);
  otLogNotePlat("Expected: %d packets", OBSERVE_MAX_PACKETS);
  otLogNotePlat("Packet Loss Ratio: %.15f", packetLoss);
  PrintDelimiter();
  return;
}

void plObserveFinishTrial(void *aContext,
                               otMessage *aMessage,
                               const otMessageInfo *aMessageInfo,
                               otError aResult)
{
  if (aResult == OT_ERROR_NONE)
  {
    otLogNotePlat("Cancelled subscription 0x%llx.", subscription.token);
  }
  else
  {
    otLogNotePlat("Failed to cancel subscription 0x%llx. Reason: %s.",
                  subscription.token, otThreadErrorToString(aResult));
  }

  EmptyMemory(&subscription, sizeof(Subscription));
  startNextTrial();
  return;
}

void plObserveResponseCallback(void *aContext,
                               otMessage *aMessage,
                               const otMessageInfo *aMessageInfo,
                               otError aResult)
{
  if (aResult == OT_ERROR_NONE)
  {
    if (trialFinished) { trialFinishedHandler(aMessage, aMessageInfo); return; }

    otCoapType type = otCoapMessageGetType(aMessage);

    if (type == OT_COAP_TYPE_NON_CONFIRMABLE)
    {
      assert(getPayloadLength(aMessage) == sizeof(Fahrenheit));

      numReceived += 1;
      printObserveNotification(aMessage, &subscription, numReceived);
    }
    else // Either is initial ACK or final CON packet.
    {
      if (type == OT_COAP_TYPE_ACKNOWLEDGMENT)
      {
        PrintDelimiter();
        otLogNotePlat("Starting the Packet Loss Observe experiment trial!");
        PrintDelimiter();
      }
      else if (type == OT_COAP_TYPE_CONFIRMABLE)
      {
        printPacketLoss();
        trialFinished = true;
        observeRequest(&subscription, OBSERVE_SERVER_URI, plObserveFinishTrial,
                      OT_COAP_TYPE_CONFIRMABLE, OBSERVE_CANCEL);
      }
    }
  }
  else
  {
    PrintCritDelimiter();
    otLogCritPlat("CoAP Observe Packet Loss has failed. Reason: %s", otThreadErrorToString(aResult));
    otLogCritPlat("Going to restart the current experiment trial.");
    PrintCritDelimiter();

    esp_restart();
  }
  return;
}

void plObserveMain()
{
  resetTrials();
  coapStart();
  InitSockAddr(&(subscription.sockAddr), OBSERVE_SERVER_IP_ADDRESS);
  observeRequest(&subscription, OBSERVE_SERVER_URI, plObserveResponseCallback,
                 OT_COAP_TYPE_CONFIRMABLE, OBSERVE_SUBSCRIBE);
  return;
}

/**
 * The code for the Throughput Observe Server start callback function comes 
 * from the ESP-IDF OpenThread SED state change callback example function:
 * https://github.com/UCSC-ThreadAscon/esp-idf/blob/master/examples/openthread/ot_sleepy_device/deep_sleep/main/esp_ot_sleepy_device.c#L73
 */
void plObserveStartCallback(otChangedFlags changed_flags, void* ctx)
{
  OT_UNUSED_VARIABLE(ctx);
  static otDeviceRole s_previous_role = OT_DEVICE_ROLE_DISABLED;

  otInstance* instance = esp_openthread_get_instance();
  if (!instance) { return; }

  otDeviceRole role = otThreadGetDeviceRole(instance);
  if ((connected(role) == true) && (connected(s_previous_role) == false))
  {
    plObserveMain();

    printNetworkKey();
    PrintDelimiter();
    printCipherSuite();
    printTxPower();
    printNetPerfTest();
    PrintDelimiter();
  }
  s_previous_role = role;
  return;
}