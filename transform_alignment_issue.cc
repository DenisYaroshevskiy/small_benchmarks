/*
  A prime example of some weird alignment issue.
  Only reproduced when passing mavx2.
  (Maybe data is not aligned?)

  if SLOWER_TIMINGS is 1 - only run 2 copies of the benchmark
                    is 0 - run 3

  On my machine:

------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
double_elements_0        105 ns          105 ns      6617708
double_elements_1        105 ns          105 ns      6664185

Vs:

------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
double_elements_0       64.6 ns         64.6 ns     10867663
double_elements_1       64.5 ns         64.5 ns     10855206
double_elements_2       64.5 ns         64.5 ns     10868602


==============
If I increase the size to  100'000 / sizeof(T) then

------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
double_elements_0       1073 ns         1073 ns       651407
double_elements_1       1078 ns         1078 ns       654136

------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
double_elements_0       1369 ns         1369 ns       492165
double_elements_1       1358 ns         1358 ns       515861
double_elements_2       1357 ns         1357 ns       514764

*/

#include <benchmark/benchmark.h>
#include <numeric>
#include <vector>

#define INLINE __attribute__((always_inline)) inline
#define NOINLINE __attribute__((noinline))

#define SLOWER_TIMINGS 1

namespace
{

using T = int;
constexpr std::size_t size = 10'000 / sizeof(T);

NOINLINE std::vector<T> const& data()
{
  static std::vector<T> res(size, T{2});
  return res;
}

INLINE void double_elements_bench(benchmark::State& state)
{
  auto v = data();

  for (auto _ : state) {
    for (T& x : v) x = x + x;
    benchmark::DoNotOptimize(v.data());
  }
}

void double_elements_0(benchmark::State& state)
{
  double_elements_bench(state);
}
BENCHMARK(double_elements_0);

void double_elements_1(benchmark::State& state)
{
  double_elements_bench(state);
}
BENCHMARK(double_elements_1);

#if (!SLOWER_TIMINGS)
void double_elements_2(benchmark::State& state)
{
  double_elements_bench(state);
}
BENCHMARK(double_elements_2);
#endif

}

BENCHMARK_MAIN();
