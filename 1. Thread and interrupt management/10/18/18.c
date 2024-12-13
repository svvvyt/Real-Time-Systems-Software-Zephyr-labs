#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys_clock.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define PRIORITY 1

K_THREAD_STACK_DEFINE(thread_stack_1, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_2, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_3, STACK_SIZE);

struct k_thread thread_data_1;
struct k_thread thread_data_2;
struct k_thread thread_data_3;

double cycles_to_seconds(uint32_t cycles)
{
  uint32_t cycles_per_second = sys_clock_hw_cycles_per_sec();
  return cycles / cycles_per_second; // int
}

void thread(void *arg1, void *arg2, void *arg3)
{
  int thread_num = (int)arg1;
  int counter = 0;

  LOG_INF("Thread %d started", thread_num);

  for (int i = 0; i < 5; i++)
  {
    LOG_INF("Thread: %d has just incremented counter: %d", thread_num, counter);
    counter++;
    // k_sleep(K_MSEC(1000)); // sleep for 1000
  }

  LOG_INF("Thread %d finished", thread_num);
}

void main()
{
  LOG_INF("Program started");

  k_tid_t tid1 = k_thread_create(&thread_data_1, thread_stack_1, STACK_SIZE,
                                 thread, (void *)1, NULL, NULL,
                                 PRIORITY, 0, K_NO_WAIT);

  k_tid_t tid2 = k_thread_create(&thread_data_2, thread_stack_2, STACK_SIZE,
                                 thread, (void *)2, NULL, NULL,
                                 PRIORITY, 0, K_NO_WAIT);

  k_tid_t tid3 = k_thread_create(&thread_data_3, thread_stack_3, STACK_SIZE,
                                 thread, (void *)3, NULL, NULL,
                                 PRIORITY, 0, K_NO_WAIT);

  k_thread_deadline_set(tid1, cycles_to_seconds(5000)); // CPU cycle units to seconds
  k_thread_deadline_set(tid2, cycles_to_seconds(100000));
  k_thread_deadline_set(tid3, cycles_to_seconds(15000000));

  LOG_INF("All threads are created with deadlines set");
}
