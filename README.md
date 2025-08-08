# Baba Core Library

A comprehensive C++ core utilities library providing essential functionality for string manipulation, file I/O, logging, and time operations.

## Features

### 🔤 String Utilities
- String trimming, splitting, and joining
- Case conversion (upper/lower)
- Pattern matching (starts_with, ends_with, contains)
- String replacement and formatting

### 📁 File Utilities
- File and directory operations
- Path manipulation
- File reading/writing (text and binary)
- Directory listing and traversal

### 📝 Logging Framework
- Multiple log levels (DEBUG, INFO, WARN, ERROR)
- Console and file output
- Thread-safe logging
- Formatted logging support

### ⏰ Time Utilities
- Current timestamp operations
- Time formatting and parsing
- Duration calculations
- Benchmark timers (Timer, ScopedTimer)

## Quick Start

### Building the Library

```bash
# Configure and build
./build.sh build

# Run tests
./build.sh test

# Run benchmarks
./build.sh benchmark

# Run examples
./build.sh examples
```

### Basic Usage

```cpp
#include <baba/core.hpp>

int main() {
    // String utilities
    auto parts = baba::core::string_utils::split("hello,world", ",");
    std::string upper = baba::core::string_utils::to_upper("hello");
    
    // File utilities
    baba::core::file_utils::write_file("test.txt", "Hello World");
    std::string content = baba::core::file_utils::read_file("test.txt");
    
    // Logging
    baba::core::Logger::info("Application started");
    baba::core::Logger::error("Something went wrong");
    
    // Time utilities
    auto timer = baba::core::time_utils::Timer();
    // ... do some work ...
    auto elapsed = timer.elapsed_seconds();
    
    return 0;
}
```

## Build System

The library uses CMake and includes:
- **Google Test** for unit testing
- **Google Benchmark** for performance testing
- **Doxygen/Sphinx** support for documentation

### Available Build Tasks

| Task | Description |
|------|-------------|
| `build` | Clean build in Release mode |
| `build_cache` | Incremental build |
| `test` | Run all unit tests |
| `test_filter` | Run filtered tests |
| `benchmark` | Run all benchmarks |
| `benchmark_filter` | Run filtered benchmarks |
| `examples` | Build and run examples |
| `format` | Format code with clang-format |
| `docs` | Generate documentation |
| `clean` | Clean build artifacts |

## Requirements

- **C++17** or later
- **CMake 3.14** or later
- **GCC/Clang** with C++17 support

## Installation

```bash
# Build and install system-wide
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

## Using in Your Project

### CMake Integration

```cmake
find_package(baba-core REQUIRED)
target_link_libraries(your_target baba-core::baba-core)
```

### Manual Integration

```cmake
add_subdirectory(path/to/baba-core)
target_link_libraries(your_target baba-core)
```

## Documentation

Generate documentation with:

```bash
# Auto-detect and generate docs
./build.sh docs

# Generate with Doxygen
./build.sh docs_doxygen

# Generate with Sphinx
./build.sh docs_sphinx
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Run the test suite: `./build.sh test`
6. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Version

Current version: **1.0.0**

## API Reference

### String Utils (`baba::core::string_utils`)
- `trim()`, `ltrim()`, `rtrim()` - String trimming
- `split()`, `join()` - String splitting and joining
- `to_upper()`, `to_lower()` - Case conversion
- `starts_with()`, `ends_with()`, `contains()` - Pattern matching
- `replace_all()` - String replacement
- `format()` - String formatting

### File Utils (`baba::core::file_utils`)
- `read_file()`, `write_file()` - File I/O
- `file_exists()`, `directory_exists()` - Existence checks
- `create_directory()`, `remove_file()` - File operations
- `list_files()`, `list_directories()` - Directory listing
- `get_extension()`, `get_filename()` - Path utilities

### Logger (`baba::core::Logger`)
- `debug()`, `info()`, `warn()`, `error()` - Logging methods
- `set_level()`, `set_console_output()` - Configuration
- `set_file_output()` - File logging
- Template methods for formatted logging

### Time Utils (`baba::core::time_utils`)
- `now()`, `now_millis()` - Current time
- `format_time()`, `parse_iso_date()` - Time formatting
- `Timer`, `ScopedTimer` - Benchmark timers
- `sleep_millis()`, `sleep_seconds()` - Sleep functions




