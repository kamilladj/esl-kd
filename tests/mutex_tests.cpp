#include <stdbool.h>
#include <stdint.h>

#include <type_traits>
#include <utility>
#include <mutex>

#include "mutex.hpp"
#include "simpletest.hpp"


DEFINE_TEST(Mutex_Simple)
{
    nrf::mutex m;
    TEST(m.try_lock());
    m.unlock();
}

DEFINE_TEST(Mutex_RecursiveLock)
{
    nrf::mutex m;
    TEST(m.try_lock());
    TEST(!m.try_lock());
    m.unlock();
}

DEFINE_TEST(Mutex_UniqueLock)
{
    nrf::mutex m;

    std::unique_lock<nrf::mutex> lock(m, std::try_to_lock);
    TEST(lock.owns_lock());
}

//DEFINE_TEST(Mutex_MoveConstructor)
//{
//    nrf::mutex m1;
//    TEST(m1.try_lock());
//
//    nrf::mutex m2(std::move(m1));
//    TEST(!m2.try_lock());
//
//    m2.unlock();
//}

//DEFINE_TEST(Mutex_MoveAssignment)
//{
//    nrf::mutex m1;
//    TEST(m1.try_lock());
//
//    nrf::mutex m2;
//    m2 = (std::move(m1));
//    TEST(!m2.try_lock());
//
//    m2.unlock();
//}
