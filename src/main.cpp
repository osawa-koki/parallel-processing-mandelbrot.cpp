#include <benchmark/benchmark.h>

#include "function.hpp"

static void BM_ExecSingle(benchmark::State& state) {
  for (auto _ : state)
    exec_single();
}
BENCHMARK(BM_ExecSingle);

BENCHMARK_MAIN();
