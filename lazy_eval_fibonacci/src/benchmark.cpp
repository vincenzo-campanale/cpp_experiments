#include "lib/plain_fibonacci.h"
#include "lib/cached_fibonacci.h"
#include "lib/efficient_cached_fibonacci.h"
#include <benchmark/benchmark.h>

static const unsigned int kValue{40};

static void BM_PlainFibonacci(benchmark::State& state)
{
    for (auto _ : state)
    {
        PlainFibonacci(kValue);
    }
}

BENCHMARK(BM_PlainFibonacci);

static void BM_CachedFibonacci(benchmark::State& state)
{
    for (auto _ : state)
    {
        CachedFibonacci(kValue);
    }
}

BENCHMARK(BM_CachedFibonacci);

static void BM_EfficientCachedFibonacci(benchmark::State& state)
{
    for (auto _ : state)
    {
        EfficientCachedFibonacci(kValue);
    }
}

BENCHMARK(BM_EfficientCachedFibonacci);

BENCHMARK_MAIN();
