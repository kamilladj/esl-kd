#include <stdbool.h>
#include <stdint.h>

#include <type_traits>
#include <utility>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "simpletest/simpletest.hpp"


void on_error()
{
    bsp_board_led_invert(1);
}

void on_success()
{
    bsp_board_led_invert(2);
}

void on_zero_tests()
{
    bsp_board_led_invert(3);
}


int main()
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    TestFixture::OnNoTestsFound = on_zero_tests;
    TestFixture::OnAllTestsPassed = on_success;
    TestFixture::OnSomeTestsFailed = on_error;

    TestFixture::ExecuteAllTests(TestFixture::Normal);

	return 0;
}

// fix - undefined reference to `operator delete(void*, unsigned int)'
// for utils::static_function
void operator delete(void*, unsigned int)
{}
