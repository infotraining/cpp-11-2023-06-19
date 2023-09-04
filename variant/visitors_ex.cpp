#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace std;

struct Circle
{
    int radius;
};

struct Rectangle
{
    int width, height;
};

struct Square
{
    int size;
};

TEST_CASE("visit a shape variant and calculate area")
{
    using Shape = variant<Circle, Rectangle, Square>;

    vector<Shape> shapes = {Circle{1}, Square{10}, Rectangle{10, 1}};

    double total_area{};

    // TODO

    //REQUIRE(total_area == Approx(113.14));
    REQUIRE_THAT(total_area, Catch::Matchers::WithinAbs(113.14, 0.01));
}