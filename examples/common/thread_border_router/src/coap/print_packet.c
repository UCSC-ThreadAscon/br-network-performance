#include "workload.h"
#include "experiment_common.h"

#define CONFIRMABLE_STRING "Confirmable"
#define NONCONFIRMABLE_STRING "Non-Confirmable"
#define ACK_STRING "Acknowledgement"
#define RESET_STRING "Reset"

#define PrintMessage(coapTypeString, length, sender)          \
  otLogNotePlat("Received %" PRIu32 " bytes, %s, from %s.",   \
                length, coapTypeString, sender);              \

void getPeerAddrString(const otMessageInfo *aMessageInfo, char *ipString) {
  otIp6AddressToString(&(aMessageInfo->mPeerAddr), ipString,
                       OT_IP6_ADDRESS_STRING_SIZE);
  return;
}

void printMessage(otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  uint32_t length = getPayloadLength(aMessage);

  char sender[OT_IP6_ADDRESS_STRING_SIZE];
  EmptyMemory(sender, OT_IP6_ADDRESS_STRING_SIZE);
  getPeerAddrString(aMessageInfo, sender);

  switch (otCoapMessageGetType(aMessage))
  {
    case OT_COAP_TYPE_CONFIRMABLE:
      PrintMessage(CONFIRMABLE_STRING, length, sender);
      break;
    case OT_COAP_TYPE_NON_CONFIRMABLE:
      PrintMessage(NONCONFIRMABLE_STRING, length, sender);
      break;
    case OT_COAP_TYPE_ACKNOWLEDGMENT:
      PrintMessage(ACK_STRING, length, sender);
      break;
    case OT_COAP_TYPE_RESET:
      PrintMessage(RESET_STRING, length, sender);
      break;
    default:
      otLogCritPlat("The request has an invalid CoAP message type.");
  }
  return;
}

void printObserveNotification(otMessage *aMessage,
                              Subscription *subscription,
                              uint32_t packetsReceived)
{
  Fahrenheit temperature = 0;
  getPayload(aMessage, &temperature);

  switch (otCoapMessageGetType(aMessage))
  {
    case OT_COAP_TYPE_CONFIRMABLE:
      otLogNotePlat(
        "Subscription: 0x%llx, Temperature:%" PRIu8 "°F, Type: %s, Packet Num: "%PRIu32 "",
        subscription->token, temperature, "CON", packetsReceived
      );
      break;
    case OT_COAP_TYPE_NON_CONFIRMABLE:
      otLogNotePlat(
        "Subscription: 0x%llx, Temperature:%" PRIu8 "°F, Type: %s, Packet Num: "%PRIu32 "",
        subscription->token, temperature, "NON", packetsReceived
      );
      break;
    case OT_COAP_TYPE_ACKNOWLEDGMENT:
      otLogNotePlat(
        "Subscription: 0x%llx, Temperature:%" PRIu8 "°F, Type: %s, Packet Num: "%PRIu32 "",
        subscription->token, temperature, "ACK", packetsReceived
      );
      break;
    case OT_COAP_TYPE_RESET:
      otLogNotePlat(
        "Subscription: 0x%llx, Temperature:%" PRIu8 "°F, Type: %s, Packet Num: "%PRIu32 "",
        subscription->token, temperature, "RST", packetsReceived
      );
      break;
    default:
      otLogNotePlat(
        "Subscription: 0x%llx, Temperature:%" PRIu8 "°F, Type: %s, Packet Num: "%PRIu32 "",
        subscription->token, temperature, "N/A", packetsReceived
      );
  }
  return;
}