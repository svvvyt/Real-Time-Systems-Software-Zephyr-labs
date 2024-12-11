#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define MONITOR_INTERVAL_SEC 3
#define FAILURE_THRESHOLD 3

static int failure_count = 0;
static bool device_disconnected = false;

bool is_device_operational(void)
{
  return rand() % 2 == 0;
}

void log_event(const char *message)
{
  LOG_INF("[Event Log]: %s", message);
}

void monitor_device(void)
{
  while (1)
  {
    if (is_device_operational())
    {
      if (device_disconnected)
      {
        /* Если устройство восстановлено */
        LOG_INF("Device reconnected and operational.");
        log_event("Device reconnected.");
        device_disconnected = false;
      }
      failure_count = 0; // Сброс счётчика сбоев
      LOG_INF("Device is functioning normally.");
    }
    else
    {
      failure_count++;
      LOG_ERR("Device failure detected! Consecutive failures: %d", failure_count);
      log_event("Device failure detected.");

      if (failure_count >= FAILURE_THRESHOLD && !device_disconnected)
      {
        LOG_ERR("Device is considered disconnected after %d failures.", FAILURE_THRESHOLD);
        log_event("Device disconnected.");
        device_disconnected = true;
      }
    }
    k_sleep(K_SECONDS(MONITOR_INTERVAL_SEC));
  }
}

void main(void)
{
  LOG_INF("Starting device monitor example.");
  log_event("Device monitoring started.");
  monitor_device();
}
