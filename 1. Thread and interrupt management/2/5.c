#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

void thread1(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        printf("Thread 1 is working...\n");
        k_yield();             // free CPU
        k_sleep(K_SECONDS(1)); // sleep mode
    }
}

void thread2(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        printf("Thread 2 is working...\n");
        k_yield();             // free CPU
        k_sleep(K_SECONDS(2)); // sleep mode
    }
}

void main()
{
    printf("Running cooperative threads\n");

    // create cooperative threads
    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE, thread1, NULL, NULL, NULL, K_PRIO_COOP(1), 0, K_NO_WAIT);
    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE, thread2, NULL, NULL, NULL, K_PRIO_COOP(1), 0, K_NO_WAIT);
}
