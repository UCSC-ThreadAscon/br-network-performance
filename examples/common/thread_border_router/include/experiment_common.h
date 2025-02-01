#pragma once

#include "workload.h"
#include "time_api.h"
#include "experiment.h"

/**
 * I got the idea to use this specific print statement delimiter
 * from the ESP Thread Border Router SDK source code:
 * https://github.com/espressif/esp-thread-br/blob/main/components/esp_ot_br_server/src/esp_br_web.c#L1168
 */
#define PrintDelimiter() otLogNotePlat("<===========================================================>");
#define PrintCritDelimiter() otLogCritPlat("<===========================================================>");

static inline bool connected(otDeviceRole role)
{
  return (role == OT_DEVICE_ROLE_CHILD)  ||
         (role == OT_DEVICE_ROLE_ROUTER) ||
         (role == OT_DEVICE_ROLE_LEADER);
}

otSockAddr createSockAddr(const char *recvAddrString);

static inline void InitSockAddr(otSockAddr *sockAddr, const char* serverAddr)
{
  EmptyMemory(sockAddr, sizeof(otSockAddr));
  *sockAddr = createSockAddr(serverAddr);
  return;
}

void expServerStartCallback(otChangedFlags changed_flags, void* ctx);

void tpConRequestHandler(void* aContext,
                         otMessage *aMessage,
                         const otMessageInfo *aMessageInfo);

void plConRequestHandler(void* aContext,
                         otMessage *aMessage,
                         const otMessageInfo *aMessageInfo);

void coapStart();
void tpObserveStartCallback(otChangedFlags changed_flags, void* ctx);