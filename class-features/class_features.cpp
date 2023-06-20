#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class IGadget
{
public:
    virtual void use() const = 0;
    virtual ~IGadget() = default;
};

class Gadget : public IGadget
{
    int id_ {-1};
    std::string name_ = "unknown";

public:
    Gadget() = default;

    Gadget(int id, std::string name)
        : id_ {id}
        , name_ {std::move(name)}
    {
    }

    Gadget(int id) : Gadget{id, "not-set"}
    {}

    int id() const
    {
        return id_;
    }

    std::string name() const
    {
        return name_;
    }

    void use() const override
    {
        std::cout << "Using gadget: " << id() << " - " << name() << "\n";
    }
};

class SuperGadget : public Gadget
{
public:
    using Gadget::Gadget;

    SuperGadget(int id)
        : Gadget {id, "not-set(super gadget)"}
    {
    }

    void use() const override
    {
        std::cout << "Using super gadget: " << id() << " - " << name() << "\n";
    }
};

class HyperGadget final : public SuperGadget
{
public: 
    using SuperGadget::SuperGadget;

    void use() const override
    {
        std::cout << "Using hyper gadget: " << id() << " - " << name() << "\n";
    }
};

class ISizeable
{
public:
    virtual size_t size() const = 0;
};

struct A : ISizeable
{
    int s;

    size_t size() const override { return s; }
};

void client(ISizeable& obj)
{
    auto s = obj.size();
}

template <typename TSizeable> 
void client_t(TSizeable& obj)
{
    auto s = obj.size();
}

TEST_CASE("poly")
{
    A a{};
    client(a);

    std::vector<int> vec;
    client_t(vec);
    client_t(a);
}

TEST_CASE("virtual function")
{
    Gadget g{665};
    SuperGadget sg{42};

    g.use(); // Gagdet::use()
    sg.use(); // SuperGadget::use()
    sg.Gadget::use(); // Gadget::use()

    IGadget* ptr_g = &sg;
    ptr_g->use(); // SuperGadget
}

// TEST_CASE("inheritance of constructors")
// {
//     SuperGadget sg1 {1, "super-gadget"};
//     SuperGadget sg2 {4};

//     Gadget& g_ref = sg1;
//     g_ref.use();
// }