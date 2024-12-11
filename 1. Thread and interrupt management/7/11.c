#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys_clock.h>

LOG_MODULE_REGISTER(cpu_idle_test, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define PRIORITY 1

K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);

struct k_thread thread_data;

struct k_timer my_timer;

void timer_handler(struct k_timer *dummy)
{
  LOG_INF("Timer worked and created interrupt");
}

void thread(void *arg1, void *arg2, void *arg3)
{
  LOG_INF("Thread initiates CPU sleep with k_cpu_idle");

  // enter CPU sleep mode, wait for interrupt
  k_cpu_idle();

  LOG_INF("CPU sleep is over. Interrupt frees CPU");
}

void main()
{
  LOG_INF("Start program");

  // create thread for CPU sleep
  k_thread_create(&thread_data, thread_stack, STACK_SIZE, thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

  k_timer_init(&my_timer, timer_handler, NULL);
  k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

  LOG_INF("Timer is running, interrupts will be working each second");
}