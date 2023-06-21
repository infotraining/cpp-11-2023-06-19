#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
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

namespace ModernCode
{
    // forward declarations
    std::unique_ptr<Gadget> get_gadget(const std::string& name);
    void use(std::unique_ptr<Gadget> g);
    void use(Gadget* g);


    // definitions
    std::unique_ptr<Gadget> get_gadget(const std::string& name)
    {
        static int id = 665;
        return std::make_unique<Gadget>(++id, name);
    }

    void use(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
}

TEST_CASE("Legacy hell with dynamic memory")
{
    using namespace ModernCode;

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use(g.get());
    } 

    {
        use(get_gadget("ipad"));
    } 

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use(std::move(g));        
    }    
}


std::queue<int> q;
std::mutex mtx_q;

TEST_CASE("critical section")
{
    mtx_q.lock(); // CS start

    q.push(1);
    q.push(2); // XXXXX

    mtx_q.unlock(); // CS ends - may leak

    {
        std::lock_guard lk{mtx_q}; // CS start
        q.push(1);
        q.push(2); 
    } // implicit unlock - CS ends
}

TEST_CASE("std::shared_ptr")
{
    using namespace ModernCode;

    std::map<std::string, std::shared_ptr<Gadget>> gadgets;

    {        
        std::unique_ptr<Gadget> g = get_gadget("ipad");
        std::shared_ptr<Gadget> sg = std::move(g);
        gadgets.insert(std::make_pair("ipad", sg));

        CHECK(sg.use_count() == 2);

        gadgets.emplace("smartwatch", get_gadget("smartwatch"));
    }

    std::cout << "--------------------\n";

    gadgets.clear();

    std::cout << "--------------------\n";
}

////////////////////////////////////



TEST_CASE("multithreading")
{
    std::thread thd1;
    std::thread thd2;
    std::thread thd3;

    {
        auto g = std::make_shared<Gadget>(1, "ipad");

        thd1 = std::thread([g] { std::cout << g->name() << std::endl; });
        thd2 = std::thread([g] { 
            std::this_thread::sleep_for(2s);
            std::cout << g->name() << std::endl; 
        });

        thd3 = std::thread([weak_g = std::weak_ptr<Gadget>(g)] {
            std::this_thread::sleep_for(1s);

            if (std::shared_ptr<Gadget> g = weak_g.lock())
            {
                std::cout << g->name() << std::endl;
            }
            else
            {
                std::cout << "Zed's dead..." << std::endl;
            }
        }
        );
    }

    std::cout << "After scope" << std::endl;

    thd1.join();
    thd2.join();
    thd3.join();
}