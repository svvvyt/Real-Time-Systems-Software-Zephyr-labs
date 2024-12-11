#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define MSGQ_MAX_MSGS 4
#define THREAD_PRIORITY 1

struct msg
{
  int data;
};

K_MSGQ_DEFINE(mailbox, sizeof(struct msg), MSGQ_MAX_MSGS, 4);

void communicator(void)
{
  struct msg message;

  for (int i = 0; i < 5; i++)
  {
    message.data = i;
    k_msgq_put(&mailbox, &message, K_FOREVER);

    LOG_INF("Sent message to mailbox: %d", message.data);

    k_msleep(1000);
  }
}

void recipient(void)
{
  struct msg message;

  while (1)
  {
    k_msgq_get(&mailbox, &message, K_FOREVER);

    LOG_INF("Received message from mailbox: %d", message.data);

    k_msleep(500);
  }
}

K_THREAD_STACK_DEFINE(communicator_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(recipient_stack, STACK_SIZE);

struct k_thread communicator_data;
struct k_thread recipient_data;

void main(void)
{
  // Create communicator thread
  k_thread_create(&communicator_data, communicator_stack, STACK_SIZE, communicator, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

  // Create recipient thread
  k_thread_create(&recipient_data, recipient_stack, STACK_SIZE, recipient, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
}
