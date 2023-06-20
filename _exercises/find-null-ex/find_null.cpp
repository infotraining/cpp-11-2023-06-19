#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

namespace ModernCpp
{
    inline namespace ver_2_0
    {
        template <typename T>
        auto find_null(T& container)
        {
            using std::begin;
            using std::end;
            return std::find_if(begin(container), end(container),
                [](const auto& ptr) { return ptr == nullptr; });
        }
    }

    namespace ver_1_0
    {
        template <typename TContainer>
        auto find_null(TContainer& container)
        {
            using std::begin;
            using std::end;
            for (const auto& it = begin(container); it != end(container); ++it)
            {
                if (*it == nullptr)
                {
                    return it;
                }
            }
            return end(container);
        }
    }

    namespace legacy
    {
        template <typename TContainer>
        typename TContainer::iterator find_null(TContainer& container)
        {
            for (typename TContainer::iterator it = container.begin(); it != container.end(); ++it)
            {
                if (*it)
                {
                    return it;
                }
            }
            return container.end();
        }
    }
}

TEST_CASE("find_null description")
{
    using namespace ModernCpp;

    SECTION("finds first null pointer in a std container of raw pointers")
    {
        vector<int*> ptrs = {new int{9}, new int{10}, NULL, new int{20}, nullptr, new int{23}};

        std::vector<int*>::iterator first_null_pos = find_null(ptrs);

        REQUIRE(distance(ptrs.begin(), first_null_pos) == 2);

        // clean-up
        for (const auto* ptr : ptrs)
            delete ptr;
    }

    SECTION("finds first null pointer in an array of raw pointers")
    {
        int* ptrs[] = {new int{9}, new int{10}, NULL, new int{20}, nullptr, new int{23}};

        auto first_null_pos = find_null(ptrs);

        REQUIRE(distance(begin(ptrs), first_null_pos) == 2);

        // clean-up
        for (const auto* ptr : ptrs)
            delete ptr;
    }

    SECTION("finds first empty shared_ptr in a initializer-list of shared_ptrs")
    {
        auto sp = make_shared<int>(10);
        CHECK(sp != nullptr);

        auto il = {sp, shared_ptr<int>{}, make_shared<int>(3)};

        auto first_null_pos = find_null(il);

        REQUIRE(distance(il.begin(), first_null_pos) == 1);
    }

    SECTION("finds first empty unique_ptr in a container of unique_ptrs")
    {
        vector<unique_ptr<int>> vec_ptr;
        vec_ptr.push_back(std::unique_ptr<int>(new int(10)));
        vec_ptr.push_back(nullptr);
        vec_ptr.push_back(std::unique_ptr<int>(new int(20)));

        auto where_null = find_null(vec_ptr);

        REQUIRE(distance(vec_ptr.begin(), where_null) == 1);
    }

    SECTION("when all pointers are valid returns iterator which equals end()")
    {
        auto il = {make_shared<int>(10), shared_ptr<int>{new int(5)}, make_shared<int>(3)};

        REQUIRE(find_null(il) == il.end());
    }
}