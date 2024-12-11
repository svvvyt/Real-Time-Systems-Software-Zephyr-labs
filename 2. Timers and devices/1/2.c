#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

void delayed_work_handler(struct k_work *my_work)
{
  LOG_INF("Executing delayable work");
}

K_WORK_DELAYABLE_DEFINE(my_delayable_work, delayed_work_handler);

void main()
{
  LOG_INF("Plan delayable work 5s ahead");

  k_work_schedule(&my_delayable_work, K_MSEC(5000));

  while (1)
  {
    LOG_INF("Main work");
    k_sleep(K_SECONDS(2));
  }
}