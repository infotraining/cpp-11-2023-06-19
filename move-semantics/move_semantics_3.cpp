#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <utility>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

namespace
{
    class Data
    {
        std::string name_;
        int* data_;
        size_t size_;

    public:
        using iterator = int*;
        using const_iterator = const int*;

        Data(std::string name, std::initializer_list<int> list)
            : name_{std::move(name)}
            , data_{new int[list.size()]}
            , size_{list.size()}
        {
            std::copy(list.begin(), list.end(), data_);

            std::cout << "Data(" << name_ << ", " << data_ << ")\n";
        }

        Data(const Data& other)
            : name_(other.name_)
            , data_{new int[other.size_]}
            , size_(other.size_)
        {
            std::copy(other.begin(), other.end(), data_);

            std::cout << "Data(" << name_ << ", " << data_ << ": cc)\n";
        }

        Data& operator=(const Data& other)
        {
            Data temp(other);
            swap(temp);

            std::cout << "Data=(" << name_ << ", " << data_ << ": cc)\n";

            return *this;
        }

        ///////////////////////////////////////////////
        // move constructor
        Data(Data&& source)
            : name_{std::move(source.name_)}
            , data_{std::exchange(source.data_, nullptr)}
            , size_{std::exchange(source.size_, 0)}
        {
            std::cout << "Data(" << name_ << ", " << data_ << ": mv)\n";
        }

        /////////////////////////////////////////////////
        // move assignment - TODO
        Data& operator=(Data&& source)
        {
            if (this != &source)
            {
                Data temp = std::move(source); // call to move constructor
                swap(temp);

                std::cout << "Data=(" << name_ << ", " << data_ << ": mv)\n";
            }
            return *this;
        }

        ~Data()
        {
            if (data_)
                std::cout << "~Data(" << name_ << ", " << data_ << ")\n";
            else
                std::cout << "~Data(after move)\n";

            delete[] data_;
        }

        void swap(Data& other)
        {
            name_.swap(other.name_);
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }

        const std::string& name() const
        {
            return name_;
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
    void print(const TContainer& container, const std::string& prefix)
    {
        std::cout << prefix << " - [ ";
        for (const auto& item : container)
        {
            std::cout << item << " ";
        }
        std::cout << "]\n";
    }
}

TEST_CASE("Dataset")
{
    Data dataset{"ds1", {1, 2, 3, 4, 5}};

    Data backup = dataset; // copy
    print(backup, "backup");

    Data target = std::move(dataset);
    print(target, "target");

    dataset = backup;
    print(dataset, "dataset");

    dataset = std::move(target);
    print(dataset, "dataset");
}

/////////////////////////////////////////////////

struct SuperDataSet
{
    std::vector<Data> data_rows;

    void print() const
    {
        for (const auto& row : data_rows)
        {
            std::cout << "row " << row.name() << ": ";
            for (const auto& item : row)
            {
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    }
};

TEST_CASE("SuperDataSet")
{
    SuperDataSet sds{
        {
            {"one", {1, 2, 3}},
            {"two", {4, 5, 6}},
            {"three", {7, 8, 9}}
        }
    };

    std::cout << "------------------------------------------" << std::endl;

    sds.print();

    std::cout << "------------------------------------------" << std::endl;

    SuperDataSet target = std::move(sds);

    target.print();

    std::cout << "------------------------------------------" << std::endl;
}