#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define NUM_THREADS 3

K_THREAD_STACK_ARRAY_DEFINE(thread_stacks, NUM_THREADS, STACK_SIZE);
struct k_thread thread_data[NUM_THREADS];

volatile int counter = 0;

void thread(void *arg1, void *arg2, void *arg3)
{
    int thread_id = (int)arg1;
    int count = 0;

    while (count < 5) // each thread runs 5 times
    {
        counter++;
        printf("Thread %d: counter = %d.\n", thread_id, counter);
        count++;
        k_sleep(K_SECONDS(1)); // lets other threads run
    }
}

void main()
{
    printk("Start thread scheduling test...\n");

    // create threads with same priority
    for (int i = 0; i < NUM_THREADS; i++)
    {
        k_thread_create(
            &thread_data[i],
            thread_stacks[i],
            STACK_SIZE,
            thread,
            (void *)i, NULL, NULL,
            K_PRIO_COOP(1), 0, K_NO_WAIT);
    }
}
