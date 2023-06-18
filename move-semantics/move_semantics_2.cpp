#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_{nullptr}
    {
    }

    UniquePtr() : ptr_{nullptr}
    {}

    explicit UniquePtr(T* ptr)
        : ptr_{ptr}
    {
    }

    // TODO: implement move semantics

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return ptr_;
    }
};

// TEST_CASE("2---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

// TEST_CASE("move semantics - UniquePtr")
// {
//     // TODO
//     UniquePtr<Gadget> pg1{new Gadget{1, "ipad"}};
//     pg1->use();
// }