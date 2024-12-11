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

K_THREAD_STACK_DEFINE(server_stack_area, 1024);

struct k_thread server_thread_data;
k_tid_t server_tid;

void client(void *arg1, void *arg2, void *arg3)
{
  uint32_t client_id = (uint32_t)arg1;
  uint32_t data_gen_interval = (uint32_t)arg2;
  uint32_t timeout = (uint32_t)arg3;
  client_data_t send_data;

  while (1)
  {
    send_data.data = k_uptime_get_32() % 100 + client_id * 100;
    send_data.timeout = timeout;

    if (k_msgq_put(&data_msgq, &send_data, K_NO_WAIT) != 0)
    {
      LOG_INF("Client %d: error sending data\n", client_id);
    }
    else
    {
      LOG_INF("Client %d: data sent %d\n", client_id, send_data.data);

      if (k_sem_take(&result_ready, K_MSEC(send_data.timeout)) == 0)
      {
        LOG_INF("Client %d: result ready\n", client_id);
      }
      else
      {
        LOG_INF("Client %d did not receive result (timeout)", client_id);
      }
    }

    k_sleep(K_MSEC(data_gen_interval)); // pause before next send operation
  }
}

#define CLIENT_STACK_SIZE 1024
#define NUM_CLIENTS 3

K_THREAD_STACK_DEFINE(client_stack_1, CLIENT_STACK_SIZE);
K_THREAD_STACK_DEFINE(client_stack_2, CLIENT_STACK_SIZE);
K_THREAD_STACK_DEFINE(client_stack_3, CLIENT_STACK_SIZE);

struct k_thread client_thread_data_1, client_thread_data_2, client_thread_data_3;
k_tid_t client_tid_1, client_tid_2, client_tid_3;

const uint32_t client_params[NUM_CLIENTS][2] = {
    {1, 4000}, // client 1, interval and timeout
    {2, 6000},
    {3, 5000},
};

void main()
{
  LOG_INF("Starting client-server app with multiple clients\n");

  // server thread
  server_tid = k_thread_create(&server_thread_data, server_stack_area, K_THREAD_STACK_SIZEOF(server_stack_area), server, NULL, NULL, NULL, 7, 0, K_NO_WAIT);

  // clients threads
  client_tid_1 = k_thread_create(&client_thread_data_1, client_stack_1, K_THREAD_STACK_SIZEOF(client_stack_1), client, (void *)0, (void *)client_params[0][0], (void *)client_params[0][1], 7, 0, K_NO_WAIT);
  client_tid_2 = k_thread_create(&client_thread_data_2, client_stack_2, K_THREAD_STACK_SIZEOF(client_stack_2), client, (void *)1, (void *)client_params[1][0], (void *)client_params[1][1], 7, 0, K_NO_WAIT);
  client_tid_3 = k_thread_create(&client_thread_data_3, client_stack_3, K_THREAD_STACK_SIZEOF(client_stack_3), client, (void *)2, (void *)client_params[2][0], (void *)client_params[2][1], 7, 0, K_NO_WAIT);
}