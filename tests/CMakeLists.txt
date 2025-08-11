# Create the test executable
add_executable(baba_core_tests
    logger_test.cpp
)

# Link with the baba-core library and Google Test
target_link_libraries(baba_core_tests
    PRIVATE
        baba-core
        gtest
        gtest_main
        gmock
        gmock_main
)

# Set C++ standard for tests
set_target_properties(baba_core_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(baba_core_tests)

# Add subdirectories following namespace structure
add_subdirectory(ds)

# Add custom target to build all tests
add_custom_target(build_tests
    DEPENDS baba_core_tests array_tests
    COMMENT "Building all tests"
)

# Add custom target to run tests
add_custom_target(run_tests
    COMMAND baba_core_tests
    DEPENDS baba_core_tests
    COMMENT "Running all tests"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
