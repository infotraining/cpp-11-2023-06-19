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

    SECTION("since C++11")
    {
        for(auto& item : vec)
        {
            item *= 2;
        }

        for(auto item : vec)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    SECTION("is interpreted as")
    {
        auto&& container = vec;
        auto begin_ = vec.begin(); // or std::begin(vec)
        auto end_  = vec.end();    // or std::end(vec)
        for(; begin_ != end_; ++begin_)
        {
            int item = *begin_;
            std::cout << item << " ";
        }
    }

    SECTION("beware - efficiency")
    {
        std::vector<std::string> words = { "one", "two", "three" };
        
        for(const auto& word : words)
        {
            std::cout << word << "\n";            
        }
    }

    SECTION("arrays")
    {
        int tab[] = {1, 2, 3, 4};

        for(const auto& item : tab)
        {
            std::cout << item << "\n";
        }
    }

    SECTION("arrays")
    {       
        for(const auto& item : {1, 2, 3, 4})
        {
            std::cout << item << "\n";
        }
    }
}