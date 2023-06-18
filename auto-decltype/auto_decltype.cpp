#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int foo(int arg) 
{
    return arg;
}

TEST_CASE("auto type deduction - case 1")
{
    int x = 10;
    const int cx = 42;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];
}

TEST_CASE("auto type deduction - case 2")
{
    int x = 10;
    const int cx = 42;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];
}