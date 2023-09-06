#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <numeric>

using namespace std;

void calc_hits(const uintmax_t count, uintmax_t& hits)
{
    std::mt19937_64 rnd_gen(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_real_distribution<double> rnd(0, 1.0);

    uintmax_t local_hits = 0;
    for (uintmax_t n = 0; n < count; ++n)
    {
        double x = rnd(rnd_gen);
        double y = rnd(rnd_gen);

        if (x * x + y * y < 1)
            local_hits++;
    }

    hits += local_hits;
}

void calc_hits(const uintmax_t count, std::atomic<uintmax_t>& hits)
{
    std::mt19937_64 rnd_gen(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_real_distribution<double> rnd(0, 1.0);

    uintmax_t local_hits = 0;
    for (uintmax_t n = 0; n < count; ++n)
    {
        double x = rnd(rnd_gen);
        double y = rnd(rnd_gen);

        if (x * x + y * y < 1)
            local_hits++;
    }

    hits += local_hits;
}

int main()
{
    const uintmax_t N = 100'000'000;

    {
        //////////////////////////////////////////////////////////////////////////////
        // single thread

        cout << "Pi calculation started!" << endl;
        const auto start = chrono::high_resolution_clock::now();

        uintmax_t hits = 0;

        calc_hits(N, hits);

        const double pi = static_cast<double>(hits) / N * 4;

        const auto end = chrono::high_resolution_clock::now();
        const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Pi = " << pi << endl;
        cout << "Elapsed = " << elapsed_time << "ms" << endl;

        //////////////////////////////////////////////////////////////////////////////
    }

    {
        //////////////////////////////////////////////////////////////////////////////
        // single thread

        cout << "Pi calculation started!" << endl;
        const auto start = chrono::high_resolution_clock::now();

        uintmax_t hits = 0;

        size_t no_of_cores = std::thread::hardware_concurrency();
        size_t N_per_thread = N / no_of_cores;

        std::vector<std::thread> thds;
        std::vector<uintmax_t> partial_hits(no_of_cores);

        for (size_t i = 0; i < no_of_cores; ++i)
            thds.push_back(std::thread([&, i, N_per_thread] { calc_hits(N_per_thread, partial_hits[i]); }));

        for (auto& thd : thds)
            thd.join();

        hits = std::accumulate(partial_hits.begin(), partial_hits.end(), 0ULL);

        const double pi = static_cast<double>(hits) / N * 4;

        const auto end = chrono::high_resolution_clock::now();
        const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Pi = " << pi << endl;
        cout << "Elapsed = " << elapsed_time << "ms" << endl;

        //////////////////////////////////////////////////////////////////////////////
    }

    {
        //////////////////////////////////////////////////////////////////////////////
        // single thread

        cout << "Pi calculation started!" << endl;
        const auto start = chrono::high_resolution_clock::now();

        std::atomic<uintmax_t> hits = 0;

        size_t no_of_cores = std::thread::hardware_concurrency();
        size_t N_per_thread = N / no_of_cores;

        std::vector<std::thread> thds;

        for (size_t i = 0; i < no_of_cores; ++i)
            thds.push_back(std::thread([&, i, N_per_thread] { calc_hits(N_per_thread, hits); }));

        for (auto& thd : thds)
            thd.join();        

        const double pi = static_cast<double>(hits) / N * 4;

        const auto end = chrono::high_resolution_clock::now();
        const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Pi = " << pi << endl;
        cout << "Elapsed = " << elapsed_time << "ms" << endl;

        //////////////////////////////////////////////////////////////////////////////
    }
}