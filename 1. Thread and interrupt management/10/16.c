#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512

#define PRIORITY_1 1
#define PRIORITY_2 2
#define PRIORITY_3 3

K_THREAD_STACK_DEFINE(thread_stack_1, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_2, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_3, STACK_SIZE);

struct k_thread thread_data_1;
struct k_thread thread_data_2;
struct k_thread thread_data_3;

void thread_entry(void *arg1, void *arg2, void *arg3)
{
  int thread_num = (int)arg1;

  LOG_INF("Thread %d started", thread_num);

  k_sleep(K_SECONDS(thread_num));

  LOG_INF("Thread %d finished", thread_num);
}

void main()
{
  LOG_INF("Program started");

  k_thread_create(&thread_data_1, thread_stack_1, STACK_SIZE, thread_entry, (void *)1, NULL, NULL, PRIORITY_1, 0, K_NO_WAIT);

  k_thread_create(&thread_data_2, thread_stack_2, STACK_SIZE, thread_entry, (void *)2, NULL, NULL, PRIORITY_2, 0, K_NO_WAIT);

  k_thread_create(&thread_data_3, thread_stack_3, STACK_SIZE, thread_entry, (void *)3, NULL, NULL, PRIORITY_3, 0, K_NO_WAIT);

  LOG_INF("All threads are created");
}