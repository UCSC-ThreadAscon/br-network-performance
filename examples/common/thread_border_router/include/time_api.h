#pragma once

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_openthread.h"
#include "openthread/logging.h"
#include "openthread/thread.h"

#define US_TO_MS(us) us / 1000
#define MS_TO_US(ms) ms * 1000

#define MS_TO_SECONDS(ms) ms / 1000
#define MS_TO_MINUTES(ms) MS_TO_SECONDS(ms) / 60
#define US_TO_MINUTES(us) MS_TO_MINUTES(US_TO_MS(us))
#define US_TO_SECONDS(us) MS_TO_SECONDS(US_TO_MS(us))

#define SECONDS_TO_MS(secs) secs * 1000
#define SECONDS_TO_US(secs) MS_TO_US(SECONDS_TO_MS(secs))

#define MINUTES_TO_SECONDS(mins) mins * 60
#define SECONDS_TO_MINUTES(secs) secs / 60

uint64_t toUs(struct timeval time);
struct timeval getTimevalNow();
struct timeval getFutureTimeval(uint64_t seconds);
uint64_t timeDiffMs(struct timeval tv1, struct timeval tv2);
uint64_t timeDiffUs(struct timeval tv1, struct timeval tv2);
struct timeval randomTime(struct timeval tv1, struct timeval tv2);

int compareTimevals(const void* ptr1, const void* ptr2);