#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <vector>
#include <array>

using namespace std;

TEST_CASE("if with initializer")
{
    std::vector vec = {1, 2, 543, 235, 534, 665, 654, 3456};

    if (auto pos = std::find(begin(vec), end(vec), 665); pos != end(vec))
    {
        std::cout << "Found: " << *pos << "\n";
    }
    else
    {
        std::cout << "Item not found!\n";
        assert(pos == end(vec));
    }        

    if (auto [a, b] = std::tuple{2, 2}; a == b)
    {
        std::cout << "a: " << a << " == b: " << b << "\n";
    }
}

/////////////////////////////////////////////
// SFINAE + enable_if

template <typename T, T v>
struct IntegralConstant
{
    static constexpr T value = v;
};

template <typename T, T v>
constexpr T IntegralConstant_v = IntegralConstant<T, v>::value;

static_assert(IntegralConstant<int, 42>::value == 42);
static_assert(IntegralConstant_v<int, 42> == 42);

template< bool B >
using BoolConstant = IntegralConstant<bool, B>;

static_assert(BoolConstant<true>::value == true);
static_assert(BoolConstant<false>::value == false);

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

///////////////////////////////
// IsVoid

template <typename T>
struct IsVoid : std::false_type /*: FalseType */
{
};

template <>
struct IsVoid<void> : std::true_type /*: TrueType */
{    
};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

///////////////////////////////////////
// EnableIf

template <bool B, typename T = void>
struct EnableIf
{};

template <typename T>
struct EnableIf<true, T>
{
    using type = T;
};

template <bool B, typename T = void>
using EnableIf_t = typename EnableIf<B, T>::type;

TEST_CASE("EnableIf")
{
    static_assert(sizeof(int) == 4);
    EnableIf<sizeof(int) == 4, int>::type x;
}

namespace BeforeCpp17
{
    template <typename T, typename = EnableIf_t<std::is_integral_v<T>>>
    bool is_power_of_2(T value)
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <typename T, typename = void, typename = EnableIf_t<std::is_floating_point_v<T>>>
    bool is_power_of_2(T value)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
}

namespace Alt
{
    template <typename T>
    auto is_power_of_2(T value) -> EnableIf_t<std::is_integral_v<T>, bool>
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <typename T>
    auto is_power_of_2(T value) -> EnableIf_t<std::is_floating_point_v<T>, bool>
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
}

template <typename T>
bool is_power_of_2(T value)
{
    if constexpr (std::is_integral_v<T>)
    {
        return value > 0 && (value & (value - 1)) == 0;
    }
    else if constexpr(std::is_floating_point_v<T>)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
    else
    {
        throw std::invalid_argument("Argument type not supported");
    }
}

constexpr size_t dataset_size = 1024;

template <typename F>
auto create_dataset(F requested_size)
{
    if constexpr(requested_size() > 512)
    {
        std::vector<int> data(requested_size());
        for (size_t i = 0; i < data.size(); ++i)
            data[i] = 2 * i;
        return data;
    }
    else
    {
        std::array<int, requested_size()> data{};

        for (size_t i = 0; i < data.size(); ++i)
            data[i] = 2 * i;
        return data;
    }
}

template <typename T>
auto create_dataset()
{
    if constexpr (std::is_floating_point_v<T>)
    {
        std::vector<int> data(dataset_size);
        for (size_t i = 0; i < data.size(); ++i)
            data[i] = 2 * i;
        return data;
    }
    else
    {
        std::array<int, dataset_size> data{};

        for (size_t i = 0; i < data.size(); ++i)
            data[i] = 2 * i;
        return data;
    }
}

TEST_CASE("is_power_of_2")
{
    CHECK(is_power_of_2(8l));
    CHECK(is_power_of_2(64ll));
    CHECK(is_power_of_2(128));
    CHECK(is_power_of_2(7) == false);

    CHECK(is_power_of_2(8.0));
    CHECK(is_power_of_2(64.0f));

    CHECK(is_power_of_2("text"s));
}

TEST_CASE("constexpr if")
{
    auto ds1 = create_dataset([] { return 1024; });
    auto ds2 = create_dataset([] { return 64; });
    
}