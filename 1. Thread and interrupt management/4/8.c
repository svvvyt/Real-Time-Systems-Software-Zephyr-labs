#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define STACK_SIZE 512
#define THREAD_PRIORITY 5
#define TIME_SLICE_MS 100

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

void thread_func(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        LOG_INF("Thread with id %d running", (int)k_current_get());
        k_msleep(500);
    }
}

void main()
{
    int32_t slice_ticks = k_ms_to_ticks_ceil32(TIME_SLICE_MS);

    // set scheduler time slice
    k_sched_time_slice_set(slice_ticks, THREAD_PRIORITY);

    // create 2 threads
    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE, (k_thread_entry_t)thread_func, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE, (k_thread_entry_t)thread_func, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

    while (1)
    {
        // main thread
        LOG_INF("Main thread running");
        k_msleep(1000);
    }
}
