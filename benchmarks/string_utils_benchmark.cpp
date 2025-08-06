#include <benchmark/benchmark.h>
#include <baba/string_utils.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace baba::core::string_utils;

// Benchmark data
static const std::string SAMPLE_TEXT = "  Hello, World! This is a sample text for benchmarking.  ";
static const std::string LONG_TEXT = std::string(10000, 'a') + std::string(10000, 'b') + std::string(10000, 'c');
static const std::string CSV_DATA = "apple,banana,cherry,date,elderberry,fig,grape,honeydew,kiwi,lemon";

// Trim benchmarks
static void BM_Trim(benchmark::State& state) {
    for (auto _ : state) {
        auto result = trim(SAMPLE_TEXT);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Trim);

static void BM_TrimLong(benchmark::State& state) {
    std::string padded_text = "   " + LONG_TEXT + "   ";
    for (auto _ : state) {
        auto result = trim(padded_text);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_TrimLong);

// Split benchmarks
static void BM_SplitChar(benchmark::State& state) {
    for (auto _ : state) {
        auto result = split(CSV_DATA, ',');
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SplitChar);

static void BM_SplitString(benchmark::State& state) {
    std::string data = "hello::world::test::data::more::items";
    for (auto _ : state) {
        auto result = split(data, "::");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SplitString);

static void BM_SplitLargeData(benchmark::State& state) {
    std::string large_csv;
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) large_csv += ",";
        large_csv += "item" + std::to_string(i);
    }
    
    for (auto _ : state) {
        auto result = split(large_csv, ',');
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SplitLargeData);

// Join benchmarks
static void BM_Join(benchmark::State& state) {
    std::vector<std::string> items = {"apple", "banana", "cherry", "date", "elderberry"};
    for (auto _ : state) {
        auto result = join(items, ",");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Join);

static void BM_JoinLarge(benchmark::State& state) {
    std::vector<std::string> items;
    for (int i = 0; i < 1000; ++i) {
        items.push_back("item" + std::to_string(i));
    }
    
    for (auto _ : state) {
        auto result = join(items, ",");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_JoinLarge);

// Case conversion benchmarks
static void BM_ToUpper(benchmark::State& state) {
    for (auto _ : state) {
        auto result = to_upper(SAMPLE_TEXT);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ToUpper);

static void BM_ToUpperLong(benchmark::State& state) {
    for (auto _ : state) {
        auto result = to_upper(LONG_TEXT);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ToUpperLong);

static void BM_ToLower(benchmark::State& state) {
    std::string upper_text = "HELLO, WORLD! THIS IS A SAMPLE TEXT FOR BENCHMARKING.";
    for (auto _ : state) {
        auto result = to_lower(upper_text);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ToLower);

// Pattern matching benchmarks
static void BM_StartsWith(benchmark::State& state) {
    for (auto _ : state) {
        bool result = starts_with(SAMPLE_TEXT, "  Hello");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_StartsWith);

static void BM_EndsWith(benchmark::State& state) {
    for (auto _ : state) {
        bool result = ends_with(SAMPLE_TEXT, "benchmarking.  ");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_EndsWith);

static void BM_Contains(benchmark::State& state) {
    for (auto _ : state) {
        bool result = contains(SAMPLE_TEXT, "sample text");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Contains);

// Replace benchmarks
static void BM_ReplaceAll(benchmark::State& state) {
    std::string text = "hello world hello universe hello galaxy";
    for (auto _ : state) {
        auto result = replace_all(text, "hello", "hi");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ReplaceAll);

static void BM_ReplaceAllLong(benchmark::State& state) {
    std::string text = LONG_TEXT + "target" + LONG_TEXT + "target" + LONG_TEXT;
    for (auto _ : state) {
        auto result = replace_all(text, "target", "replacement");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ReplaceAllLong);

// Format benchmark
static void BM_Format(benchmark::State& state) {
    for (auto _ : state) {
        auto result = format("Hello %s, number: %d, float: %.2f", "world", 42, 3.14159);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Format);

// Comparison with standard library alternatives
static void BM_StdTransformUpper(benchmark::State& state) {
    for (auto _ : state) {
        std::string result = SAMPLE_TEXT;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_StdTransformUpper);

static void BM_StdStringStreamSplit(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> result;
        std::stringstream ss(CSV_DATA);
        std::string item;
        while (std::getline(ss, item, ',')) {
            result.push_back(item);
        }
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_StdStringStreamSplit);

// Memory allocation benchmarks
static void BM_TrimNoAllocation(benchmark::State& state) {
    // Test case where no allocation should be needed
    std::string clean_text = "hello world";
    for (auto _ : state) {
        auto result = trim(clean_text);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_TrimNoAllocation);

// Edge case benchmarks
static void BM_SplitEmptyString(benchmark::State& state) {
    for (auto _ : state) {
        auto result = split("", ',');
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SplitEmptyString);

static void BM_ReplaceAllNoMatch(benchmark::State& state) {
    for (auto _ : state) {
        auto result = replace_all(SAMPLE_TEXT, "xyz", "abc");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ReplaceAllNoMatch);
