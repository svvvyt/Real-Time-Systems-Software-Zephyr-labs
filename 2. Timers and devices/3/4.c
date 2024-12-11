#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

typedef struct
{
  int data;
  uint32_t timeout;
} client_data_t;

#define MSGQ_SIZE 10
#define MSG_SIZE sizeof(client_data_t)

K_MSGQ_DEFINE(data_msgq, MSG_SIZE, MSGQ_SIZE, 4);

K_SEM_DEFINE(result_ready, 0, 1); // semaphore to inform client about end of handling

void process_data(int data)
{
  LOG_INF("Handling data: %d -> %d\n", data, data * 3); // tripling data number
}

void server(void *arg1, void *arg2, void *arg3)
{
  client_data_t received_data;

  while (1)
  {
    if (k_msgq_get(&data_msgq, &received_data, K_FOREVER) == 0)
    {
      process_data(received_data.data);

      k_sem_give(&result_ready); // notify client about end of handling
    }
  }
}

K_THREAD_DEFINE(server_tid, 1024, server, NULL, NULL, NULL, 7, 0, 0);

void client(void *arg1, void *arg2, void *arg3)
{
  client_data_t send_data;
  send_data.timeout = 1000;

  while (1)
  {
    send_data.data = k_uptime_get_32() % 100;

    if (k_msgq_put(&data_msgq, &send_data, K_NO_WAIT) != 0)
    {
      LOG_INF("Error sending data");
    }
    else
    {
      LOG_INF("Data sent: %d", send_data.data);

      if (k_sem_take(&result_ready, K_MSEC(send_data.timeout)) == 0)
      {
        LOG_INF("Client received notification about result ready");
      }
      else
      {
        LOG_INF("Client did not receive result (timeout)");
      }
    }

    k_sleep(K_SECONDS(2)); // pause before next send operation
  }
}

K_THREAD_DEFINE(client_tid, 1024, client, NULL, NULL, NULL, 7, 0, 0);

void main()
{
  LOG_INF("Starting client-server app");
}