cmake_minimum_required(VERSION 3.14)

project(baba-core
    VERSION 1.0.0
    DESCRIPTION "A comprehensive C++ core utilities library"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for better warnings and optimization
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

# Include GNUInstallDirs for standard installation directories
include(GNUInstallDirs)

# Option to build tests and benchmarks
option(BABA_CORE_BUILD_TESTS "Build tests" ON)
option(BABA_CORE_BUILD_BENCHMARKS "Build benchmarks" ON)
option(BABA_CORE_BUILD_EXAMPLES "Build examples" ON)

# Create the main library target
add_library(baba-core)

# Add alias for consistent naming when used as subdirectory
add_library(baba-core::baba-core ALIAS baba-core)

# Set target properties
set_target_properties(baba-core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME baba-core
)

# Add source files
add_subdirectory(src)

# Set include directories
target_include_directories(baba-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Enable testing if requested
if(BABA_CORE_BUILD_TESTS)
    enable_testing()
    
    # Download and configure Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # Make Google Test available
    FetchContent_MakeAvailable(googletest)
    
    # Add tests directory
    add_subdirectory(tests)
endif()

# Enable benchmarks if requested
if(BABA_CORE_BUILD_BENCHMARKS)
    # Download and configure Google Benchmark
    include(FetchContent)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    
    # Disable benchmark tests
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    
    # Make Google Benchmark available
    FetchContent_MakeAvailable(googlebenchmark)
    
    add_subdirectory(benchmarks)
endif()

# Enable examples if requested
if(BABA_CORE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation rules
install(TARGETS baba-core
    EXPORT baba-coreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

# Export targets
install(EXPORT baba-coreTargets
    FILE baba-coreTargets.cmake
    NAMESPACE baba-core::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/baba-core
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/baba-coreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/baba-coreConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/baba-core
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/baba-coreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/baba-coreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/baba-coreConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/baba-core
)
