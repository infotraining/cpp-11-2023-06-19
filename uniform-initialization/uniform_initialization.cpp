#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Data
{
    char c;
    double d;
    int data[5];
};

class Point
{
    int x_, y_;

public:
    Point(int x, int y) : x_(x), y_(y)
    {}

    int x() const
    {
        return x_;
    }

    int y() const
    {
        return y_;
    }
};

TEST_CASE("initialization syntax in C++98")
{
    SECTION("simple types")
    {
        int x = 10;
        int y(10);
        REQUIRE(x == y);

        //int z();
        //REQUIRE(z == 0);
    }

    SECTION("aggregates - c-like structures")
    {
        Data d1{ 'a', 3.14, {1, 2, 3} };
        Data d2{ 'a' };
        Data d3{};
    }

    SECTION("native arrays")
    {
        int tab1[4] = { 1, 2, 3, 4 };
        int tab2[4] = { 1, 2 };
        int tab3[4] = {};
    }

    SECTION("classes")
    {
        Point pt1(1, 2);

        REQUIRE(pt1.x() == 1);
        REQUIRE(pt1.y() == 2);
    }

    SECTION("containers")
    {
        std::vector<int> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
    }
}