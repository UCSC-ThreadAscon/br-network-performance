#include <openthread/thread_ftd.h>
#include <openthread/logging.h>

/**
 * Sets the Leader Weight to be the maximum on the server.
 */
static inline void SetMaxLeaderWeight()
{
  otThreadSetLocalLeaderWeight(OT_INSTANCE, UINT8_MAX);
  otLogNotePlat("Set leader weight to %d.", UINT8_MAX);
  return;
}

/**
 * Sets the Leader Weight to be the minimum on the client that the Border Router or
 * Delay server will always be the leader.
 */
static inline void SetMinLeaderWeight()
{
  otThreadSetLocalLeaderWeight(OT_INSTANCE, 0);
  otLogNotePlat("Set leader weight to %d.", 0);
  return;
}