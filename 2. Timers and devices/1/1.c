#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

void on_timer_expired(struct k_timer *timer_id)
{
  LOG_INF("Timer has just expired");
}

K_TIMER_DEFINE(timer, on_timer_expired, NULL);

void main()
{
  LOG_INF("Starting kernel timer");

  k_timer_start(&timer, K_MSEC(1000), K_MSEC(1000)); // 1s-timer starts and repeats each 1s

  while (1)
  {
    k_sleep(K_SECONDS(10)); // let timer work
  }
}