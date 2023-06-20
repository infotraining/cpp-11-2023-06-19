#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <any>
#include <map>

using namespace std;

TEST_CASE("auto - first look")
{
    auto x = 42;
    auto dx = 0.001f;

    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();

    *it = 6;

    CHECK(vec[0] == 6);

    SECTION("* or &")
    {
        auto x = 42.0;

        const auto* const ptr_x = &x;

        auto& y = x;
    }    
}

TEST_CASE("const hell")
{
    int x = 10;
    const int cx = 10;

    int* p = &x;

    SECTION("pointers to const")
    {
        const int* ptr_to_const0 = nullptr;
        const int* ptr_to_const1 = &cx; // west-const
        int const* ptr_to_const2 = &cx; // east-const

        ptr_to_const1 = &cx;
        //(*ptr_to_const1) = 665;
    }

    SECTION("const pointer to mutable")
    {
        int* const const_ptr_to_value = &x;
        // const_ptr_to_value = &x; // ERROR = pointer is const
        
        (*const_ptr_to_value) = 665;

        CHECK(x == 665);
    }

    SECTION("const pointer to const")
    {
        const int* const const_ptr_to_value1 = &x; // west-const
        int const* const const_ptr_to_value2 = &x; // east-const
        // const_ptr_to_value = &x; // ERROR = pointer is const

        CHECK(*const_ptr_to_value1 == 10);
    }
}

namespace Explain
{
    template <typename T>
    size_t size(const T& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    size_t size(T (&table)[N])
    {
        return N;
    }
}

TEST_CASE("using auto")
{
    int vec[] = {1, 2, 3, 4};
    for(auto it = std::begin(vec); it != std::end(vec); ++it)
    {   
        std::cout << *it << " ";
    }

    for(size_t i = 0; i < Explain::size(vec); ++i)
    {
        std::cout << vec[i] << " ";
    }
    
    std::cout << "\n";
}

TEST_CASE("variable declaration & auto")
{
    int x = 42;
    auto ax = 42; // int

    int y(10);
    auto ay(10);  // int

    int z{10};
    auto az{10};  // int - since C++17

    auto lst = {1}; // std::initializer<int>    
}

int foo(int arg)
{
    return arg;
}

template <typename T>
void deduce1(T arg)
{
    //std::cout << __FUNCSIG__ << "\n";
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

TEST_CASE("auto type deduction - case 1")
{
    int x = 10;
    auto ax1 = 10; // int
    
    const int cx = 42;
    auto ax2 = cx; // int  - const is removed

    int& ref_x = x;
    auto ax4 = ref_x; // int - ref is removed
    
    const int& cref_x = x;
    auto ax5 = cref_x; // int - ref is removed & const is removed

    int tab[10];
    auto ax6 = tab; // int* - array decays to pointer    
}

TEST_CASE("auto type deduction - case 2")
{
    int x = 10;
    auto& ax1 = x; // int&

    const int cx = 42;
    auto& ax2 = cx; // const int&

    int& ref_x = x;
    auto& ax3 = ref_x; // int&

    const int& cref_x = x;
    auto& ax4 = cref_x; // const int&
    
    int tab[10];
    auto& ax5 = tab; // int (&ax5)[10]
}

struct Default 
{ 
    int foo() const { return 1; } 
};
 
struct NonDefault
{
    NonDefault() = delete;
    auto foo() -> int const { return 1; }
};

auto foo() -> int // int foo()
{
    return 42;
}

auto bar() -> int;

int bar()
{
    return 665;
}

template <typename T>
auto multiply(T a, T b) 
{
    return a * b;
}

TEST_CASE("decltype")
{
    std::vector<int> vec1 = {1, 2, 3};

    auto vec2 = vec1;
    CHECK(vec2.size() == 3);

    decltype(vec1) vec3;
    CHECK(vec3.size() == 0);

    std::map<std::string, float> coll = { {"pi", 3.14f}, {"e", 2.81f } };

    using RefFloat = decltype(coll.at(std::declval<std::string>())); 
    RefFloat pi = coll.at("pi");

    decltype(Default().foo()) obj1;
    decltype(std::declval<NonDefault>().foo()) obj2;
}

auto describe_number(int x)
{
    if (x % 2 == 0)
        return "even"s;
    else
        return "odd"s;
}

TEST_CASE("auto - return type deduction for functions")
{
    auto result = describe_number(12);
}

template <typename TContainer>
decltype(auto) get_nth(TContainer& container, size_t nth)
{
    return container[nth];
}

template <typename F, typename TArg>
decltype(auto) call(F f, TArg arg)
{
    std::cout << "Log: calling " << typeid(f).name() << "\n";
    return f(arg);
}

TEST_CASE("get_nth")
{
    std::vector<std::string> vec = {"1", "2", "3", "4"};

    CHECK(get_nth(vec, 2) == "3"s);

    get_nth(vec, 2) = "43"s;

    CHECK(vec[2] == "43");

    std::vector<bool> flags = {0, 1, 1, 0};

    CHECK(get_nth(flags, 2) == true);

    get_nth(flags, 2) = false;

    CHECK(get_nth(flags, 2) == false);

    std::cout << call(describe_number, 42) << "\n";
}

