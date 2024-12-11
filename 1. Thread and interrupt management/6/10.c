#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define NUM_THREADS 3

K_THREAD_STACK_ARRAY_DEFINE(thread_stacks, NUM_THREADS, STACK_SIZE);

struct k_thread thread_data[NUM_THREADS];

volatile int counter = 0;

void thread(void *arg1, void *arg2, void *arg3)
{
    int thread_id = (int)arg1;

    while (counter < 20)
    {
        printf("Thread with id %d running\n", thread_id);
        k_busy_wait(500000);
        k_yield();
    }
}

void main()
{
    printf("Start time slice demo\n");

    // set time_slice to 200ms
    k_sched_time_slice_set(200, K_PRIO_PREEMPT(0));
    printf("Time slice is now 200ms\n");

    // create preemptive threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        k_thread_create(&thread_data[i], thread_stacks[i], STACK_SIZE, thread, (void *)i, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);
    }

    k_sleep(K_SECONDS(5)); // let threads work
    printf("End demo\n");
}
