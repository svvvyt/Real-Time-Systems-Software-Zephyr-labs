#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define HIGHER_PRIORITY 1
#define LOWER_PRIORITY 2

K_THREAD_STACK_DEFINE(main_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(child_thread_stack, STACK_SIZE);

struct k_thread main_thread_data;
struct k_thread child_thread_data;

volatile int counter = 0;

void main_thread(void *arg1, void *arg2, void *arg3)
{
    while (true)
    {
        counter++;
        printf("Main thread, counter: %d.\n", counter);
        k_msleep(10000000);
    }
}

void child_thread(void *arg1, void *arg2, void *arg3)
{
    while (true)
    {
        counter++;
        printf("Child thread, counter: %d.\n", counter);
        k_msleep(100000);
    }
}

void main()
{
    // create main thread
    k_tid_t main_tid = k_thread_create(&main_thread_data, main_thread_stack, STACK_SIZE, main_thread, NULL, NULL, NULL, HIGHER_PRIORITY, 0, K_NO_WAIT);
    printf("Created main thread, priority: %d.\n", HIGHER_PRIORITY);

    // create child thread
    k_tid_t child_tid = k_thread_create(&child_thread_data, child_thread_stack, STACK_SIZE, child_thread, NULL, NULL, NULL, LOWER_PRIORITY, 0, K_NO_WAIT);
    printf("Created child thread, priority: %d.\n", LOWER_PRIORITY);

    while (true)
    {
        k_msleep(1000);
    }
}
