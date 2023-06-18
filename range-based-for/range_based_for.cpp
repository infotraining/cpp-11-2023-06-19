#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("range-based-for")
{
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    SECTION("iterating over container in C++98")
    {
        for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            (*it) *= 2;
        }

        for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
}