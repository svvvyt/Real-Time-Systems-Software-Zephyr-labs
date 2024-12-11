#include <zephyr/kernel.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(main_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(child_thread_stack, STACK_SIZE);

struct k_thread main_thread_data;
struct k_thread child_thread_data;

volatile int counter = 0;

void main_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        counter++;
        printf("Main thread, counter: %d.\n", counter);
        k_msleep(10000000);
    }
}

void child_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        counter++;
        printf("Child thread, counter: %d.\n", counter);
        k_msleep(15000000);
    }
}

void main()
{
    // create main thread
    k_tid_t main_tid = k_thread_create(&main_thread_data, main_thread_stack, STACK_SIZE, main_thread, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    printf("Created main thread.\n");

    // create child thread
    k_tid_t child_tid = k_thread_create(&child_thread_data, child_thread_stack, STACK_SIZE, child_thread, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    printf("Created child thread.\n");

    char state_str[50];
    while (1)
    {
        k_thread_state_str(main_tid, state_str, sizeof(state_str));
        printf("Main thread state: %s.\n", state_str);

        k_thread_state_str(child_tid, state_str, sizeof(state_str));
        printf("Child thread state: %s.\n", state_str);

        k_msleep(100000000);
    }
}
