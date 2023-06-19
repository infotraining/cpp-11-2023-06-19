#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

struct meters
{
    double value;
};

meters operator""_m(long double value)
{
    return meters{static_cast<double>(value)};
}

meters calc_distance(meters m, double factor)
{
    return meters{m.value * factor};
}

constexpr long double operator""_deg_to_rad(long double deg)
{
    long double radians = deg * 3.1415 / 180;
    return radians;
}

TEST_CASE("user defined literals")
{
    double x_rad = 90.0_deg_to_rad;

    meters dist = calc_distance(10.0_m, 2.0);

    CHECK(dist.value == 20.0);
}

TEST_CASE("raw-string literals")
{
    std::string raw_text = R"(c:\nasz\test)";

    auto text = R"raw(line1
line2
li)"ne3)raw";

    std::string t = "dgfdgfdgfdg\
		fdsfdsfdsffs\
		fdsfdsf";

    std::cout << raw_text << "\n";

    std::cout << text << "\n";
    std::cout << t << "\n";

    auto c_text = "text";
    auto str_text = "text"s;
    auto my_dist = 10.0_m;
    auto my_time = 10ms;

    auto wide_str = L"Text";
;}