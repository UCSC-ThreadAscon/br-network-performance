#include "trials.h"

#define NVS_NAMESPACE "nvs_trials"
#define NVS_TRIALS "trials"
#define MAX_TRIALS 1000

/**
 * This function should run AFTER each experimental trial is complete. The function
 * records the numbers of trials done so far, and restarts the device.
 *
 * I discovered the existence of the `esp_reset_reason()` function when reading:
 * https://forum.arduino.cc/t/how-to-distinguish-reset-by-power-on-from-reset-by-software-command/1160400/9
 *
 * I learned how abotu `esp_restart()` from:
 * https://esp32.com/viewtopic.php?t=4706
 */
void startNextTrial(void)
{
  uint32_t numTrials = 0;
  nvs_handle_t handle = 0;
  esp_reset_reason_t reason = esp_reset_reason();

  ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle));

  if (reason == ESP_RST_SW)
  {
    ESP_ERROR_CHECK(nvs_get_u32(handle, NVS_TRIALS, &numTrials));
  }

  /** Add in the trial that has just been completed, and write the
   *  updated number of trials to local storage.
   */
  numTrials += 1;
  ESP_ERROR_CHECK(nvs_set_u32(handle, NVS_TRIALS, numTrials));

  nvs_close(handle);

  if (numTrials < MAX_TRIALS)
  {
    otLogNotePlat("Trial %" PRIu32 " is now complete.", numTrials);
    esp_restart();
  }
  else
  {
    otLogNotePlat("Finished running %" PRIu32 " trials for current experiment.",
                  numTrials);
  }
  return;
}