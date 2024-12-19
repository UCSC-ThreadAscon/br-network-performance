#pragma once

#include "workload.h"

#define DELAY_SERVER_IP CONFIG_DELAY_SERVER_IP_ADDRESS
#define SERVER_IP CONFIG_SERVER_IP_ADDRESS

static inline void InitSockAddr(otSockAddr *sockAddr, const char* serverAddr)
{
  EmptyMemory(sockAddr, sizeof(otSockAddr));
  *sockAddr = createSockAddr(serverAddr);
  return;
}