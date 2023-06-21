#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <utility>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_{nullptr}
    {
    }

    UniquePtr()
        : ptr_{nullptr}
    {
    }

    explicit UniquePtr(T* ptr)
        : ptr_{ptr}
    {
    }

    // UniquePtr(T&& source )
    //     : ptr_{source.ptr_}
    // {
    //     source.ptr_ = nullptr;
    // }

    UniquePtr(UniquePtr&& source) // move constructor
        : ptr_{std::exchange(source.ptr_, nullptr)}
    {
    }

    UniquePtr& operator=(UniquePtr&& source) // move assignment
    {
        if (this != &source)
        {
            delete ptr_;
            // ptr_ = source.ptr_;
            // source.ptr_ = nullptr;
            ptr_ = std::exchange(source.ptr_, nullptr);
        }

        return *this;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

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

UniquePtr<Gadget> create_gadget()
{
    static int id_gen = 0;
    int id = ++id_gen;
    return UniquePtr<Gadget>(new Gadget{id, "Gadget#" + std::to_string(id) });
}

namespace Explain
{
    template <typename T, typename... TArgs>
    UniquePtr<T> make_unique(TArgs&&... args)
    {
        return UniquePtr<T>{new T(std::forward<TArgs>(args)...)};
    }
}

TEST_CASE("move semantics - UniquePtr")
{
    //UniquePtr<Gadget> pg1{new Gadget{1, "ipad"}};
    auto pg1 = Explain::make_unique<Gadget>(1, "ipad");
    pg1->use();

    UniquePtr<Gadget> pg2 = std::move(pg1);         // move constructor

    pg1 = UniquePtr<Gadget>(new Gadget{2, "ipod"}); // move assignment

    std::vector<UniquePtr<Gadget>> gadgets;
    gadgets.push_back(std::move(pg1));
    gadgets.push_back(UniquePtr<Gadget>{new Gadget{3, "smart?"}});
    gadgets.push_back(create_gadget());
    gadgets.push_back(create_gadget());
    gadgets.push_back(create_gadget());
    gadgets.push_back(create_gadget());

    for(const auto& ptr_g : gadgets)
    {
        ptr_g->use();
    }

    gadgets.clear();
}

class User
{
    std::string name_;
    UniquePtr<Gadget> gadget_;
public:
    User(std::string name, UniquePtr<Gadget> gadget)
        : name_{std::move(name)}, gadget_{std::move(gadget)}
    {
    }

    void foo()
    {
        std::cout << name_ << "\n";
        gadget_->use();
    }
};

TEST_CASE("move semantics - User")
{
    std::string name = "Jan";
    User user1{name, create_gadget()};
    user1.foo();

    User target = std::move(user1);
    target.foo();
}