#include <algorithm>
#include <any>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <map>

using namespace std;

TEST_CASE("any")
{
	std::any anything;

	REQUIRE(anything.has_value() == false);

	anything = 42;
	anything = "text"s;
	anything = std::vector<int>{1, 2, 3};

	SECTION("type")
	{
		std::cout << anything.type().name() << "\n";
	}

	SECTION("any_cast & copy")
	{
		std::vector<int> vec = std::any_cast<std::vector<int>>(anything);

		CHECK(vec == std::vector{1, 2, 3});

		REQUIRE_THROWS_AS(std::any_cast<std::string>(anything), std::bad_any_cast);
	}

	SECTION("any_cast & pointers")
	{
		std::vector<int>* ptr_vec = std::any_cast<std::vector<int>>(&anything);

		CHECK(ptr_vec != nullptr);

		REQUIRE(std::any_cast<std::string>(&anything) == nullptr);
	}
}

struct Data
{
	Data(std::initializer_list<std::any> args)
	{
		for(auto&& arg : args)
		{	
			if (const std::string* ptr = std::any_cast<std::string>(&arg))
			{
				std::cout << "String: " << *ptr << "\n";
			}
		}
	}
};

using LST = std::initializer_list<std::any>;

class DynamicDictionary
{
	std::map<std::string, std::any> dict_;
public:
	template<typename TArg>
	auto insert(const std::string& key, TArg&& arg)
	{
		return dict_.emplace(key, std::forward<TArg>(arg));
	}

	template <typename T>
	auto get(const std::string& key) const
	{
		const std::any& obj = dict_.at(key);
		return std::any_cast<T>(obj);
	}
};

TEST_CASE("use cases")
{
	Data d1 = LST{ 1, 4.14, "text"s, LST{2, 6.28, "text"s} };

	DynamicDictionary dd;

	dd.insert("name", "Jan");
	dd.insert("age", 42);

	REQUIRE(dd.get<int>("age") == 42);
}

