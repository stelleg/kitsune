//===- retref-bench.cc - XRay Instrumentation Benchmarks ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Define benchmarks for measuring the cost of XRay instrumentation (the sleds,
// the trampolines).
//
//===----------------------------------------------------------------------===//

#include <x86intrin.h>

#include "benchmark/benchmark.h"
#include "xray/xray_interface.h"

[[clang::xray_never_instrument]] __attribute__((noinline)) int
neverInstrumented() {
  benchmark::ClobberMemory();
  return 0;
}

[[clang::xray_never_instrument]] static void BM_ReturnNeverInstrumented(
    benchmark::State& state) {
  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(neverInstrumented());
  }
}

BENCHMARK(BM_ReturnNeverInstrumented);

[[clang::xray_always_instrument]] __attribute__((noinline)) int
alwaysInstrumented() {
  benchmark::ClobberMemory();
  return 0;
}

[[clang::xray_never_instrument]] static void BM_ReturnInstrumentedUnPatched(
    benchmark::State& state) {
  __xray_unpatch();
  while (state.KeepRunning()) {
    int x;
    benchmark::DoNotOptimize(x = alwaysInstrumented());
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_ReturnInstrumentedUnPatched);


[[clang::xray_never_instrument]] static void BM_ReturnInstrumentedPatchedThenUnpatched(
    benchmark::State& state) {
  __xray_patch();
  __xray_unpatch();
  while (state.KeepRunning()) {
    int x;
    benchmark::DoNotOptimize(x = alwaysInstrumented());
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_ReturnInstrumentedPatchedThenUnpatched);


[[clang::xray_never_instrument]] static void BM_ReturnInstrumentedPatched(
    benchmark::State& state) {
  __xray_patch();
  while (state.KeepRunning()) {
    int x;
    benchmark::DoNotOptimize(alwaysInstrumented());
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_ReturnInstrumentedPatched);

[[clang::xray_never_instrument]] static void BM_RDTSCP_Cost(
    benchmark::State& state) {
  while (state.KeepRunning()) {
    unsigned cpu;
    unsigned tsc;
    benchmark::DoNotOptimize(tsc = __rdtscp(&cpu));
    benchmark::ClobberMemory();
  }
}

volatile unsigned global_cpu;
volatile unsigned tsc;
[[clang::xray_never_instrument]] void benchmark_handler(int32_t,
                                                        XRayEntryType) {
  unsigned cpu;
  benchmark::DoNotOptimize(tsc = __rdtscp(&cpu));
  global_cpu = cpu;
  benchmark::ClobberMemory();
}

BENCHMARK(BM_RDTSCP_Cost);

[[clang::xray_never_instrument]] static void
BM_ReturnInstrumentedPatchedWithLogHandler(benchmark::State& state) {
  __xray_set_handler(benchmark_handler);
  __xray_patch();
  benchmark::ClobberMemory();
  while (state.KeepRunning()) {
    int x;
    benchmark::DoNotOptimize(x = alwaysInstrumented());
    benchmark::ClobberMemory();
  }
  __xray_remove_handler();
}

BENCHMARK(BM_ReturnInstrumentedPatchedWithLogHandler);

BENCHMARK_MAIN();
