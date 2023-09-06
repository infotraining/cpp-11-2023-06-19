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
#include <memory_resource>
#include <list>

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

/////////////////////////////////////////////////
// iteration over tuple

template <size_t... Is>
struct IndexSequence
{};

IndexSequence<0, 1, 2> is1{};
IndexSequence<0, 1, 2, 3> is2{};
IndexSequence<1, 1, 2, 2> is3{};

template <typename F, typename... Ts>
void index_demo(const std::tuple<Ts...>& tpl, F f)
{
	f(std::get<0>(tpl));
	f(std::get<1>(tpl));
	f(std::get<2>(tpl));
}

template <typename F, typename... Ts, size_t... Is>
void index_demo_impl(const std::tuple<Ts...>& tpl, F f, IndexSequence<Is...> /*<0, 1, 2>*/)
{
	(..., f(std::get<Is>(tpl)));
}

/////////////////////////////////////
// tuple_for_each

template <typename F, typename... Ts, size_t... Is>
void tuple_for_each_impl(const std::tuple<Ts...>& tpl, F f, std::index_sequence<Is...> /*<0, 1, 2>*/)
{
	(..., f(std::get<Is>(tpl)));
}

template <typename F, typename... Ts>
void tuple_for_each(const std::tuple<Ts...>& tpl, F f)
{
	tuple_for_each_impl(tpl, f, std::make_index_sequence<sizeof...(Ts)>{});
}

////////////////////////////////////


TEST_CASE("iteration over tuple")
{
	std::tuple tpl{1, 3.14, "text"};

	index_demo(tpl, [](const auto& item) { std::cout << item << "\n"; });

	index_demo_impl(tpl, [](const auto& item) { std::cout << item << "\n"; }, 
					IndexSequence<0, 1, 2>{});

	tuple_for_each(tpl, [](const auto& item) { std::cout << item << "\n"; });
}

template <typename... TArgs>
auto sum(TArgs... args) // sum(1, 2, 3, 4)
{
	return (... + args);  //(((1 + 2) + 3) + 4);
}

template <typename... TArgs>
auto sum_r(TArgs... args) // sum_r(1, 2, 3, 4)
{
	return (args + ...);  // sum(1 + (2 + (3 + 4)));
}

template <typename F, typename... TArgs>
auto call_for_all(F f, TArgs... args) // call_for_all(f, 1, 2, 3, 4)
{
	(..., f(args)); // f(1), f(2), f(3), f(4); // fold expression for , operator
}

TEST_CASE("fold expressions")
{
	CHECK(sum(1, 2, 3, 4) == 10);

	call_for_all([](const auto& item) { std::cout << item << "\n"; }, 52, 55, 23);

	auto print = [](const auto& item) { std::cout << item << "\n"; };

	//index_demo_impl(std::tuple{"t1", "t2"}, print, IndexSequence<0, 1>{});

	tuple_for_each(std::tuple{"t1", "t2"}, print);
}

TEST_CASE("Data & tuple_for_each")
{
	Data d1{1,"pi"};

	auto print = [](const auto& item) { std::cout << item << "\n"; };
	tuple_for_each(d1.tied(), print);
}

template<typename Func>
auto benchmark(Func test_func, int iterations)
{
	const auto start = std::chrono::system_clock::now();
	while (iterations-- > 0)
		test_func();
	const auto stop = std::chrono::system_clock::now();
	const auto secs = std::chrono::duration<double>(stop - start);
	return secs.count();
}

TEST_CASE("benchmark")
{
	constexpr int iterations{100};
	constexpr int total_nodes{20'000};

	auto default_std_alloc = [total_nodes]
		{
			std::list<int> list;
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

	auto default_pmr_alloc = [total_nodes]
		{
			std::pmr::list<int> list;
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

	auto pmr_alloc_no_buf = [total_nodes]
		{
			std::pmr::monotonic_buffer_resource mbr;
			std::pmr::polymorphic_allocator<int> pa{&mbr};
			std::pmr::list<int> list{pa};
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

	auto pmr_alloc_and_buf = [total_nodes]
		{
			std::array<std::byte, total_nodes * 32> buffer; // enough to fit in all nodes
			std::pmr::monotonic_buffer_resource mbr{buffer.data(), buffer.size()};
			std::pmr::polymorphic_allocator<int> pa{&mbr};
			std::pmr::list<int> list{pa};
			for (int i{}; i != total_nodes; ++i)
				list.push_back(i);
		};

	const double t1 = benchmark(default_std_alloc, iterations);
	const double t2 = benchmark(default_pmr_alloc, iterations);
	const double t3 = benchmark(pmr_alloc_no_buf, iterations);
	const double t4 = benchmark(pmr_alloc_and_buf, iterations);

	std::cout << std::fixed << std::setprecision(3)
		<< "t1 (default std alloc): " << t1 << " sec; t1/t1: " << t1 / t1 << '\n'
		<< "t2 (default pmr alloc): " << t2 << " sec; t1/t2: " << t1 / t2 << '\n'
		<< "t3 (pmr alloc  no buf): " << t3 << " sec; t1/t3: " << t1 / t3 << '\n'
		<< "t4 (pmr alloc and buf): " << t4 << " sec; t1/t4: " << t1 / t4 << '\n';
}