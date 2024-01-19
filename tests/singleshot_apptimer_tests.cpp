#include <stdbool.h>
#include <stdint.h>

#include <type_traits>
#include <utility>
#include <mutex>

#include "nrf_delay.h"
#include "singleshot_apptimer.hpp"
#include "static_function.hpp"

#include "simpletest.hpp"


DEFINE_TEST(AsyncTimer_Simple1)
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


DEFINE_TEST(AsyncTimer_Simple2)
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

    nrf_delay_ms(200);
    TEST(!called);
    nrf_delay_ms(200);
    TEST(called);
}


DEFINE_TEST(AsyncTimer_Simple3)
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

    tt.cancel();
    TEST(!called);
}


DEFINE_TEST(AsyncTimer_Simple4)
{
    static volatile bool called;

    nrf::singleshot_apptimer tt;
    for (int i = 0; i < 3; i++)
    {
        called = false;
        tt.async_wait(
            300,
            [](error::error_status e)
            {
                if (!e)
                    called = true;
            }
        );

        nrf_delay_ms(400);
    }

    TEST(called);
}


//DEFINE_TEST(AsyncTimer_Simple5)
//{
//    static volatile bool called;
//    called = false;
//
//    nrf::singleshot_apptimer tt;
//    for (int i = 0; i < 3; i++)
//    {
//        tt.async_wait(
//            300,
//            [](error::error_status e)
//            {
//                if (!e)
//                    called = true;
//            }
//        );
//    }
//
//    TEST(!called);
//}


DEFINE_TEST(AsyncTimer_Simple6)
{
    static volatile bool called;
    called = false;

    nrf::singleshot_apptimer tt;
    tt.cancel();
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