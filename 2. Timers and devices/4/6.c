#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define TIMER_INTERVAL_MS 1000

void on_timer_expired(struct k_timer *timer_id)
{
  LOG_INF("Timer worked: %lld ms\n", k_uptime_get());
}

void on_timer_stopped(struct k_timer *timer_id)
{
  LOG_INF("Timer stopped");
}

K_TIMER_DEFINE(my_timer, on_timer_expired, on_timer_stopped);

void main()
{
  LOG_INF("Starting timer\n");

  k_timer_start(&my_timer, K_MSEC(TIMER_INTERVAL_MS), K_MSEC(TIMER_INTERVAL_MS));

  k_sleep(K_SECONDS(10)); // wait for enough number of intervals

  k_timer_stop(&my_timer);
}