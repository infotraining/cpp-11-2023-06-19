#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory>

using namespace std;

// agregat
struct Data
{
    char c;
    double d;
    int data[5];

    void foo()
    {}
};

class Point
{
    int x_, y_;

public:
    Point(int x, int y) : x_{x}, y_{y}
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

TEST_CASE("initialization syntax since C++11")
{
    SECTION("simple types")
    {
        int x = 10;
        int y(10);
        auto z{10}; // *
        int iz{10};
        REQUIRE(x == y);

        int v{};
        int* ptr1 = nullptr;
        int* ptr2{};
        CHECK(ptr1 == ptr2);        
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
        Point pt2{1, 2};

        auto ptr_pt = std::make_unique<Point>(45, 55);

        REQUIRE(pt1.x() == 1);
        REQUIRE(pt1.y() == 2);
    }

    SECTION("containers")
    {
        const std::vector<int> vec = {1, 2, 3};        
    }
}

void print_list(std::initializer_list<int> lst)
{
    for(const auto& item : lst)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

class NoCopyable
{
protected:
    NoCopyable() = default;
 
public:
    NoCopyable(const NoCopyable&) = delete;
    NoCopyable& operator=(const NoCopyable&) = delete;
};

class Array : public NoCopyable
{
    int* items_{};
    size_t size_{};
    inline static size_t counter{};
public:
    using iterator = int*;
    using const_iterator = const int*;    

    Array(size_t size, int value) :
        items_{new int[size]}, size_{size}
    {
        std::fill_n(items_, size_, value);
        ++counter;
    }

    Array(std::initializer_list<int> lst)
        : items_{new int[lst.size()]}, size_{lst.size()}
    {
        std::copy(lst.begin(), lst.end(), items_);
        ++counter;
    } 

    ~Array()
    {
        delete[] items_;
    }

    iterator begin()
    {
        return items_;
    }

    iterator end()
    {
        return items_ + size_;
    }

    const_iterator begin() const
    {
        return items_;
    }

    const_iterator end() const
    {
        return items_ + size_;
    }
};

void foo(int x)
{
    std::cout << "foo(" << x << ")\n";
}

void foo(double) = delete;

struct IndexedSet : std::set<int>
{
    using std::set<int>::set; // inheriting all constructors

    const int& operator[](size_t index)
    {
        auto pos_n = next(begin(), index);
        return *pos_n;
    }

    const int& at(size_t index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bound");

        auto pos_n = next(begin(), index);
        return *pos_n;
    }
};

TEST_CASE("initializer_list")
{
    print_list({1, 2, 3, 4});
    print_list({1, 2, 3, 4, 6, 7, 8});

    Array arr = {1, 2, 3, 4};

    for(const auto& item : arr)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";

    Array another_arr{10, 1};

    for(const auto& item : another_arr)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";

    //Array backup = another_arr;

    foo(42);
    //foo(42.665); // foo(double) is deleted


    IndexedSet numbers = {1, 65, 4, 345, 32, 42};

    CHECK(numbers[1] == 4);
}

