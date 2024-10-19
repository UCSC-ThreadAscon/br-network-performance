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

uint64_t timeDiffUs(struct timeval tv1, struct timeval tv2)
{
  return toUs(tv2) - toUs(tv1);
}