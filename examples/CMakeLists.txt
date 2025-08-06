# List of all example executables
set(EXAMPLES
    basic_usage
    string_utils_example
    file_utils_example
    time_utils_example
    logger_example
    integration_example
)

# Create executables for all examples
foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    
    # Link with the baba-core library
    target_link_libraries(${EXAMPLE} PRIVATE baba-core)
    
    # Set C++ standard for the example
    set_target_properties(${EXAMPLE} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endforeach()

# Add custom target to build all examples
add_custom_target(build_examples
    DEPENDS ${EXAMPLES}
    COMMENT "Building all examples"
)

# Add individual run targets for each example
foreach(EXAMPLE ${EXAMPLES})
    add_custom_target(run_${EXAMPLE}
        COMMAND ${EXAMPLE}
        DEPENDS ${EXAMPLE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running ${EXAMPLE}"
    )
endforeach()

# Add a target to run all examples
add_custom_target(run_all_examples
    COMMENT "Running all examples"
)

foreach(EXAMPLE ${EXAMPLES})
    add_dependencies(run_all_examples run_${EXAMPLE})
endforeach()
