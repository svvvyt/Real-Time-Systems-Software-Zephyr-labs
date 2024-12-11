#include <zephyr/kernel.h>

#define STACK_SIZE 1024

#define MAIN_THREAD_PRIORITY 5
#define CHILD_THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(main_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(child_thread_stack, STACK_SIZE);

struct k_thread main_thread_data;
struct k_thread child_thread_data;

volatile int counter = 0; // shared variable

void main_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        counter++;
        printf("Main thread, counter: %d,\n", counter);
        k_msleep(500);
    }
}

void child_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        counter++;
        printf("Child thread, counter: %d,\n", counter);
        k_msleep(1000);
    }
}

void main()
{
    // create main thread
    k_tid_t main_tid = k_thread_create(&main_thread_data, main_thread_stack, STACK_SIZE, main_thread, NULL, NULL, NULL, MAIN_THREAD_PRIORITY, 0, K_NO_WAIT);
    printf("Created main thread, priority: %d.\n", MAIN_THREAD_PRIORITY);

    // create child thread
    k_tid_t child_tid = k_thread_create(&child_thread_data, child_thread_stack, STACK_SIZE, child_thread, NULL, NULL, NULL, CHILD_THREAD_PRIORITY, 0, K_NO_WAIT);
    printf("Created child thread, priority: %d.\n", CHILD_THREAD_PRIORITY);

    // continuously working
    while (1)
    {
        k_msleep(1000);
    }
}
