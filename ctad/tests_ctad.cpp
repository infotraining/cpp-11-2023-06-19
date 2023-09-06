#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <list>
#include <map>
#include <array>

using namespace std;

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    puts(__FUNCSIG__);
}

template <typename T>
void deduce2(T& arg)
{    
    puts(__FUNCSIG__);
}

template <typename T>
void deduce3(T&& arg) // universal reference
{
    puts(__FUNCSIG__);
}


TEST_CASE("Template Argument Deduction - case 1")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = cx;
    int tab[10];

    deduce1(x); // int -> int
    deduce1(cx); // const int -> int
    deduce1(ref_x); // int& -> int
    deduce1(cref_x); // const int& -> int
    deduce1(tab); // int[10] -> int*

    auto ax1 = x;
    auto ax2 = cx; // int
}

TEST_CASE("Template Argument Deduction - case 2")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = cx;
    int tab[10];

    deduce2(x);
    deduce2(cx);
    deduce2(ref_x);
    deduce2(cref_x);
    deduce2(tab);
}

TEST_CASE("Template Argument Deduction - case 3")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = cx;
    int tab[10];

    deduce3(x); // x: int lvalue -> int&
    deduce3(42);
}

/////////////////////////
// CTAD

template <typename T1, typename T2>
struct ValuePair
{
    T1 fst;
    T2 snd;

    ValuePair(const T1& f, const T2& s) : fst{f}, snd{s}
    {}
};

// deduction guides
template <typename T1, typename T2>
ValuePair(T1, T2) -> ValuePair<T1, T2>;

ValuePair(const char*, const char*) -> ValuePair<std::string, std::string>;

// helper function - legacy style
template <typename T1, typename T2>
ValuePair<T1, T2> make_value_pair(T1 fst, T2 snd)
{
    return ValuePair<T1, T2>(fst, snd);
}

TEST_CASE("Before C++17")
{
    ValuePair<int, double> vp1{42, 3.14};

    vp1 = make_value_pair(665, 6.28);

    auto vp3 = make_value_pair<double>(42, 3.13);
}

TEST_CASE("Since C++17")
{
    ValuePair vp1{42, 3.14}; // ValuePair<int, double>
    ValuePair vp2{"text", "text"s}; // ValuePair<const char*, string>  
    ValuePair vp3{"text", "text"};  // ValuePair<const char*, string>  
}

TEST_CASE("vector")
{
    std::vector<int> vec1 = {1, 2, 3};
    std::vector vec2 = {1, 2, 3};    
    
    std::vector vec3{vec2}; // cc -> std::vector<int>
}

template <typename T>
struct Wrapper
{
    T value;

    void print() const
    {
        std::cout << "Value: " << value << "\n";
    }
};

// C++17 requires deduction guide for an aggregate
template <typename T>
Wrapper(T) -> Wrapper<T>;

TEST_CASE("CTAD - aggregates")
{
    Wrapper w1{42};
    w1.print();

    Wrapper w2{"text"};
    w2.print();
}

namespace GreatExperiment
{
    template <typename... Ts>
    struct Tuple : std::tuple<Ts...>
    {
        using std::tuple<Ts...>::tuple;

        template <typename T1, typename T2, typename T3, typename T4>
        Tuple(std::pair<T1, T2> a, std::pair<T3, T4> b) 
            : Tuple(a.first, a.second, b.first, b.second)
        {
        }
    };

    template <typename... Ts>
    Tuple(Ts...) -> Tuple<Ts...>;

    template <typename T1, typename T2, typename T3, typename T4>
    Tuple(std::pair<T1, T2>, std::pair<T3, T4>) -> Tuple<T1, T2, T3, T4>;

    /////////////////////////////
    // Array

    template <typename T, size_t N>
    struct Array
    {
        T items[N];

        //...
    };

    template <typename T, typename... Ts>
    Array(T, Ts...) -> Array<T, sizeof...(Ts) + 1>;
}

namespace LegacyCode
{
    int* create_on_heap()
    {
        return new int(13);
    }
}

TEST_CASE("CTAD - std library")
{
    std::pair<int, double> p1{42, 3.14};
    std::pair p2{42, 3.14};
    std::pair p3{"text", "test"s}; 

    std::tuple tpl1{1, 3.14, "test", "text"s};
    std::tuple tpl2{p1, p2};

    GreatExperiment::Tuple<int, double, std::string> gt1{1, 3.14, "text"};

    CHECK(std::get<0>(gt1) == 1);

    GreatExperiment::Tuple gt2a{1, 3.14};
    GreatExperiment::Tuple gt2b{1, 3.14};

    CHECK(gt2a == gt2b);

    GreatExperiment::Tuple gt3{p1, p2};

    std::optional<int> opt_int = 42;
    std::optional opt_double = 3.14;
    std::optional opt_real = opt_double; // CTAD - copy rule

    std::unique_ptr<int> uptr{LegacyCode::create_on_heap()};

    std::unique_ptr target = std::move(uptr);
    std::shared_ptr sptr = std::move(target);

    std::function f = LegacyCode::create_on_heap;

    std::vector vec = {1, 2, 3};
    
    std::list lst = {1, 2, 3};

    std::map dict = { std::pair{1, "one"}, {2, "two"} };

    std::vector vmap(dict.begin(), dict.end());

    std::array tab1 = {1, 2, 3, 4};

    GreatExperiment::Array tab2 = {1, 2, 3};
} 

