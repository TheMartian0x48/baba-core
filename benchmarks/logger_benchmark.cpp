#include <benchmark/benchmark.h>
#include <baba/logger.hpp>
#include <filesystem>
#include <sstream>

using namespace baba::core;

// Logger benchmark fixture
class LoggerBenchmarkFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        test_dir = "benchmark_logger";
        std::filesystem::create_directory(test_dir);
        log_file = test_dir + "/benchmark.log";
        
        // Reset logger to known state
        Logger::instance().set_level(LogLevel::DEBUG);
        Logger::instance().set_console_output(false); // Disable console for benchmarks
        Logger::instance().set_file_output(log_file);
    }
    
    void TearDown(const ::benchmark::State& state) override {
        Logger::instance().set_file_output(""); // Close file
        Logger::instance().set_console_output(true); // Restore console
        std::filesystem::remove_all(test_dir);
    }
    
protected:
    std::string test_dir;
    std::string log_file;
};

// Basic logging benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_LogDebug)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().debug("Debug message for benchmarking");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_LogInfo)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("Info message for benchmarking");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_LogWarn)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().warn("Warning message for benchmarking");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_LogError)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().error("Error message for benchmarking");
    }
    Logger::instance().flush();
}

// Generic log method benchmark
BENCHMARK_F(LoggerBenchmarkFixture, BM_GenericLog)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().log(LogLevel::INFO, "Generic log message for benchmarking");
    }
    Logger::instance().flush();
}

// Message length benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_ShortMessage)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("Short");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_MediumMessage)(benchmark::State& state) {
    std::string medium_msg = "This is a medium length message for benchmarking logger performance with typical message sizes";
    for (auto _ : state) {
        Logger::instance().info(medium_msg);
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_LongMessage)(benchmark::State& state) {
    std::string long_msg(1000, 'A'); // 1000 character message
    for (auto _ : state) {
        Logger::instance().info(long_msg);
    }
    Logger::instance().flush();
}

// Log level filtering benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_FilteredOutDebug)(benchmark::State& state) {
    Logger::instance().set_level(LogLevel::INFO); // Debug messages will be filtered out
    
    for (auto _ : state) {
        Logger::instance().debug("This debug message should be filtered out");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_FilteredOutInfo)(benchmark::State& state) {
    Logger::instance().set_level(LogLevel::ERROR); // Info messages will be filtered out
    
    for (auto _ : state) {
        Logger::instance().info("This info message should be filtered out");
    }
    Logger::instance().flush();
}

// Console vs file output benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_ConsoleOutput)(benchmark::State& state) {
    Logger::instance().set_file_output(""); // Disable file output
    Logger::instance().set_console_output(true); // Enable console output
    
    for (auto _ : state) {
        Logger::instance().info("Console output message");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_FileOutput)(benchmark::State& state) {
    Logger::instance().set_console_output(false); // Disable console output
    Logger::instance().set_file_output(log_file); // Enable file output
    
    for (auto _ : state) {
        Logger::instance().info("File output message");
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_BothOutputs)(benchmark::State& state) {
    Logger::instance().set_console_output(true); // Enable console output
    Logger::instance().set_file_output(log_file); // Enable file output
    
    for (auto _ : state) {
        Logger::instance().info("Both outputs message");
    }
    Logger::instance().flush();
}

// Flush operation benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_FlushOperation)(benchmark::State& state) {
    // Log some messages first
    for (int i = 0; i < 100; ++i) {
        Logger::instance().info("Message " + std::to_string(i));
    }
    
    for (auto _ : state) {
        Logger::instance().flush();
    }
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_LogWithFlush)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("Message with immediate flush");
        Logger::instance().flush();
    }
}

// Concurrent logging simulation (single-threaded benchmark)
BENCHMARK_F(LoggerBenchmarkFixture, BM_MixedLogLevels)(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        switch (counter % 4) {
            case 0: Logger::instance().debug("Debug message " + std::to_string(counter)); break;
            case 1: Logger::instance().info("Info message " + std::to_string(counter)); break;
            case 2: Logger::instance().warn("Warning message " + std::to_string(counter)); break;
            case 3: Logger::instance().error("Error message " + std::to_string(counter)); break;
        }
        counter++;
    }
    Logger::instance().flush();
}

// Message formatting benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_MessageFormatting)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("Formatted message with timestamp and level");
    }
    Logger::instance().flush();
}

// Special characters and encoding benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_SpecialCharacters)(benchmark::State& state) {
    std::string special_msg = "Message with special chars: !@#$%^&*()[]{}|\\:;\"'<>,.?/~`";
    for (auto _ : state) {
        Logger::instance().info(special_msg);
    }
    Logger::instance().flush();
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_NewlineCharacters)(benchmark::State& state) {
    std::string multiline_msg = "Line 1\nLine 2\nLine 3\nLine 4";
    for (auto _ : state) {
        Logger::instance().info(multiline_msg);
    }
    Logger::instance().flush();
}

// Stress test benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_HighVolumeLogging)(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < 100; ++i) {
            Logger::instance().info("High volume message " + std::to_string(i));
        }
    }
    Logger::instance().flush();
    state.SetItemsProcessed(state.iterations() * 100);
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_RapidLogging)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("Rapid logging message");
    }
    // Don't flush until the end to test buffering
    Logger::instance().flush();
}

// Memory usage benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_LargeMessageMemory)(benchmark::State& state) {
    for (auto _ : state) {
        std::string large_msg(10000, 'M'); // 10KB message
        Logger::instance().info(large_msg);
        // Message goes out of scope, testing memory cleanup
    }
    Logger::instance().flush();
    state.SetBytesProcessed(state.iterations() * 10000);
}

// Comparison benchmarks with direct file I/O
static void BM_DirectFileWrite(benchmark::State& state) {
    std::string log_file = "direct_benchmark.log";
    
    for (auto _ : state) {
        std::ofstream file(log_file, std::ios::app);
        file << "[2023-01-01 12:00:00.000] [INFO] Direct file write message\n";
        file.close();
    }
    
    std::filesystem::remove(log_file);
}
BENCHMARK(BM_DirectFileWrite);

static void BM_DirectFileWriteWithFlush(benchmark::State& state) {
    std::string log_file = "direct_benchmark_flush.log";
    
    for (auto _ : state) {
        std::ofstream file(log_file, std::ios::app);
        file << "[2023-01-01 12:00:00.000] [INFO] Direct file write with flush\n";
        file.flush();
        file.close();
    }
    
    std::filesystem::remove(log_file);
}
BENCHMARK(BM_DirectFileWriteWithFlush);

// Logger configuration benchmarks
BENCHMARK_F(LoggerBenchmarkFixture, BM_SetLogLevel)(benchmark::State& state) {
    LogLevel levels[] = {LogLevel::DEBUG, LogLevel::INFO, LogLevel::WARN, LogLevel::ERROR};
    int counter = 0;
    
    for (auto _ : state) {
        Logger::instance().set_level(levels[counter % 4]);
        counter++;
    }
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_SetConsoleOutput)(benchmark::State& state) {
    bool toggle = true;
    for (auto _ : state) {
        Logger::instance().set_console_output(toggle);
        toggle = !toggle;
    }
}

BENCHMARK_F(LoggerBenchmarkFixture, BM_SetFileOutput)(benchmark::State& state) {
    std::string temp_file = test_dir + "/temp_benchmark.log";
    bool toggle = true;
    
    for (auto _ : state) {
        if (toggle) {
            Logger::instance().set_file_output(temp_file);
        } else {
            Logger::instance().set_file_output("");
        }
        toggle = !toggle;
    }
}

// Singleton access benchmark
BENCHMARK_F(LoggerBenchmarkFixture, BM_SingletonAccess)(benchmark::State& state) {
    for (auto _ : state) {
        Logger& logger = Logger::instance();
        benchmark::DoNotOptimize(&logger);
    }
}

// LogLevel to string conversion benchmark
static void BM_LogLevelToString(benchmark::State& state) {
    LogLevel levels[] = {LogLevel::DEBUG, LogLevel::INFO, LogLevel::WARN, LogLevel::ERROR};
    int counter = 0;
    
    for (auto _ : state) {
        std::string level_str = to_string(levels[counter % 4]);
        benchmark::DoNotOptimize(level_str);
        counter++;
    }
}
BENCHMARK(BM_LogLevelToString);

// Empty message benchmark
BENCHMARK_F(LoggerBenchmarkFixture, BM_EmptyMessage)(benchmark::State& state) {
    for (auto _ : state) {
        Logger::instance().info("");
    }
    Logger::instance().flush();
}

// Numeric message benchmark
BENCHMARK_F(LoggerBenchmarkFixture, BM_NumericMessage)(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        Logger::instance().info("Counter value: " + std::to_string(counter++));
    }
    Logger::instance().flush();
}

// File rotation simulation (multiple files)
BENCHMARK_F(LoggerBenchmarkFixture, BM_MultipleLogFiles)(benchmark::State& state) {
    int file_counter = 0;
    for (auto _ : state) {
        std::string current_log = test_dir + "/log_" + std::to_string(file_counter % 5) + ".log";
        Logger::instance().set_file_output(current_log);
        Logger::instance().info("Message to file " + std::to_string(file_counter % 5));
        file_counter++;
    }
    Logger::instance().flush();
}
