#include <string>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>
#include <execution>

void background_work(int id, const std::string& text)
{
	std::cout << "background_work#" << id << " started..." << std::endl;

	for (const auto& c : text)
	{
		std::cout << "background_work#" << id << " - " << c << std::endl;

		std::random_device rd;
		auto interval = std::chrono::milliseconds(rd() % 1000 + 100);
		std::this_thread::sleep_for(interval);
	}

	std::cout << "background_work#" << id << " finished..." << std::endl;
}

void run_job(const std::string& text)
{
	std::thread thd{&background_work, 3, text};
	thd.detach();
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

int main()
{
	std::thread thd_empty;
	std::cout << thd_empty.get_id() << "\n";
	
	{
		std::thread thd_1{&background_work, 1, "THREAD1"};
		std::thread thd_2{[] { background_work(2, "THREAD2"); }};
		run_job("JOB!!!!!!!!!!!!!!!!!!!");


		std::vector<std::thread> thds;
		thds.push_back(std::move(thd_1));
		thds.push_back(std::move(thd_2));

		for(auto& thd : thds)
		{
			if (thd.joinable())
				thd.join();
		}
	}

	std::cout << "---------------------------------" << std::endl;

	std::vector<int> vec(1'000'000);

	std::random_device rd;
	std::mt19937_64 rnd_gen{rd()};
	std::uniform_int_distribution<> rnd_distr(0, 1'000'000);

	std::generate(vec.begin(), vec.end(), [&] { return rnd_distr(rnd_gen); });

	auto t1 = benchmark([vec = vec]() mutable { std::sort(vec.begin(), vec.end()) ; }, 1);
	auto t2 = benchmark([vec = vec]() mutable { std::sort(std::execution::par_unseq, vec.begin(), vec.end()); }, 1);

	std::transform(std::execution::par_unseq, 
		vec.begin(), vec.end(), vec.begin(), [](int pixel) {
			return (pixel < 10'000) ? 0 : pixel;
		});
	
	std::cout << "Time sort1: " << t1 << "\n";
	std::cout << "Time sort2: " << t2 << "\n";
}

