#pragma once

#include "print_statement.h"
#include "experiment.h"
#include "utilities.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_openthread.h"
#include "esp_openthread_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "openthread/cli.h"
#include "openthread/instance.h"
#include "openthread/logging.h"
#include "openthread/tasklet.h"
#include "openthread/coap.h"
#include "openthread/udp.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define OT_INSTANCE esp_openthread_get_instance()

#define MS_TO_TICKS(ms) ms / portTICK_PERIOD_MS
#define MS_TO_MICRO(ms) ms * 1000

#define MAIN_WAIT_TIME MS_TO_TICKS(5000) // 5 seconds

#define handleError(error, desc)                                        \
  if (error != OT_ERROR_NONE) {                                         \
    otLogCritPlat("%s error: %s", desc, otThreadErrorToString(error));  \
    return;                                                             \
  }                                                                     \

#define HandleMessageError(desc, aMessage, error)       \
  if (error != OT_ERROR_NONE) {                         \
    otMessageFree(aMessage);                            \
    handleError(error, desc);                           \
    return;                                             \
  }                                                     \

void resetTrials(void);
void startNextTrial(void);

#define COAP_SOCK_PORT OT_DEFAULT_COAP_PORT

/** ---- CoAP Common API ---- */
uint16_t getPayloadLength(const otMessage *aMessage);
void getPayload(const otMessage *aMessage, void* buffer);
void printMessage(otMessage *aMessage, const otMessageInfo *aMessageInfo);

/* ---- CoAP Server API ---- */
otError createResource(otCoapResource *resource,
                       Experiment experiment,
                       const char *resourceName,
                       otCoapRequestHandler requestHandler);

void resourceDestructor(otCoapResource *resource);
void sendCoapResponse(otMessage *aRequest, const otMessageInfo *aRequestInfo);

/* ---- CoAP Observe Client API ---- */

// https://datatracker.ietf.org/doc/html/rfc7641#section-2
#define OBSERVE_SUBSCRIBE 0
#define OBSERVE_CANCEL 1

typedef struct Subscription
{
  otSockAddr sockAddr;
  uint64_t token;
  uint8_t tokenLength;
}
Subscription;

void observeRequest(Subscription *subscription,
                    const char *uri,
                    otCoapResponseHandler responseCallback,
                    otCoapType type);