#ifndef NETWORK_TEST_HELPER_HPP
#define NETWORK_TEST_HELPER_HPP

#include <gtest/gtest.h>

class NetworkTests : public ::testing::Test
{
public:
    template <class Func, class... Args>
    static void func(Func f, Args... a);
    int increment_id_ = 0;
};

template <class Func, class... Args>
void NetworkTests::func(Func f, Args... a)
{
    f(a...);
}
#endif