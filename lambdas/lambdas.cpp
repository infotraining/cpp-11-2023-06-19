#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <execution>
#include <functional>
#include <queue>

using namespace std;

int foo(int a)
{
    return a * a;
};

struct Foo
{
    int factor;

    int operator()(int a) const
    {
        return a * factor;
    }
};

template <typename TContainer>
void print(const TContainer& container, const std::string& prefix)
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("function & functors")
{
    CHECK(foo(4) == 16);

    Foo foonctor{10};
    CHECK(foonctor(4) == 40);

    std::vector<int> vec = {1, 2, 3, 4};
    std::vector<int> results(vec.size());

    std::transform(vec.begin(), vec.end(), results.begin(), foo);
    print(results, "results foo");

    std::transform(vec.begin(), vec.end(), results.begin(), foonctor);
    print(results, "results foonctor");
}

// closure class
struct Lambda_42376542836
{
    //Lambda_42376542836() = delete;
    auto operator()() const { std::cout << "lambda\n"; }
};

struct Lambda_237123792t7
{
    const double factor_;

    auto operator()(int n) const { return n * factor_; }
};

TEST_CASE("lambda expressions")
{
    auto lambda = [] { std::cout << "lambda\n"; };   // lambda - closure object
    lambda();

    SECTION("is interpreted as")
    {
        auto lambda = Lambda_42376542836();
        lambda();
    } 

    SECTION("stl algorithms")
    {
        std::vector<int> vec = {1, 2, 3, 4};
        std::vector<double> results(vec.size());

        int a = 2;
        double factor = 0.5;

        auto factorizer = [a, &factor](int n) { return n * factor + a; };

        factor = 100.0;

        std::transform(std::execution::par_unseq, 
            vec.begin(), vec.end(), results.begin(), 
            factorizer);

        print(results, "results lambda");

        int x = 42;

        const int my_number = [=]() -> int { 
            if (x == 42)
                return 100;
            else
                return factorizer(10);
            }();
    }
}

TEST_CASE("lambda = init expressions")
{
    auto ptr = std::make_unique<int>(42);

    auto capture = [ptr = std::move(ptr)] { std::cout << *ptr << "\n"; };
}


std::function<void()> on_print;

using Task = std::function<void()>;
std::queue<Task> q_tasks;

struct Printer
{
    int id{665};

    void startup()
    {   
        on_print = [this]() { this->print(); };
    }

    void on()
    {
        std::cout << "Printer " << id << " is on...\n";
    }

    void print()
    {
        std::cout << "Printer " << id << " is printing...\n";
    }

    void off()
    {
        std::cout << "Printer " << id << " is off...\n";
    }
};

TEST_CASE("callback")
{
    Printer prn;
    prn.startup();

    //.....
    q_tasks.push([&prn] { prn.on();});
    q_tasks.push([&prn] { prn.print();});
    q_tasks.push([&prn] { prn.off();});

    on_print();

    std::cout << "-----------\n";

    while (!q_tasks.empty())
    {
        Task task = q_tasks.front();
        task();
        q_tasks.pop();
    }
}

struct Lambda_572348529347df
{
    int seed_;

    auto operator()() { return ++seed_; }
};

auto create_generator(int seed = 0) 
{
    return [seed]() mutable -> int { return ++seed; };
}

TEST_CASE("generator")
{
    auto gen1 = create_generator(665);
    CHECK(gen1() == 666);
    CHECK(gen1() == 667);
    CHECK(gen1() == 667);

    auto gen2 = create_generator(1000);
    CHECK(gen2() == 1001);
}

struct Lambda_37537256972345g
{
    template <typename T>
    auto operator()(const T& obj) const { return obj.size(); }
};

TEST_CASE("generic lambdas")
{
    auto sizer = [](const auto& obj) { return obj.size(); };

    std::string str = "text";
    CHECK(sizer(str) == 4);


    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    CHECK(sizer(vec) == 6);
}

template <class F, class... Args>
void for_each_arg(F f, Args&&... args)
{
    (f(std::forward<Args>(args)), ...); // fold expressions
}

TEST_CASE("for_each_arg")
{
    auto printer = [](const auto& arg) { std::cout << arg << "\n"; };

    for_each_arg(printer, 1, 2, "text", std::string{"abc"});
}