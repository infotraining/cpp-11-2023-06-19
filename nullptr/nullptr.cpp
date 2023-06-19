#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

void foo(int* ptr)
{
    std::cout << "foo(int*: " << ptr << ")\n";
}

void foo(nullptr_t) 
{
    std::cout << "foo(nullptr_t: " << nullptr << ")\n";
}


TEST_CASE("NULL is imperfect null pointer")
{
    int* ptr = NULL;

    unique_ptr<int> uptr1 = nullptr;

    if (!uptr1) std::cout << "aaa\n";

    foo(ptr);
    foo(uptr1.get());
}