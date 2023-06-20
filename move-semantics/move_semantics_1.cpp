#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

std::string full_name(const std::string& first_name, std::string&& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& lref_name = name;                             // lref_name binds to lvalue name
        const std::string& ref_full = full_name(name, "Kowalski"); // ref_full binds to rvalue
        // ref_full[0] = 'J'; // ERROR - const ref
    }

    SECTION("C++11")
    {
        std::string&& rv_ref_full = full_name(name, "Kowalski");
        rv_ref_full[0] = 'J';
        std::cout << rv_ref_full << "\n";

        // std::string&& rv_name = name; // ERROR - string&& cannot bind to lvalue
    }
}

template <typename T>
struct Vector
{
    void push_back(const T& item)
    {
        std::cout << "Copy item " << item << "\n";
    }

    void push_back(T&& item)
    {
        std::cout << "Move item " << item << "\n";
    }
};

TEST_CASE("vector.push_back")
{
    Vector<std::string> vec;

    {
        std::string lvalue = "lvalue";

        vec.push_back(lvalue);
        vec.push_back("rvalue");
        vec.push_back(std::move(lvalue));
        // vec.push_back(static_cast<std::string&&>(lvalue));
        // assert(lvalue.size() == 0);
        lvalue = "new text"; // OK
    }
}

namespace
{
    struct Data
    {
        int n;
        std::string str;
        std::vector<int> vec;
    };

    Data create_data()
    {
        return {1, "text", {1, 2, 3}};
    }
}

TEST_CASE("default special functions")
{
    Data source = create_data();

    Data backup = source;

    Data target = std::move(source);

    // CHECK((source.n == 1) && (source.str == "") && (source.vec == std::vector<int>{}));
    // CHECK((target.n == 1) && (target.str == "text") && (target.vec == std::vector{1, 2, 3}));
}