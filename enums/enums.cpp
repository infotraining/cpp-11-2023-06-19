#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum DayOfWeek : uint8_t { mon = 1, tue, wed, thd, fri, sat, sun};

TEST_CASE("enum types in C++98")
{
    static_assert(sizeof(DayOfWeek) == 1);

    DayOfWeek today = mon;
    REQUIRE(today == 1);

    int value = today;
    ++value;
    REQUIRE(value == 2);

    DayOfWeek tomorrow = static_cast<DayOfWeek>(value);
    REQUIRE(tomorrow == tue);
}

enum class Color : uint32_t { red = 1, blue, white, black };

TEST_CASE("enum classes - since C++11")
{
    Color color = Color::blue;

    uint8_t value = static_cast<int>(color); // may be dangerous

    CHECK(value == 2);

    ++value;

    Color new_color = static_cast<Color>(value);

    CHECK(new_color == Color::white);

    std::underlying_type_t<Color> raw_value = 1; // C++14
}

TEST_CASE("std::byte")
{
    std::byte b1{42};
    b1 <<= 2;

    //b1 += 5; // ERROR

    std::cout << std::to_integer<int>(b1) << "\n"; 
}