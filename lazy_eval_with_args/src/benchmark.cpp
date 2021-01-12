#include "make_memoized.h"
#include "lib/func.h"
#include <benchmark/benchmark.h>

static const int kFirstArg{50};
static const double kSecondArg{8.0};

static void BM_FuncMemoized_FirstCall(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto FuncMemoized{MakeMemoized(Func)};
        state.ResumeTiming();

        FuncMemoized(kFirstArg, kSecondArg);
    }
}
// Register the function as a benchmark



static void BM_FuncMemoized_SecondCall(benchmark::State& state)
{
   auto FuncMemoized{MakeMemoized(Func)};
    FuncMemoized(kFirstArg, kSecondArg);

    for (auto _ : state)
    {
        FuncMemoized(kFirstArg, kSecondArg);
    }
}

BENCHMARK(BM_FuncMemoized_FirstCall);
BENCHMARK(BM_FuncMemoized_SecondCall);
BENCHMARK_MAIN();
