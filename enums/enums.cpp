#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum DayOfWeek { mon = 1, tue, wed, thd, fri, sat, sun};

TEST_CASE("enum types in C++98")
{
    DayOfWeek today = mon;
    REQUIRE(today == 1);

    int value = today;
    ++value;
    REQUIRE(value == 2);

    DayOfWeek tomorrow = static_cast<DayOfWeek>(value);
    REQUIRE(tomorrow == tue);
}