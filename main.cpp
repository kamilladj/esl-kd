#include "boards.h"

#include "smart_button.hpp"
#include "blink_event_pwm_manager.hpp"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define BUTTON NRF_GPIO_PIN_MAP(1,6)


void logs_init()
{
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);
    logs_init();

    const unsigned int device_id = 4321;

    nrf::blink_event_pwm_manager blink_manager(device_id);

    nrf::smart_button<BUTTON> a([&blink_manager](nrf::button_events evt) { blink_manager.enable(evt); });

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
    }
}


void operator delete(void*, unsigned int)
{}
