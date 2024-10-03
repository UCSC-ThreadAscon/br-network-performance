/**
 * All of the code is both taken from and based upon the code
 * from the "System Time" page from the ESP-IDF documentation:
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/system_time.html
*/

#include "time_api.h"
#include "assert.h"

/** https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/system_time.html#get-current-time
 */
uint64_t toUs(struct timeval time)
{
  return (uint64_t)time.tv_sec * 1000000L + (uint64_t)time.tv_usec;
}

struct timeval getTimevalNow()
{
  struct timeval tvNow;
  gettimeofday(&tvNow, NULL);
  return tvNow;
}

/**
 * What will be the timestamp "[insert number here]" seconds from now?
*/
struct timeval getFutureTimeval(uint64_t seconds)
{
  struct timeval tvFuture;
  gettimeofday(&tvFuture, NULL);
  tvFuture.tv_sec += seconds;
  return tvFuture;
}

/**
 * The code to find the duration of time between two timevals comes from:
 * https://github.com/espressif/esp-idf/blob/master/examples/openthread/ot_sleepy_device/deep_sleep/main/esp_ot_sleepy_device.c#L117
 *
 * This function assumes that "tv2" occurs later than "tv1". The function
 * returns the time elapsed starting at "tv1" to "tv2" in milliseconds.
*/
uint64_t timeDiffMs(struct timeval tv1, struct timeval tv2)
{
  return (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000;
}

uint64_t timeDiffUs(struct timeval tv1, struct timeval tv2)
{
  return toUs(tv2) - toUs(tv1);
}

/**
 * Selects a random wakeup time that is after timeval "tv1", but
 * before the end of timeval "tv2".
*/
struct timeval randomTime(struct timeval tv1, struct timeval tv2)
{
  assert(toUs(tv1) < toUs(tv2));

  struct timeval random;
  random.tv_sec = 0;
  random.tv_usec = 0;

  do {
      time_t durationSec = tv2.tv_sec - tv1.tv_sec;
      time_t secondAfterNow = tv1.tv_sec + 1;

      random.tv_sec = (esp_random() % durationSec) + secondAfterNow;
      random.tv_usec = 0;
  } 
  while((toUs(random) <= toUs(tv1)) ||
        (toUs(random) >= toUs(tv2)));

  return random;
}

int compareTimevals(const void* ptr1, const void* ptr2) {
  struct timeval *timevalPtr1 = (struct timeval *) ptr1;
  struct timeval *timevalPtr2 = (struct timeval *) ptr2;

  struct timeval timeval1 = *timevalPtr1;
  struct timeval timeval2 = *timevalPtr2;

  uint64_t micro1 = toUs(timeval1);
  uint64_t micro2 = toUs(timeval2);

  if (micro1 < micro2) {
    return -1;
  }
  else if (micro1 == micro2) {
    return 0;
  }
  else {
    return 1;
  }
}