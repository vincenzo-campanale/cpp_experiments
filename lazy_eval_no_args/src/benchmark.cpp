#include "lib/lazy_eval.h"
#include "lib/sample_function.h"
#include <benchmark/benchmark.h>

static void BM_Function(benchmark::State& state)
{
    for (auto _ : state)
    {
        SampleFunction();
    }
}

BENCHMARK(BM_Function);

static void BM_LazyFunctionFirstCall(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto LazySampleFunction{MakeLazy(SampleFunction)};
        state.ResumeTiming();

        LazySampleFunction();
    }
}

BENCHMARK(BM_LazyFunctionFirstCall);

static void BM_LazyFunctionSuccessiveCalls(benchmark::State& state)
{
    auto LazySampleFunction{MakeLazy(SampleFunction)};
    LazySampleFunction();

    for (auto _ : state)
    {
        LazySampleFunction();
    }
}

BENCHMARK(BM_LazyFunctionSuccessiveCalls);

BENCHMARK_MAIN();
