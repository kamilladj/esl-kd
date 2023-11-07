#include <stdbool.h>
#include <stdint.h>

#include <type_traits>
#include <utility>
#include <mutex>

#include "nrf_delay.h"
#include "nrf/singleshot_apptimer.hpp"
#include "utils/static_function.hpp"

#include "simpletest/simpletest.hpp"


DEFINE_TEST(AsyncTimer_Simple)
{
    static volatile bool called;
    called = false;

    nrf::singleshot_apptimer tt;
    tt.async_wait(
        300,
        [](error::error_status e)
        {
            if (!e)
                called = true;
        }
    );

    nrf_delay_ms(400);
    TEST(called);
}