#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/irq.h>

LOG_MODULE_REGISTER(interrupt_test, LOG_LEVEL_INF);

void handle_interrupt_control()
{
  LOG_INF("Lock all interrupts");
  unsigned int key = irq_lock();
  LOG_INF("All interrupts are now locked, handling critical section");

  // critical section
  k_busy_wait(500000); // 5s

  LOG_INF("Critical section over, unlocking interrupts");
  irq_unlock(key);
  LOG_INF("Interrupts are now unlocked");
}

void main()
{
  LOG_INF("Start program");

  handle_interrupt_control();

  LOG_INF("End program");
}