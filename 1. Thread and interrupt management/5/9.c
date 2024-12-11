#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define THREAD_PRIORITY 1

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

void thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        LOG_INF("Thread with id %d running", (int)k_current_get());
        k_msleep(500);
    }
}

void main()
{
    // create 2 threads with lower priority
    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE, (k_thread_entry_t)thread, NULL, NULL, NULL, THREAD_PRIORITY + 1, 0, K_NO_WAIT);

    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE, (k_thread_entry_t)thread, NULL, NULL, NULL, THREAD_PRIORITY + 1, 0, K_NO_WAIT);

    while (1)
    {
        LOG_INF("Main thread running");

        // lock scheduler
        LOG_INF("Locking scheduler");
        k_sched_lock();

        // critical section
        for (int i = 0; i < 5; i++)
        {
            LOG_INF("Critical section iteration %d", i);
            k_busy_wait(100000);
        }

        // unlock scheduler
        LOG_INF("Unlocking scheduler");
        k_sched_unlock();

        k_msleep(1000);
    }
}
