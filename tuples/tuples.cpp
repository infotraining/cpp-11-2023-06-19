#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <numeric>
#include <array>
#include <set>
#include <map>

using namespace std;

template <typename... Ts>
std::ostream operator<<(std::ostream& out, const std::tuple<Ts...>& tpl)
{
	out << "???";
	return out;
}

TEST_CASE("tuple")
{
	std::tuple<int, double, std::string> tpl1{1, 3.14, "text"};

	CHECK(std::get<0>(tpl1) == 1);
	CHECK(std::get<1>(tpl1) == 3.14);
	CHECK(std::get<2>(tpl1) == "text"s);

	std::get<0>(tpl1) = 42;

	auto tpl2 = std::make_tuple(42, 3.14, "text"s);
	auto tpl3 = std::tuple(42, 6.28, "text"s); // C++17 - CTAD

	SECTION("comparing")
	{
		//CHECK(tpl1 == tpl2);
		CHECK(tpl1 < tpl3);
	}

	SECTION("tuples & refs")
	{
		int x;
		double pi;

		std::tuple<int&, double&> ref_tpl{x, pi};

		std::get<0>(ref_tpl) = 42;

		CHECK(x == 42);

		ref_tpl = std::tuple(665, 3.14);

		CHECK(x == 665);
		CHECK(pi == 3.14);

		std::tie(x, pi) = std::tuple(666, 6.14); // tie -> tuple<int&, double&>

		CHECK(x == 666);
		CHECK(pi == 6.14);
	}
}

std::tuple<int, int, double> calculate_stats(const std::vector<int>& data)
{
	const auto [min_pos, max_pos] = std::minmax_element(data.begin(), data.end());

	double avg = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

	return std::make_tuple(*min_pos, *max_pos, avg);
}

TEST_CASE("tuples & functions")
{
	std::vector<int> data = {6, 2, 5, 42, 665, 234, 65, 44, 1};

	SECTION("Before C++17")
	{
		int min, max;
		double avg;

		// write a function that finds minimum, maximum & average value in data
		std::tie(min, max, avg) = calculate_stats(data);

		REQUIRE(min == 1);
		REQUIRE(max == 665);
		REQUIRE_THAT(avg, Catch::Matchers::WithinAbs(118.222, 0.001));
	}

	SECTION("Since C++17")
	{
		const auto [min, max, avg] = calculate_stats(data);

		REQUIRE(min == 1);
		REQUIRE(max == 665);
		REQUIRE_THAT(avg, Catch::Matchers::WithinAbs(118.222, 0.001));
	}
}

std::array<int, 3> get_coord()
{
	return {1, 2, 3};
}

struct Timestamp
{
	int h, m, s;
};

TEST_CASE("sb & compatible types")
{
	SECTION("native arrays")
	{
		int coords[3] = {1, 2, 3};

		auto& [x, y, z] = coords;

		CHECK(z == 3);

		z = 6;

		CHECK(coords[2] == 6);
	}

	SECTION("std::array")
	{
		auto [x, y, z] = get_coord();

		CHECK(z == 3);
	}

	SECTION("std::pair")
	{
		std::set<int> my_set = {1, 2, 3};

		if (auto [pos, success] = my_set.insert(4); success)
		{
			std::cout << "Item was inserted!" << *pos << "\n";
		}
	}

	SECTION("struct")
	{
		auto [hours, minutes, seconds] = Timestamp{14, 3, 0};
	}

	SECTION("The best use-case")
	{
		std::map<std::string, double> data = {{"pi"s, 3.14}, {"e"s, 2.71}};

		for (const auto& [key, value] : data)
			std::cout << key << " - " << value << "\n";

		std::vector vec = {"zero", "one", "two" };
		
		for (auto [i, it] = std::tuple{0, begin(vec)}; i < std::size(vec); ++i, ++it)
		{
			std::cout << i << " - " << *it << "\n";
		}
	}
}

/////////////////////////////////////////////////
// tuples & == < > != >= <=

struct Data
{
	int x;
	std::string text;

	auto tied() const
	{
		return std::tie(x, text);
	}

	bool operator==(const Data& other) const
	{		
		return this->tied() == other.tied();
	}

	bool operator<(const Data& other) const
	{
		return tied() < other.tied();
	}

	//auto operator<=>(const Data&) const = default; // C++20
};

TEST_CASE("comparisons")
{
	Data d1{1, "one"};

	CHECK(d1 == Data{1, "one"});
	CHECK(d1 < Data{1, "two"});
}