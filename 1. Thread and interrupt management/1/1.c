#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5
#define HIGHER_PRIORITY 3

K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);
struct k_thread thread_data;

void thread_func(void *arg1, void *arg2, void *arg3)
{
    printf("Thread is running.\n");
}

void main()
{
    // create thread
    k_tid_t my_tid = k_thread_create(&thread_data, thread_stack, STACK_SIZE, thread_func, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    printf("Created thread with number: %d.\n", THREAD_PRIORITY);

    // change thread priority
    k_thread_priority_set(my_tid, HIGHER_PRIORITY);
    printf("Thread priority changed to %d.\n", HIGHER_PRIORITY);

    return 0;
}
