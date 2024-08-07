#pragma once

#include "print_statement.h"
#include "test.h"
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
#include "openthread/coap_secure.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define OT_INSTANCE esp_openthread_get_instance()

#define OT_DISCONNECTED(role) (role == OT_DEVICE_ROLE_DISABLED) || (role == OT_DEVICE_ROLE_DETACHED)

#define MS_TO_TICKS(ms) ms / portTICK_PERIOD_MS
#define MS_TO_MICRO(ms) ms * 1000

#define MAIN_WAIT_TIME MS_TO_TICKS(5000) // 5 seconds

void handleError(otError error, char* desc);

#define HandleMessageError(desc, aMessage, error)       \
  if (error != OT_ERROR_NONE) {                         \
    otMessageFree(aMessage);                            \
    handleError(error, desc);                           \
    return;                                             \
  }                                                     \

#define COAP_SOCK_PORT OT_DEFAULT_COAP_PORT

/** ---- CoAP CLI API ---- */
void expServerStartCallback(otChangedFlags changed_flags, void* ctx);

/** ---- CoAP Common API ---- */
uint16_t getPayloadLength(const otMessage *aMessage);
void getPayload(const otMessage *aMessage, void* buffer);

/* ---- CoAP Server API ---- */
otError createResource(otCoapResource *resource,
                       Test test,
                       const char *resourceName,
                       otCoapRequestHandler requestHandler);

void resourceDestructor(otCoapResource *resource);
void sendCoapResponse(otMessage *aRequest, const otMessageInfo *aRequestInfo);
void printRequest(otMessage *aMessage, const otMessageInfo *aMessageInfo);