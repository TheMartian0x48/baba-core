# Create the benchmark executables
add_executable(baba_core_benchmarks
    logger_benchmark.cpp
)

add_executable(array_benchmark
    benchmark.cpp
)

# Link with the baba-core library (and Google Benchmark for logger_benchmark)
target_link_libraries(baba_core_benchmarks
    PRIVATE
        baba-core
        benchmark::benchmark
        benchmark::benchmark_main
)

target_link_libraries(array_benchmark
    PRIVATE
        baba-core
)

# Set C++ standard for benchmarks
set_target_properties(baba_core_benchmarks PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
set_target_properties(array_benchmark PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add custom target to build all benchmarks
add_custom_target(build_benchmarks
    DEPENDS baba_core_benchmarks array_benchmark
    COMMENT "Building all benchmarks"
)

# Add custom target to run all benchmarks
add_custom_target(run_benchmarks
    COMMAND baba_core_benchmarks
    COMMAND array_benchmark
    DEPENDS baba_core_benchmarks array_benchmark
    COMMENT "Running all benchmarks"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
