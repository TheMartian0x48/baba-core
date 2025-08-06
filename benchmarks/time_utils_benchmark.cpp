#include <benchmark/benchmark.h>
#include <baba/time_utils.hpp>
#include <thread>
#include <chrono>
#include <vector>

using namespace baba::core::time_utils;

// Basic time function benchmarks
static void BM_Now(benchmark::State& state) {
    for (auto _ : state) {
        TimePoint time = now();
        benchmark::DoNotOptimize(time);
    }
}
BENCHMARK(BM_Now);

static void BM_NowMillis(benchmark::State& state) {
    for (auto _ : state) {
        std::int64_t millis = now_millis();
        benchmark::DoNotOptimize(millis);
    }
}
BENCHMARK(BM_NowMillis);

static void BM_NowSeconds(benchmark::State& state) {
    for (auto _ : state) {
        std::int64_t seconds = now_seconds();
        benchmark::DoNotOptimize(seconds);
    }
}
BENCHMARK(BM_NowSeconds);

// Time formatting benchmarks
static void BM_FormatTime(benchmark::State& state) {
    TimePoint test_time = now();
    std::string format = "%Y-%m-%d %H:%M:%S";
    
    for (auto _ : state) {
        std::string formatted = format_time(test_time, format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatTime);

static void BM_FormatNow(benchmark::State& state) {
    std::string format = "%Y-%m-%d %H:%M:%S";
    
    for (auto _ : state) {
        std::string formatted = format_now(format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatNow);

static void BM_FormatTimeSimple(benchmark::State& state) {
    TimePoint test_time = now();
    std::string format = "%H:%M:%S";
    
    for (auto _ : state) {
        std::string formatted = format_time(test_time, format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatTimeSimple);

static void BM_FormatTimeComplex(benchmark::State& state) {
    TimePoint test_time = now();
    std::string format = "%A, %B %d, %Y at %I:%M:%S %p %Z";
    
    for (auto _ : state) {
        std::string formatted = format_time(test_time, format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatTimeComplex);

// ISO date parsing benchmarks
static void BM_ParseIsoDateValid(benchmark::State& state) {
    std::string iso_string = "2023-12-25 15:30:45";
    
    for (auto _ : state) {
        TimePoint parsed = parse_iso_date(iso_string);
        benchmark::DoNotOptimize(parsed);
    }
}
BENCHMARK(BM_ParseIsoDateValid);

static void BM_ParseIsoDateInvalid(benchmark::State& state) {
    std::string invalid_string = "invalid-date-format";
    
    for (auto _ : state) {
        TimePoint parsed = parse_iso_date(invalid_string);
        benchmark::DoNotOptimize(parsed);
    }
}
BENCHMARK(BM_ParseIsoDateInvalid);

// Duration operation benchmarks
static void BM_TimeDiff(benchmark::State& state) {
    TimePoint time1 = now();
    TimePoint time2 = time1 + std::chrono::milliseconds(100);
    
    for (auto _ : state) {
        Duration diff = time_diff(time1, time2);
        benchmark::DoNotOptimize(diff);
    }
}
BENCHMARK(BM_TimeDiff);

static void BM_ToMillis(benchmark::State& state) {
    Duration duration = std::chrono::milliseconds(1500);
    
    for (auto _ : state) {
        std::int64_t millis = to_millis(duration);
        benchmark::DoNotOptimize(millis);
    }
}
BENCHMARK(BM_ToMillis);

static void BM_ToSeconds(benchmark::State& state) {
    Duration duration = std::chrono::milliseconds(2500);
    
    for (auto _ : state) {
        double seconds = to_seconds(duration);
        benchmark::DoNotOptimize(seconds);
    }
}
BENCHMARK(BM_ToSeconds);

static void BM_ToMinutes(benchmark::State& state) {
    Duration duration = std::chrono::seconds(150);
    
    for (auto _ : state) {
        double minutes = to_minutes(duration);
        benchmark::DoNotOptimize(minutes);
    }
}
BENCHMARK(BM_ToMinutes);

static void BM_ToHours(benchmark::State& state) {
    Duration duration = std::chrono::minutes(150);
    
    for (auto _ : state) {
        double hours = to_hours(duration);
        benchmark::DoNotOptimize(hours);
    }
}
BENCHMARK(BM_ToHours);

// Timer class benchmarks
static void BM_TimerCreation(benchmark::State& state) {
    for (auto _ : state) {
        Timer timer;
        benchmark::DoNotOptimize(&timer);
    }
}
BENCHMARK(BM_TimerCreation);

static void BM_TimerElapsed(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        Duration elapsed = timer.elapsed();
        benchmark::DoNotOptimize(elapsed);
    }
}
BENCHMARK(BM_TimerElapsed);

static void BM_TimerElapsedMillis(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        std::int64_t millis = timer.elapsed_millis();
        benchmark::DoNotOptimize(millis);
    }
}
BENCHMARK(BM_TimerElapsedMillis);

static void BM_TimerElapsedSeconds(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        double seconds = timer.elapsed_seconds();
        benchmark::DoNotOptimize(seconds);
    }
}
BENCHMARK(BM_TimerElapsedSeconds);

static void BM_TimerElapsedMinutes(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        double minutes = timer.elapsed_minutes();
        benchmark::DoNotOptimize(minutes);
    }
}
BENCHMARK(BM_TimerElapsedMinutes);

static void BM_TimerReset(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        timer.reset();
    }
}
BENCHMARK(BM_TimerReset);

// ScopedTimer benchmarks
static void BM_ScopedTimerCreation(benchmark::State& state) {
    for (auto _ : state) {
        ScopedTimer timer("Benchmark Operation");
        benchmark::DoNotOptimize(&timer);
        // Timer destructor is called here
    }
}
BENCHMARK(BM_ScopedTimerCreation);

static void BM_ScopedTimerElapsed(benchmark::State& state) {
    ScopedTimer timer("Benchmark Operation");
    
    for (auto _ : state) {
        Duration elapsed = timer.elapsed();
        benchmark::DoNotOptimize(elapsed);
    }
}
BENCHMARK(BM_ScopedTimerElapsed);

// Sleep function benchmarks (these will actually sleep, so use small durations)
static void BM_SleepMillis(benchmark::State& state) {
    for (auto _ : state) {
        sleep_millis(1); // 1ms sleep
    }
}
BENCHMARK(BM_SleepMillis);

static void BM_SleepSeconds(benchmark::State& state) {
    for (auto _ : state) {
        sleep_seconds(0.001); // 1ms sleep
    }
}
BENCHMARK(BM_SleepSeconds);

// Multiple timer benchmarks
static void BM_MultipleTimers(benchmark::State& state) {
    for (auto _ : state) {
        Timer timer1;
        Timer timer2;
        Timer timer3;
        
        std::int64_t elapsed1 = timer1.elapsed_millis();
        std::int64_t elapsed2 = timer2.elapsed_millis();
        std::int64_t elapsed3 = timer3.elapsed_millis();
        
        benchmark::DoNotOptimize(elapsed1);
        benchmark::DoNotOptimize(elapsed2);
        benchmark::DoNotOptimize(elapsed3);
    }
}
BENCHMARK(BM_MultipleTimers);

// Time measurement precision benchmarks
static void BM_TimeMeasurementPrecision(benchmark::State& state) {
    for (auto _ : state) {
        TimePoint start = now();
        // Simulate very short operation
        volatile int dummy = 0;
        for (int i = 0; i < 100; ++i) {
            dummy += i;
        }
        TimePoint end = now();
        
        Duration elapsed = time_diff(start, end);
        benchmark::DoNotOptimize(elapsed);
        benchmark::DoNotOptimize(dummy);
    }
}
BENCHMARK(BM_TimeMeasurementPrecision);

// Stress test benchmarks
static void BM_HighFrequencyTiming(benchmark::State& state) {
    Timer timer;
    
    for (auto _ : state) {
        for (int i = 0; i < 1000; ++i) {
            std::int64_t elapsed = timer.elapsed_millis();
            benchmark::DoNotOptimize(elapsed);
        }
    }
    state.SetItemsProcessed(state.iterations() * 1000);
}
BENCHMARK(BM_HighFrequencyTiming);

static void BM_ManyTimerCreations(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Timer> timers;
        timers.reserve(100);
        
        for (int i = 0; i < 100; ++i) {
            timers.emplace_back();
        }
        
        benchmark::DoNotOptimize(timers);
    }
    state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_ManyTimerCreations);

// Duration conversion chain benchmark
static void BM_DurationConversionChain(benchmark::State& state) {
    Duration duration = std::chrono::milliseconds(123456);
    
    for (auto _ : state) {
        std::int64_t millis = to_millis(duration);
        double seconds = to_seconds(duration);
        double minutes = to_minutes(duration);
        double hours = to_hours(duration);
        
        benchmark::DoNotOptimize(millis);
        benchmark::DoNotOptimize(seconds);
        benchmark::DoNotOptimize(minutes);
        benchmark::DoNotOptimize(hours);
    }
}
BENCHMARK(BM_DurationConversionChain);

// Time arithmetic benchmarks
static void BM_TimeArithmetic(benchmark::State& state) {
    TimePoint base_time = now();
    Duration offset = std::chrono::milliseconds(100);
    
    for (auto _ : state) {
        TimePoint future = base_time + offset;
        TimePoint past = base_time - offset;
        Duration diff = future - past;
        
        benchmark::DoNotOptimize(future);
        benchmark::DoNotOptimize(past);
        benchmark::DoNotOptimize(diff);
    }
}
BENCHMARK(BM_TimeArithmetic);

// Comparison with standard library
static void BM_StdChronoNow(benchmark::State& state) {
    for (auto _ : state) {
        auto time = std::chrono::steady_clock::now();
        benchmark::DoNotOptimize(time);
    }
}
BENCHMARK(BM_StdChronoNow);

static void BM_StdChronoSystemNow(benchmark::State& state) {
    for (auto _ : state) {
        auto time = std::chrono::system_clock::now();
        benchmark::DoNotOptimize(time);
    }
}
BENCHMARK(BM_StdChronoSystemNow);

static void BM_StdChronoDurationCast(benchmark::State& state) {
    auto duration = std::chrono::milliseconds(1500);
    
    for (auto _ : state) {
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        benchmark::DoNotOptimize(millis);
    }
}
BENCHMARK(BM_StdChronoDurationCast);

// Edge case benchmarks
static void BM_ZeroDuration(benchmark::State& state) {
    Duration zero_duration = std::chrono::milliseconds(0);
    
    for (auto _ : state) {
        std::int64_t millis = to_millis(zero_duration);
        double seconds = to_seconds(zero_duration);
        benchmark::DoNotOptimize(millis);
        benchmark::DoNotOptimize(seconds);
    }
}
BENCHMARK(BM_ZeroDuration);

static void BM_LargeDuration(benchmark::State& state) {
    Duration large_duration = std::chrono::hours(24 * 365); // One year
    
    for (auto _ : state) {
        double hours = to_hours(large_duration);
        double minutes = to_minutes(large_duration);
        benchmark::DoNotOptimize(hours);
        benchmark::DoNotOptimize(minutes);
    }
}
BENCHMARK(BM_LargeDuration);

// Memory usage benchmarks
static void BM_TimerMemoryUsage(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Timer> timers(1000);
        
        for (auto& timer : timers) {
            std::int64_t elapsed = timer.elapsed_millis();
            benchmark::DoNotOptimize(elapsed);
        }
        
        benchmark::DoNotOptimize(timers);
    }
    state.SetItemsProcessed(state.iterations() * 1000);
}
BENCHMARK(BM_TimerMemoryUsage);

// Format string variations
static void BM_FormatTimeISO(benchmark::State& state) {
    TimePoint test_time = now();
    std::string format = "%Y-%m-%dT%H:%M:%S";
    
    for (auto _ : state) {
        std::string formatted = format_time(test_time, format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatTimeISO);

static void BM_FormatTimeCustom(benchmark::State& state) {
    TimePoint test_time = now();
    std::string format = "Date: %d/%m/%Y Time: %H:%M:%S";
    
    for (auto _ : state) {
        std::string formatted = format_time(test_time, format);
        benchmark::DoNotOptimize(formatted);
    }
}
BENCHMARK(BM_FormatTimeCustom);

// Concurrent timer simulation (single-threaded)
static void BM_ConcurrentTimerSimulation(benchmark::State& state) {
    std::vector<Timer> timers(10);
    
    for (auto _ : state) {
        for (auto& timer : timers) {
            std::int64_t elapsed = timer.elapsed_millis();
            benchmark::DoNotOptimize(elapsed);
        }
        
        // Reset some timers randomly
        if (state.iterations() % 5 == 0) {
            timers[state.iterations() % timers.size()].reset();
        }
    }
}
BENCHMARK(BM_ConcurrentTimerSimulation);
