#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class IGadget
{
public:
    virtual void use() const = 0;
    virtual ~IGadget() { } // TODO: replace user provided implementation with default
};

class Gadget : public IGadget
{
    int id_ {-1};
    std::string name_ = "unknown";

public:
    // TODO: define default constructor

    Gadget(int id, std::string name)
        : id_ {id}
        , name_ {std::move(name)}
    {
    }

    // TODO: delegating constructor

    int id() const
    {
        return id_;
    }

    std::string name() const
    {
        return name_;
    }

    void use() const // TODO: use override
    {
        std::cout << "Using gadget: " << id() << " - " << name() << "\n";
    }
};

class SuperGadget : public Gadget
{
public:
    // TODO: inherit constructors from Gadget

    SuperGadget(int id)
        : Gadget {id, "not-set(super gadget)"}
    {
    }

    void use() // TODO: control overriding a virtual function
    {
        std::cout << "Using super gadget: " << id() << " - " << name() << "\n";
    }
};

class HyperGadget /* TODO: mark class as final specialization */ : public SuperGadget
{
public:
    // TODO: inherit constructors from SuperGadget

    void use() const // TODO: control overriding a virtual function
    {
        std::cout << "Using hyper gadget: " << id() << " - " << name() << "\n";
    }
};

// TEST_CASE("inheritance of constructors")
// {
//     SuperGadget sg1 {1, "super-gadget"};
//     SuperGadget sg2 {4};

//     Gadget& g_ref = sg1;
//     g_ref.use();
// }