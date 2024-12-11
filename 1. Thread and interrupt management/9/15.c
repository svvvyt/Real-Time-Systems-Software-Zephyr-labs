#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define MAX_MSG_SIZE 32
#define THREAD_PRIORITY 1

K_PIPE_DEFINE(my_pipe, MAX_MSG_SIZE, 4);

void communicator()
{
  char data[] = "test_msg";
  size_t bytes_written;

  while (1)
  {
    k_pipe_put(&my_pipe, data, sizeof(data), &bytes_written, 0, K_FOREVER);

    LOG_INF("Sent pipe data: %s", data);

    k_msleep(1000);
  }
}

void recipient()
{
  char buffer[MAX_MSG_SIZE];
  size_t bytes_read;

  while (1)
  {
    k_pipe_get(&my_pipe, buffer, sizeof(buffer), &bytes_read, 0, K_FOREVER);

    if (bytes_read > 0)
    {
      LOG_INF("Received pipe data: %s", buffer);
    }

    k_msleep(500);
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