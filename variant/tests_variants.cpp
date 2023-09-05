#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;

TEST_CASE("variant")
{
	std::variant<int, double, std::string> v1;

	REQUIRE(std::holds_alternative<int>(v1) == true);
	REQUIRE(v1.index() == 0);
	REQUIRE(std::get<int>(v1) == 0);

	v1 = 42;
	v1 = 3.14;
	v1 = "text"s;

	SECTION("get - may throw exception")
	{
		REQUIRE_THROWS_AS(std::get<int>(v1), std::bad_variant_access);

		std::string& str = std::get<std::string>(v1);
		REQUIRE(str == "text"s);
	}

	SECTION("get_if - works on pointers")
	{
		if (std::string* ptr_str = std::get_if<std::string>(&v1))
		{
			std::cout << *ptr_str << "\n";
		}

		REQUIRE(std::get_if<int>(&v1) == nullptr);
	}

	//v1 = std::vector{1, 2, 3};
}

struct NoDefault
{
	int value;

	NoDefault() = delete;

	NoDefault(int value) : value{value}
	{}
};

TEST_CASE("variant - monostate")
{
	std::variant<std::monostate, NoDefault, int, double> v;

	REQUIRE(std::holds_alternative<std::monostate>(v));

	v = 42;
	v = 3.14;
	v = NoDefault{665};
}

// Visitor
struct Printer
{
	std::string name;

	std::string operator()(int x) const
	{
		std::cout << "int: " << x << "\n";

		return name + " - "s + "int: "s + std::to_string(x);
	}

	std::string operator()(double x) const
	{
		std::cout << "double: " << x << "\n";
		return name + " - "s + "double: "s + std::to_string(x);
	}

	std::string operator()(const std::string& s) const
	{
		std::cout << "std::string: " << s << "\n";

		return name + " - "s + s + s;
	}

	std::string operator()(const std::vector<int>& v) const
	{
		std::cout << "std::vector: " << v.size() << "\n";

		return name + " - "s + std::to_string(v.size());
	}
};

struct GenericPrinter
{
	template <typename T>
	auto operator()(const T& obj) const
	{
		const type_info& ti = typeid(obj);

		std::cout << ti.name() << ": " << obj << "\n";
	}
};

struct A
{
	std::string operator()(int x) const
	{}
};

struct B
{
	std::string operator()(double x) const
	{}
};

struct AB : A, B
{
	using A::operator();
	using B::operator();
};

//////////////////////
// overload

template <typename... Ts>
struct overload : Ts...
{
	using Ts::operator()...; // C++17
};

// required in C++17 only
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

/////////////////////

TEST_CASE("visiting variants")
{
	std::variant<int, double, std::string> v1 = 3.14;

	std::string result = std::visit(Printer{"Printer"}, v1);

	std::cout << "result: " << result << "\n";

	std::visit(GenericPrinter{}, v1);

	std::visit([](const auto& obj) {
		const type_info& ti = typeid(obj);
		std::cout << ti.name() << ": " << obj << "\n";
	}, v1);

	SECTION("overload")
	{
		std::visit(overload{
				[](int i) { std::cout << "int: " << i << "\n";  },
				[](double d) { std::cout << "double: " << d << "\n"; },
				[](const std::string& s) { std::cout << "string: " << s << "\n"; }
			}, v1);

		overload printer{
			[](int i) { std::cout << "int: " << i << "\n";  },
			[](double d) { std::cout << "double: " << d << "\n"; },
			[](const std::string& s) { std::cout << "string: " << s << "\n"; },
			[](const auto& obj) { std::cout << "unknown type: " << obj.size() << "\n";  }
		};

		std::variant<int, double, std::string, std::vector<int>> v2 = 3.14;
		std::visit(printer, v2);

		v2 = std::vector{1, 2, 3};
		std::visit(printer, v2);

		auto lambda = [](auto... args) { };
	}
}

