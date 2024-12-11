#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define MSGQ_SIZE 10         // message queue size
#define MSG_SIZE sizeof(int) // message size

K_MSGQ_DEFINE(timer_msgq, MSG_SIZE, MSGQ_SIZE, 4);

void timer_handler(struct k_timer *dummy)
{
  int msg = 1;

  if (k_msgq_put(&timer_msgq, &msg, K_NO_WAIT) != 0)
  {
    LOG_INF("Queue full, unable to put message");
  }
}

K_TIMER_DEFINE(my_timer, timer_handler, NULL);

void main()
{
  LOG_INF("Starting timer for generating messages");

  k_timer_start(&my_timer, K_MSEC(2000), K_MSEC(2000)); // timer repeats each 2s

  while (1)
  {
    int received_msg = 0;

    if (k_msgq_get(&timer_msgq, &received_msg, K_FOREVER) == 0)
    {
      uint32_t current_time_ms = k_uptime_get_32(); // get current system time in ms

      LOG_INF("Message received in %u time after system started", current_time_ms);
    }
  }
}