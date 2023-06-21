#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"

//#define MSVC

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

void have_fun(const Gadget& cg)
{
    puts(__PRETTY_FUNCTION__);
    cg.use();
}

void have_fun(Gadget&& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

// void use(Gadget& g)
// {
//     have_fun(g);
// }

// void use(const Gadget& g)
// {
//     have_fun(g);
// }

// void use(Gadget&& g)
// {
//     have_fun(std::move(g));
// }

template <typename TG>
void use(TG&& g)
{
    have_fun(std::forward<TG>(g));
    
    // if constexpr(std::is_reference_v<TG>)
    //     have_fun(g);
    // else
    //     have_fun(std::move(g));
}

// TEST_CASE("4---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

TEST_CASE("custom forwarding")
{
    Gadget g{1, "gadget"};
    const Gadget cg{2, "const-gadget"};

    use(g); // g is lvalue
    use(cg); // cg lvalue
    use(Gadget{3, "temp-gadget"}); // rvalue
}


template <typename F, typename... TArg>
decltype(auto) call(F&& f, TArg&&... arg)
{
    std::cout << "Log: calling " << typeid(f).name() << "\n";
    return f(std::forward<TArg>(arg)...);
}

// template <typename F, typename TArg1, typename TArg2>
// decltype(auto) call(F&& f, TArg1&& arg1, TArg1&& arg2)
// {
//     std::cout << "Log: calling " << typeid(f).name() << "\n";
//     return f(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2));
// }

template< int tries = 1, typename F, typename... Args  >
auto timing(F&& f, Args&&... args)
{
    auto start = std::chrono::steady_clock::now();
    for (int i{ 0 }; i < tries; ++i) std::invoke(f, std::forward<Args>(args)...);
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count(); // value in seconds
}

void foo(int)
{
    std::cout << "foo(int)" << "\n";
}

void bar(int, int)
{
    std::cout << "foo(int, int)" << "\n";    
}

TEST_CASE("call")
{
    call(foo, 42);
    call(bar, 45, 56);
}