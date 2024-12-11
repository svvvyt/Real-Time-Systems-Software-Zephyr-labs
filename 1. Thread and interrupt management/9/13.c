#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define QUEUE_SIZE 10
#define THREAD_PRIORITY 1

struct message
{
  int data;
};

K_QUEUE_DEFINE(my_queue);

void communicator()
{
  struct message msg;

  for (int i = 0; i < 5; i++)
  {
    msg.data = i;
    k_queue_append(&my_queue, &msg);

    LOG_INF("Sent msg: %d", msg.data);

    k_msleep(10000);
  }
}

void recipient()
{
  while (1)
  {
    struct message *msg = k_queue_get(&my_queue, K_FOREVER);

    if (msg != NULL)
    {
      LOG_INF("Received msg: %d", msg->data);
    }

    k_msleep(5000);
  }
}

K_THREAD_STACK_DEFINE(communicator_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(recipient_stack, STACK_SIZE);

struct k_thread communicator_data;
struct k_thread recipient_data;

void main()
{
  // create communicator thread
  k_thread_create(&communicator_data, communicator_stack, STACK_SIZE, (k_thread_entry_t)communicator, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

  // create recipient thread
  k_thread_create(&recipient_data, recipient_stack, STACK_SIZE, (k_thread_entry_t)recipient, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
}