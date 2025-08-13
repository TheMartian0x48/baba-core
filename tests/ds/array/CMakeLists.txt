# Array tests - separate files for each type
add_executable(array_bool_test
    array_bool_test.cpp
)

target_link_libraries(array_bool_test
    baba-core
    gtest
    gtest_main
)

add_executable(array_i32_test
    array_i32_test.cpp
)

target_link_libraries(array_i32_test
    baba-core
    gtest
    gtest_main
)

add_executable(array_f32_test
    array_f32_test.cpp
)

target_link_libraries(array_f32_test
    baba-core
    gtest
    gtest_main
)

add_executable(array_struct_test
    array_struct_test.cpp
)

target_link_libraries(array_struct_test
    baba-core
    gtest
    gtest_main
)

# Add to test suite
add_test(NAME ArrayBoolTests COMMAND array_bool_test)
add_test(NAME ArrayI32Tests COMMAND array_i32_test)
add_test(NAME ArrayF32Tests COMMAND array_f32_test)
add_test(NAME ArrayStructTests COMMAND array_struct_test)
