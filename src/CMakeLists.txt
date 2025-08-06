# Add source files to the library target
target_sources(baba-core
    PRIVATE
        string_utils.cpp
        file_utils.cpp
        logger.cpp
        time_utils.cpp
)

# Link with required system libraries
if(WIN32)
    # Windows-specific libraries
    target_link_libraries(baba-core PRIVATE)
else()
    # Unix-like systems
    target_link_libraries(baba-core PRIVATE pthread stdc++fs)
endif()
