#!/usr/bin/env bash

set -e

readonly PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly BUILD_DIR="${PROJECT_ROOT}/build"
readonly TEST_EXE="tests/baba_core_tests"
readonly BENCH_EXE="benchmarks/baba_core_benchmarks"
readonly EXAMPLE_EXE="examples/basic_usage"

readonly GREEN='\033[0;32m'
readonly RED='\033[0;31m'
readonly CYAN='\033[0;36m'
readonly YELLOW='\033[1;33m'
readonly NC='\033[0m' 

print_heading() {
    local msg="$1"
    echo -e "\n${RED}==========[ ${msg} ]==========${NC}"
}

print_step() {
    echo -e "${CYAN}==>${NC} ${GREEN}$1${NC}"
}

print_warning() {
    echo -e "${YELLOW}Warning:${NC} $1"
}


usage() {
    print_heading "Usage Information"
    echo "This script manages the build, test, benchmark, and documentation processes for Baba Core Library."
    echo
    echo "Usage: $0 [task]"
    echo
    echo "Available tasks:"
    echo "  configure_project       - Configures the project with CMake."
    echo "  build                   - Cleans and builds the project in Release mode."
    echo "  build_cache             - Builds using existing configuration (faster)."
    echo "  test                    - Builds the project and then runs all tests."
    echo "  test_cache              - Runs all tests using the last successful build."
    echo "  test_filter             - Runs tests with custom filter pattern."
    echo "  benchmark               - Builds the project and then runs all benchmarks."
    echo "  benchmark_cache         - Runs all benchmarks using the last successful build."
    echo "  benchmark_filter        - Runs benchmarks with custom filter pattern."
    echo "  examples                - Builds and runs examples."
    echo "  docs                    - Generates documentation with Doxygen."
    echo "  clean                   - Removes the build directory."
    echo "  help                    - Shows this help message."
    echo
    echo "If no task is provided, an interactive menu will be shown."
}

# Executes a target binary if it exists.
_execute_if_exists() {
    local exe_path="$1"
    local target_name="$2"
    local heading="$3"
    local args="${4:-}"

    if [[ -f "$exe_path" ]]; then
        print_heading "$heading"
        if [[ -n "$args" ]]; then
            "$exe_path" $args
        else
            "$exe_path"
        fi
        print_heading "$target_name run finished"
    else
        print_heading "$target_name executable not found"
        echo "Please build the project first by running: $0 build"
        print_heading "$target_name run aborted"
        exit 1
    fi
}

configure_project() {
    print_step "Configuring project with CMake..."
    cmake .. -DCMAKE_BUILD_TYPE=Release
}

build_project() {
    print_step "Building project..."
    cmake --build . --config Release
}

do_build() {
    print_heading "Build Started"
    print_step "Setting up build environment..."
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"

    pushd "$BUILD_DIR" > /dev/null
    configure_project
    build_project
    popd > /dev/null

    print_heading "Build Finished Successfully"
}

do_build_cache() {
    print_heading "Cache Build Started"
    if [[ -f "$BUILD_DIR/CMakeCache.txt" ]]; then
        pushd "$BUILD_DIR" > /dev/null
        build_project
        popd > /dev/null
        print_heading "Cache Build Finished Successfully"
    else
        print_heading "Cache Build Failed"
        echo -e "${RED}Error: Build directory is not configured. Please run: $0 configure_project${NC}"
        print_heading "Cache Build Aborted"
        exit 1
    fi
}

do_test() {
    do_build
    run_tests_cache 
}

run_tests_cache() {
    _execute_if_exists "${BUILD_DIR}/${TEST_EXE}" "Test" "Running Tests (from cache)"
}

run_tests_with_filter() {
    print_heading "Running Tests with Filter"
    
    if [[ ! -f "${BUILD_DIR}/${TEST_EXE}" ]]; then
        echo -e "${RED}Error: Test executable not found. Please build first.${NC}"
        exit 1
    fi
    
    echo -e "${CYAN}Enter test filter pattern (Google Test syntax):${NC}"
    echo "Examples:"
    echo "  *String*           - Run all tests containing 'String'"
    echo "  FileUtilsTest.*    - Run all tests in FileUtilsTest suite"
    echo "  *Logger*           - Run all tests containing 'Logger'"
    echo
    read -p "Filter pattern: " filter_pattern
    
    if [[ -z "$filter_pattern" ]]; then
        echo -e "${RED}Error: No filter pattern provided.${NC}"
        exit 1
    fi
    
    _execute_if_exists "${BUILD_DIR}/${TEST_EXE}" "Filtered Test" "Running Tests with Filter: $filter_pattern" "--gtest_filter=$filter_pattern"
}

run_benchmarks() {
    do_build
    run_benchmarks_cache 
}

run_benchmarks_cache() {
    _execute_if_exists "${BUILD_DIR}/${BENCH_EXE}" "Benchmark" "Running Benchmarks (from cache)"
}

run_benchmarks_with_filter() {
    print_heading "Running Benchmarks with Filter"
    
    if [[ ! -f "${BUILD_DIR}/${BENCH_EXE}" ]]; then
        echo -e "${RED}Error: Benchmark executable not found. Please build first.${NC}"
        exit 1
    fi
    
    echo -e "${CYAN}Enter benchmark filter pattern (Google Benchmark syntax):${NC}"
    echo "Examples:"
    echo "  .*String.*         - Run all benchmarks containing 'String'"
    echo "  .*File.*           - Run all benchmarks containing 'File'"
    echo "  BM_StringSplit.*   - Run specific benchmark patterns"
    echo
    read -p "Filter pattern: " filter_pattern
    
    if [[ -z "$filter_pattern" ]]; then
        echo -e "${RED}Error: No filter pattern provided.${NC}"
        exit 1
    fi
    
    _execute_if_exists "${BUILD_DIR}/${BENCH_EXE}" "Filtered Benchmark" "Running Benchmarks with Filter: $filter_pattern" "--benchmark_filter=$filter_pattern"
}

run_examples() {
    print_heading "Running Examples"
    
    if [[ ! -f "$BUILD_DIR/CMakeCache.txt" ]]; then
        print_step "Building project first..."
        do_build
    else
        print_step "Using existing build..."
        do_build_cache
    fi
    
    _execute_if_exists "${BUILD_DIR}/${EXAMPLE_EXE}" "Example" "Running Basic Usage Example"
}

generate_docs() {
    print_heading "Generating Documentation with Doxygen"
    
    if ! command -v doxygen &> /dev/null; then
        echo -e "${RED}Error: Doxygen not found. Please install Doxygen.${NC}"
        echo "On Ubuntu/Debian: sudo apt-get install doxygen"
        echo "On macOS: brew install doxygen"
        exit 1
    fi
    
    print_step "Creating Doxygen configuration..."
    
    # Create docs directory if it doesn't exist
    mkdir -p docs
    
    # Generate Doxyfile if it doesn't exist
    if [[ ! -f "Doxyfile" ]]; then
        print_step "Generating Doxyfile..."
        doxygen -g Doxyfile
        
        # Customize the Doxyfile for our project
        sed -i.bak \
            -e "s/PROJECT_NAME           = \"My Project\"/PROJECT_NAME           = \"Baba Core Library\"/" \
            -e "s/PROJECT_NUMBER         =/PROJECT_NUMBER         = 1.0.0/" \
            -e "s/PROJECT_BRIEF          =/PROJECT_BRIEF          = \"Core utilities library for C++\"/" \
            -e "s/OUTPUT_DIRECTORY       =/OUTPUT_DIRECTORY       = docs/" \
            -e "s/INPUT                  =/INPUT                  = include src/" \
            -e "s/RECURSIVE              = NO/RECURSIVE              = YES/" \
            -e "s/EXTRACT_ALL            = NO/EXTRACT_ALL            = YES/" \
            -e "s/EXTRACT_PRIVATE        = NO/EXTRACT_PRIVATE        = YES/" \
            -e "s/EXTRACT_STATIC         = NO/EXTRACT_STATIC         = YES/" \
            -e "s/GENERATE_LATEX         = YES/GENERATE_LATEX         = NO/" \
            -e "s/HAVE_DOT               = NO/HAVE_DOT               = YES/" \
            -e "s/CALL_GRAPH             = NO/CALL_GRAPH             = YES/" \
            -e "s/CALLER_GRAPH           = NO/CALLER_GRAPH           = YES/" \
            Doxyfile
        
        # Remove backup file
        rm -f Doxyfile.bak
        
        print_step "Doxyfile created and configured"
    else
        print_step "Using existing Doxyfile"
    fi
    
    print_step "Running Doxygen..."
    doxygen Doxyfile
    
    if [[ -f "docs/html/index.html" ]]; then
        print_heading "Documentation Generated Successfully"
        echo -e "${GREEN}Documentation available at: docs/html/index.html${NC}"
        echo -e "${GREEN}Open with: open docs/html/index.html${NC}"
    else
        print_heading "Documentation Generation Failed"
        echo -e "${RED}Error: Documentation generation failed${NC}"
        exit 1
    fi
}


handle_task() {
    case $1 in
        configure_project)      configure_only ;;
        build)                  do_build ;;
        build_cache)            do_build_cache ;;
        test)                   do_test ;;
        test_cache)             run_tests_cache ;;
        test_filter)            run_tests_with_filter ;;
        benchmark)              run_benchmarks ;;
        benchmark_cache)        run_benchmarks_cache ;;
        benchmark_filter)       run_benchmarks_with_filter ;;
        examples)               run_examples ;;
        docs)                   generate_docs ;;
        clean)                  do_clean ;;
        help)                   usage ;;
        *)
            echo -e "${RED}Unknown task: $1${NC}"
            usage
            exit 1
            ;;
    esac
}

configure_only() {
    print_heading "Configure Only"
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
    pushd "$BUILD_DIR" > /dev/null
    configure_project
    popd > /dev/null
    print_heading "Configure Finished"
}

do_clean() {
    print_heading "Cleaning Project"
    if [[ -d "$BUILD_DIR" ]]; then
        print_step "Removing build directory: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
    fi
    
    if [[ -d "docs" ]]; then
        print_step "Removing docs directory"
        rm -rf docs/
    fi
    
    if [[ -f "Doxyfile" ]]; then
        print_step "Removing Doxyfile"
        rm -f Doxyfile
    fi
    
    print_step "Clean complete."
    print_heading "Clean Finished"
}

choose_task() {
    print_heading "Interactive Task Runner"
    local tasks=(
        "configure_project" 
        "build" 
        "build_cache" 
        "test" 
        "test_cache" 
        "test_filter"
        "benchmark" 
        "benchmark_cache" 
        "benchmark_filter"
        "examples"
        "docs"
        "clean" 
        "help" 
        "quit"
    )

    echo -e "${CYAN}Please choose a task to run:${NC}"
    PS3="$(echo -e "${CYAN}Enter choice [1-${#tasks[@]}]: ${NC}")"

    select task in "${tasks[@]}"; do
        if [[ "$task" == "quit" ]]; then
            echo "Exiting."
            break
        elif [[ -n "$task" ]]; then
            handle_task "$task"
            break
        else
            echo -e "${RED}Invalid choice. Please try again.${NC}"
        fi
    done
}

main() {
    cd "$PROJECT_ROOT"

    if [[ $# -eq 0 ]]; then
        choose_task
    else
        handle_task "$1"
    fi
}

main "$@"
