# Create the benchmark executable
add_executable(baba_core_benchmarks
    string_utils_benchmark.cpp
    file_utils_benchmark.cpp
    logger_benchmark.cpp
    time_utils_benchmark.cpp
)

# Link with the baba-core library and Google Benchmark
target_link_libraries(baba_core_benchmarks
    PRIVATE
        baba-core
        benchmark::benchmark
        benchmark::benchmark_main
)

# Set C++ standard for benchmarks
set_target_properties(baba_core_benchmarks PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add custom target to build all benchmarks
add_custom_target(build_benchmarks
    DEPENDS baba_core_benchmarks
    COMMENT "Building all benchmarks"
)

# Add custom target to run benchmarks
add_custom_target(run_benchmarks
    COMMAND baba_core_benchmarks
    DEPENDS baba_core_benchmarks
    COMMENT "Running all benchmarks"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
