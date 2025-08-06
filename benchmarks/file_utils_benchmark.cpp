#include <benchmark/benchmark.h>
#include <baba/file_utils.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

using namespace baba::core::file_utils;

// Setup test directory and files for benchmarks
class FileUtilsBenchmarkFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        test_dir = "benchmark_file_utils";
        create_directory(test_dir);
        
        // Create test files of various sizes
        small_file = test_dir + "/small.txt";
        medium_file = test_dir + "/medium.txt";
        large_file = test_dir + "/large.txt";
        
        // Small file (1KB)
        std::string small_content(1024, 'A');
        write_file(small_file, small_content);
        
        // Medium file (100KB)
        std::string medium_content(100 * 1024, 'B');
        write_file(medium_file, medium_content);
        
        // Large file (1MB)
        std::string large_content(1024 * 1024, 'C');
        write_file(large_file, large_content);
        
        // Create directory with many files
        many_files_dir = test_dir + "/many_files";
        create_directory(many_files_dir);
        for (int i = 0; i < 100; ++i) {
            std::string file_path = many_files_dir + "/file_" + std::to_string(i) + ".txt";
            write_file(file_path, "Content of file " + std::to_string(i));
        }
    }
    
    void TearDown(const ::benchmark::State& state) override {
        remove_directory(test_dir);
    }
    
protected:
    std::string test_dir;
    std::string small_file;
    std::string medium_file;
    std::string large_file;
    std::string many_files_dir;
};

// File reading benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadSmallFile)(benchmark::State& state) {
    for (auto _ : state) {
        std::string content = read_file(small_file);
        benchmark::DoNotOptimize(content);
    }
    state.SetBytesProcessed(state.iterations() * 1024);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadMediumFile)(benchmark::State& state) {
    for (auto _ : state) {
        std::string content = read_file(medium_file);
        benchmark::DoNotOptimize(content);
    }
    state.SetBytesProcessed(state.iterations() * 100 * 1024);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadLargeFile)(benchmark::State& state) {
    for (auto _ : state) {
        std::string content = read_file(large_file);
        benchmark::DoNotOptimize(content);
    }
    state.SetBytesProcessed(state.iterations() * 1024 * 1024);
}

// File writing benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_WriteSmallFile)(benchmark::State& state) {
    std::string content(1024, 'X');
    int counter = 0;
    for (auto _ : state) {
        std::string file_path = test_dir + "/write_small_" + std::to_string(counter++) + ".txt";
        bool result = write_file(file_path, content);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(state.iterations() * 1024);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_WriteMediumFile)(benchmark::State& state) {
    std::string content(100 * 1024, 'Y');
    int counter = 0;
    for (auto _ : state) {
        std::string file_path = test_dir + "/write_medium_" + std::to_string(counter++) + ".txt";
        bool result = write_file(file_path, content);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(state.iterations() * 100 * 1024);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_WriteLargeFile)(benchmark::State& state) {
    std::string content(1024 * 1024, 'Z');
    int counter = 0;
    for (auto _ : state) {
        std::string file_path = test_dir + "/write_large_" + std::to_string(counter++) + ".txt";
        bool result = write_file(file_path, content);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(state.iterations() * 1024 * 1024);
}

// Line-based operations benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadLines)(benchmark::State& state) {
    // Create a file with many lines
    std::string lines_file = test_dir + "/lines.txt";
    std::vector<std::string> lines;
    for (int i = 0; i < 1000; ++i) {
        lines.push_back("This is line number " + std::to_string(i) + " with some content");
    }
    write_lines(lines_file, lines);
    
    for (auto _ : state) {
        std::vector<std::string> read_lines_result = read_lines(lines_file);
        benchmark::DoNotOptimize(read_lines_result);
    }
    state.SetItemsProcessed(state.iterations() * 1000);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_WriteLines)(benchmark::State& state) {
    std::vector<std::string> lines;
    for (int i = 0; i < 1000; ++i) {
        lines.push_back("Benchmark line " + std::to_string(i));
    }
    
    int counter = 0;
    for (auto _ : state) {
        std::string file_path = test_dir + "/write_lines_" + std::to_string(counter++) + ".txt";
        bool result = write_lines(file_path, lines);
        benchmark::DoNotOptimize(result);
    }
    state.SetItemsProcessed(state.iterations() * 1000);
}

// File existence checks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_FileExists)(benchmark::State& state) {
    for (auto _ : state) {
        bool exists = file_exists(small_file);
        benchmark::DoNotOptimize(exists);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_FileExistsNonExistent)(benchmark::State& state) {
    for (auto _ : state) {
        bool exists = file_exists("non_existent_file.txt");
        benchmark::DoNotOptimize(exists);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_DirectoryExists)(benchmark::State& state) {
    for (auto _ : state) {
        bool exists = directory_exists(test_dir);
        benchmark::DoNotOptimize(exists);
    }
}

// File operations benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_CopyFile)(benchmark::State& state) {
    int counter = 0;
    for (auto _ : state) {
        std::string dest_file = test_dir + "/copy_" + std::to_string(counter++) + ".txt";
        bool result = copy_file(small_file, dest_file);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_MoveFile)(benchmark::State& state) {
    state.PauseTiming();
    // Create files to move
    std::vector<std::string> files_to_move;
    for (int i = 0; i < state.max_iterations; ++i) {
        std::string src_file = test_dir + "/move_src_" + std::to_string(i) + ".txt";
        write_file(src_file, "Content to move");
        files_to_move.push_back(src_file);
    }
    state.ResumeTiming();
    
    int counter = 0;
    for (auto _ : state) {
        if (counter < files_to_move.size()) {
            std::string dest_file = test_dir + "/move_dest_" + std::to_string(counter) + ".txt";
            bool result = move_file(files_to_move[counter], dest_file);
            benchmark::DoNotOptimize(result);
            counter++;
        }
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_RemoveFile)(benchmark::State& state) {
    state.PauseTiming();
    // Create files to remove
    std::vector<std::string> files_to_remove;
    for (int i = 0; i < state.max_iterations; ++i) {
        std::string file_path = test_dir + "/remove_" + std::to_string(i) + ".txt";
        write_file(file_path, "Content to remove");
        files_to_remove.push_back(file_path);
    }
    state.ResumeTiming();
    
    int counter = 0;
    for (auto _ : state) {
        if (counter < files_to_remove.size()) {
            bool result = remove_file(files_to_remove[counter]);
            benchmark::DoNotOptimize(result);
            counter++;
        }
    }
}

// File information benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_GetFileSize)(benchmark::State& state) {
    for (auto _ : state) {
        std::size_t size = get_file_size(medium_file);
        benchmark::DoNotOptimize(size);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_GetExtension)(benchmark::State& state) {
    std::string file_path = "/path/to/file.txt";
    for (auto _ : state) {
        std::string ext = get_extension(file_path);
        benchmark::DoNotOptimize(ext);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_GetFilename)(benchmark::State& state) {
    std::string file_path = "/path/to/file.txt";
    for (auto _ : state) {
        std::string filename = get_filename(file_path);
        benchmark::DoNotOptimize(filename);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_GetDirectory)(benchmark::State& state) {
    std::string file_path = "/path/to/file.txt";
    for (auto _ : state) {
        std::string dir = get_directory(file_path);
        benchmark::DoNotOptimize(dir);
    }
}

// Directory listing benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ListFiles)(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> files = list_files(many_files_dir, false);
        benchmark::DoNotOptimize(files);
    }
    state.SetItemsProcessed(state.iterations() * 100);
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ListFilesRecursive)(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> files = list_files(test_dir, true);
        benchmark::DoNotOptimize(files);
    }
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ListDirectories)(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> dirs = list_directories(test_dir, false);
        benchmark::DoNotOptimize(dirs);
    }
}

// Path operations benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_GetAbsolutePath)(benchmark::State& state) {
    std::string relative_path = "relative/path/to/file.txt";
    for (auto _ : state) {
        std::string abs_path = get_absolute_path(relative_path);
        benchmark::DoNotOptimize(abs_path);
    }
}

// Append operations benchmark
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_AppendFile)(benchmark::State& state) {
    std::string append_file_path = test_dir + "/append_test.txt";
    write_file(append_file_path, "Initial content\n");
    
    for (auto _ : state) {
        bool result = append_file(append_file_path, "Appended line\n");
        benchmark::DoNotOptimize(result);
    }
}

// Comparison with standard library operations
static void BM_StdReadFile(benchmark::State& state) {
    std::string test_file = "benchmark_std_test.txt";
    std::string content(1024, 'S');
    std::ofstream out(test_file);
    out << content;
    out.close();
    
    for (auto _ : state) {
        std::ifstream file(test_file);
        std::string read_content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        benchmark::DoNotOptimize(read_content);
    }
    
    remove_file(test_file);
    state.SetBytesProcessed(state.iterations() * 1024);
}
BENCHMARK(BM_StdReadFile);

static void BM_StdWriteFile(benchmark::State& state) {
    std::string content(1024, 'S');
    int counter = 0;
    
    for (auto _ : state) {
        std::string file_path = "benchmark_std_write_" + std::to_string(counter++) + ".txt";
        std::ofstream file(file_path);
        file << content;
        file.close();
        benchmark::DoNotOptimize(file.good());
    }
    
    // Cleanup
    for (int i = 0; i < counter; ++i) {
        std::string file_path = "benchmark_std_write_" + std::to_string(i) + ".txt";
        remove_file(file_path);
    }
    
    state.SetBytesProcessed(state.iterations() * 1024);
}
BENCHMARK(BM_StdWriteFile);

// Stress test benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ManySmallFiles)(benchmark::State& state) {
    std::string content = "Small file content";
    int counter = 0;
    
    for (auto _ : state) {
        std::string file_path = test_dir + "/many_small_" + std::to_string(counter++) + ".txt";
        bool result = write_file(file_path, content);
        benchmark::DoNotOptimize(result);
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadManySmallFiles)(benchmark::State& state) {
    state.PauseTiming();
    // Create many small files
    std::vector<std::string> small_files;
    for (int i = 0; i < 100; ++i) {
        std::string file_path = test_dir + "/read_many_" + std::to_string(i) + ".txt";
        write_file(file_path, "Content " + std::to_string(i));
        small_files.push_back(file_path);
    }
    state.ResumeTiming();
    
    for (auto _ : state) {
        for (const auto& file_path : small_files) {
            std::string content = read_file(file_path);
            benchmark::DoNotOptimize(content);
        }
    }
    state.SetItemsProcessed(state.iterations() * 100);
}

// Memory usage benchmarks
BENCHMARK_F(FileUtilsBenchmarkFixture, BM_ReadFileMemoryUsage)(benchmark::State& state) {
    // Test memory efficiency by reading the same large file multiple times
    for (auto _ : state) {
        std::string content = read_file(large_file);
        benchmark::DoNotOptimize(content);
        // Content goes out of scope, testing memory cleanup
    }
    state.SetBytesProcessed(state.iterations() * 1024 * 1024);
}
