#include <stdbool.h>
#include <stdint.h>

#include <type_traits>
#include <utility>

#include "atomic_32.hpp"
#include "simpletest.hpp"


DEFINE_TEST(Atomic32_Empty)
{
    nrf::atomic_32 a;
    TEST(a == 0);
    TEST(a.load() == 0);
}

DEFINE_TEST(Atomic32_StoreExchange)
{
    nrf::atomic_32 a(1);
    TEST(a == 1);
    TEST(a.load() == 1);

    TEST(a.store(2) == 2);
    TEST(a.exchange(3) == 2);

    TEST(a == 3);
    TEST(a.load() == 3);
}

DEFINE_TEST(Atomic32_Inc)
{
    nrf::atomic_32 a(1);

    TEST(a == 1);
    TEST(a++ == 1);
    TEST(a == 2);
    TEST(++a == 3);
    TEST(a == 3);
}

DEFINE_TEST(Atomic32_Dec)
{
    nrf::atomic_32 a(5);

    TEST(a == 5);
    TEST(a-- == 5);
    TEST(a == 4);
    TEST(--a == 3);
    TEST(a == 3);
}
