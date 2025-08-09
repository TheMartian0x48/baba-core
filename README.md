# Baba Core Library

A modern C++ core utilities library providing high-performance data structures, logging, and memory management utilities.

## Features

### 🧠 Core Utilities (`baba::core`)
- Aligned memory allocation and management
- Custom allocators for performance-critical applications

### 📝 Logging Framework (`baba::logger`)
- Modern C++20 logging with lambda-based lazy evaluation
- Asynchronous and synchronous logging modes
- Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
- Thread-safe operations with performance metrics

### 🗂️ Data Structures (`baba::ds`)
- **Aligned String**: High-performance string with configurable memory alignment
- **Stack**: Template-based aligned stack implementation
- **Queue**: Template-based aligned circular queue implementation

## Quick Start

### Building the Library

```bash
# Create build directory and configure
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
make -j4

# Run tests
./tests/baba_core_tests

# Run example
./examples/logger_example
```

### Basic Usage

```cpp
#include <baba/baba.hpp>

int main() {
    // Logging
    baba::logger::info([]() { return "Application started"; });
    baba::logger::error([]() { return "Something went wrong"; });
    
    // Data structures
    baba::ds::string str("Hello");
    str += " World!";
    
    baba::ds::stack<int> stack;
    stack.push(42);
    
    baba::ds::queue<std::string> queue;
    queue.push("message");
    
    // Core utilities
    void* aligned_ptr = baba::core::aligned_alloc(64, 16);
    baba::core::aligned_free(aligned_ptr);
    
    return 0;
}
```

## Project Structure

```
include/baba/
├── baba.hpp           # Master header (includes everything)
├── core.hpp           # Core utilities
├── logger.hpp         # Logging functionality  
├── ds.hpp             # Data structures
├── core/              # Core implementation headers
├── logger/            # Logger implementation headers
└── ds/                # Data structure implementation headers
```

## Requirements

- **C++20** or later
- **CMake 3.14** or later
- **GCC 10** or later (configured to use `/home/adityaku/dev/tool/gcc/10/bin/g++`)

## Installation

### System-wide Installation

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
sudo make install
```

### Using in Your Project

#### CMake Integration

```cmake
find_package(baba-core REQUIRED)
target_link_libraries(your_target baba-core::baba-core)
```

#### As Subdirectory

```cmake
add_subdirectory(path/to/baba-core)
target_link_libraries(your_target baba-core)
```

## Namespace Organization

The library is organized into three main namespaces:

- **`baba::core`**: Low-level utilities (memory management, system interfaces)
- **`baba::logger`**: Dedicated logging subsystem  
- **`baba::ds`**: High-performance data structures

This design provides clear separation of concerns and intuitive organization.

## Examples

See the `examples/` directory for complete usage examples demonstrating all three namespaces working together.

## Testing

The library includes comprehensive tests using Google Test:

```bash
cd build
make -j4
./tests/baba_core_tests
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes with appropriate tests
4. Ensure all tests pass: `make && ./tests/baba_core_tests`
5. Submit a pull request

## License

This project is licensed under the MIT License.

## Version

Current version: **1.0.0**
