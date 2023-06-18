#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void foo(int* ptr)
{
    std::cout << "foo(int*: " << ptr << ")\n";
}

TEST_CASE("NULL is imperfect null pointer")
{
    int* ptr = NULL;

    foo(ptr);
    foo(NULL);
}
