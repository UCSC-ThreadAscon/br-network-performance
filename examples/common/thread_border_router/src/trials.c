#include "workload.h"
#include "nvs.h"

#define NVS_NAMESPACE "nvs_trials"
#define NVS_TRIALS "trials"
#define MAX_TRIALS 1

/**
 * This function should run BEFORE the experiment trial begins.
 * 
 * If the FTD has just powered on, then the number of trials in NVS will be reset to 0.
 * Otherwise (if there was a software reset), the number of trials will be unchanged.
 *
 * I learned that calling "nvs_open()" with the "NVS_READWRITE" flag creates a new
 * namespace (if the namespace does not exist) from:
 * https://www.esp32.com/viewtopic.php?t=32000#p109551
 *
 * I discovered the existence of the `esp_reset_reason()` function when reading:
 * https://forum.arduino.cc/t/how-to-distinguish-reset-by-power-on-from-reset-by-software-command/1160400/9
 */
void resetTrials(void)
{
  esp_reset_reason_t reason = esp_reset_reason();

  if (reason == ESP_RST_USB)
  {
    otLogNotePlat("%s %s", "The device has just powered on by a USB peripheral.",
                  "Going to reset the number of experiment trials to 0.");

    nvs_handle_t handle = 0;

    ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle));
    ESP_ERROR_CHECK(nvs_set_u32(handle, NVS_TRIALS, 0));

    nvs_close(handle);
  }

  return;
}

/**
 * This function should run AFTER each experiment trial is complete. The function
 * records the numbers of trials done so far, and restarts the device.
 *
 * I discovered the existence of the `esp_reset_reason()` function when reading:
 * https://forum.arduino.cc/t/how-to-distinguish-reset-by-power-on-from-reset-by-software-command/1160400/9
 *
 * I learned about the `esp_restart()` function from:
 * https://esp32.com/viewtopic.php?t=4706
 */
void startNextTrial(void)
{
  uint32_t numTrials = 0;
  nvs_handle_t handle = 0;

  ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle));
  ESP_ERROR_CHECK(nvs_get_u32(handle, NVS_TRIALS, &numTrials));

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
    otLogNotePlat("Finished running %" PRIu32 " trials for the current experiment.",
                  numTrials);
  }
  return;
}