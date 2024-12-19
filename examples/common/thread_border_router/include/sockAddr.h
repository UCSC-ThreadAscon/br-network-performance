#pragma once

#include "workload.h"

#define FTD_IP_ADDRESS CONFIG_FTD_IP_ADDRESS

static inline void InitSockAddr(otSockAddr *sockAddr, const char* serverAddr)
{
  EmptyMemory(sockAddr, sizeof(otSockAddr));
  *sockAddr = createSockAddr(serverAddr);
  return;
}