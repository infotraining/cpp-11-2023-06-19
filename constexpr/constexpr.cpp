#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace Cpp11
{
    constexpr uintmax_t factorial(uintmax_t n)
    {
        return (n == 1) ? 1 : factorial(n - 1) * n;
    }
}

constexpr uintmax_t factorial(uintmax_t n)
{
    if (n < 1)
        return 1;
    else
        return factorial(n - 1) * n;
}

TEST_CASE("constexpr")
{
    constexpr int N = 42;
    const int n = 42;

    int x = 10;
    constexpr int size = N * n;

    constexpr auto fact_5 = factorial(5);
    static_assert(fact_5 == 120);

    int buffer[factorial(6)];

    int value = 10;
    std::cout << factorial(10) << std::endl;
}

constexpr int check(int i)
{
    int low = 0;
    int high = 99;

    if (low <= i && i < high)
        return i;
    else
        throw std::out_of_range("range error");
}

TEST_CASE("constexpr + throw")
{
    constexpr int size = check(50);
    try
    {
        int size = check(150);
    }
    catch (const std::out_of_range& e)
    {
        std::cout << e.what() << std::endl;
    }
}

template <size_t N>
constexpr auto create_factorial_lookup_table()
{
    std::array<uintmax_t, N> lookup_table{};

    for (size_t i = 0; i < N; ++i)
    {
        lookup_table[i] = factorial(i);
    }

    return lookup_table;
}

template <size_t N, typename F>
constexpr auto create_lookup_table(F f)
{
    std::array<uintmax_t, N> lookup_table{};

    for (size_t i = 0; i < N; ++i)
    {
        lookup_table[i] = f(i);
    }

    return lookup_table;
}

TEST_CASE("lookup table")
{
    constexpr auto factorials = create_factorial_lookup_table<10>();

    constexpr auto squares = create_lookup_table<100>([](int x)
        { return x * x; });
}