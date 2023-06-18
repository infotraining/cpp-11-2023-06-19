#include <catch2/catch_test_macros.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_{std::move(name)}, data_{new int[list.size()]}
        , size_{list.size()}
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , data_{new int[other.size_]}
        , size_(other.size_)
    {
        std::copy(other.begin(), other.end(), data_);

        std::cout << "Data(" << name_ << ": cc)\n";
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor - TODO
    

    /////////////////////////////////////////////////
    // move assignment - TODO
    
    ~Data()
    {
        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin()
    {
        return data_;
    }

    iterator end()
    {
        return data_ + size_;
    }

    const_iterator begin() const
    {
        return data_;
    }

    const_iterator end() const
    {
        return data_ + size_;
    }
};

Data create_data_set()
{
    Data ds{"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

template <typename TContainer>
void print(const std::string& prefix, const TContainer& container)
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

// TEST_CASE("3---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

// TEST_CASE("Data & move semantics")
// {
//     Data ds1{"ds1", {1, 2, 3, 4, 5}};

//     Data backup = ds1; // copy
//     print("backup", backup);
// }