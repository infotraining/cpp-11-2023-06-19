#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>


// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

using namespace std;
using namespace Utils;

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);


    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
}

// TEST_CASE("Legacy hell with dynamic memory")
// {
//     using namespace LegacyCode;

//     {
//         Gadget* g = get_gadget("ipad");

//         use_gadget(g);
//     } // memory leak

//     {
//         use_gadget(get_gadget("ipad"));
//     } // memory leak

//     {
//         Gadget* g = get_gadget("ipad");

//         use(g);

//         std::cout << g->name() << std::endl; // UB - use after delete
//     }
// }