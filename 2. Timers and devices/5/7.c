#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

#define SLEEP_INTERVAL_MS 5000
#define ACTIVE_INTERVAL_MS 5000

#define BUTTON_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

K_SEM_DEFINE(button_sem, 0, 1);

bool is_device_active = true;

void activate_device()
{
    is_device_active = true;
    LOG_INF("Device activated.");
}

void deactivate_device()
{
    is_device_active = false;
    LOG_INF("Device deactivated.");
}

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    LOG_INF("Button pressed. Activating device...");
    k_sem_give(&button_sem);
}

void configure_button()
{
    if (!device_is_ready(button.port))
    {
        LOG_ERR("Button device is not ready.");
        return;
    }

    int ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0)
    {
        LOG_ERR("Failed to configure button pin: %d", ret);
        return;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0)
    {
        LOG_ERR("Failed to configure button interrupt: %d", ret);
        return;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);
    LOG_INF("Button configured.");
}

void main()
{
    LOG_INF("System started. Begin handling device.");

    configure_button();

    while (1)
    {
        if (is_device_active)
        {
            // Active regime
            LOG_INF("Device is active.");
            k_sleep(K_MSEC(ACTIVE_INTERVAL_MS));
            deactivate_device();
        }
        else
        {
            // Sleep regime
            LOG_INF("Device is sleeping.");
            k_sleep(K_MSEC(SLEEP_INTERVAL_MS));
            k_sem_take(&button_sem, K_FOREVER);
            activate_device();
        }
    }
}
