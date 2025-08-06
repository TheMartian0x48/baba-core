#include <gtest/gtest.h>
#include <baba/file_utils.hpp>
#include <filesystem>
#include <fstream>

using namespace baba::core::file_utils;

class FileUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test directory
        test_dir = "test_file_utils";
        create_directory(test_dir);
        
        // Create test file
        test_file = test_dir + "/test.txt";
        test_content = "Hello, World!\nThis is a test file.\nLine 3";
        
        write_file(test_file, test_content);
    }
    
    void TearDown() override {
        // Clean up test files and directory
        remove_directory(test_dir);
    }
    
    std::string test_dir;
    std::string test_file;
    std::string test_content;
};

// File reading tests
TEST_F(FileUtilsTest, ReadFileSuccess) {
    std::string content = read_file(test_file);
    EXPECT_EQ(content, test_content);
}

TEST_F(FileUtilsTest, ReadFileNonExistent) {
    std::string content = read_file("non_existent_file.txt");
    EXPECT_TRUE(content.empty());
}

TEST_F(FileUtilsTest, ReadLinesSuccess) {
    std::vector<std::string> lines = read_lines(test_file);
    EXPECT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "Hello, World!");
    EXPECT_EQ(lines[1], "This is a test file.");
    EXPECT_EQ(lines[2], "Line 3");
}

TEST_F(FileUtilsTest, ReadLinesNonExistent) {
    std::vector<std::string> lines = read_lines("non_existent_file.txt");
    EXPECT_TRUE(lines.empty());
}

// File writing tests
TEST_F(FileUtilsTest, WriteFileSuccess) {
    std::string new_file = test_dir + "/new_file.txt";
    std::string new_content = "New file content";
    
    bool result = write_file(new_file, new_content);
    EXPECT_TRUE(result);
    
    std::string read_content = read_file(new_file);
    EXPECT_EQ(read_content, new_content);
}

TEST_F(FileUtilsTest, WriteFileInvalidPath) {
    bool result = write_file("/invalid/path/file.txt", "content");
    EXPECT_FALSE(result);
}

TEST_F(FileUtilsTest, AppendFileSuccess) {
    std::string append_content = "\nAppended line";
    
    bool result = append_file(test_file, append_content);
    EXPECT_TRUE(result);
    
    std::string read_content = read_file(test_file);
    EXPECT_EQ(read_content, test_content + append_content);
}

TEST_F(FileUtilsTest, WriteLinesSuccess) {
    std::string new_file = test_dir + "/lines_file.txt";
    std::vector<std::string> lines = {"Line 1", "Line 2", "Line 3"};
    
    bool result = write_lines(new_file, lines);
    EXPECT_TRUE(result);
    
    std::vector<std::string> read_lines_result = read_lines(new_file);
    EXPECT_EQ(read_lines_result, lines);
}

// File existence tests
TEST_F(FileUtilsTest, FileExistsTrue) {
    EXPECT_TRUE(file_exists(test_file));
}

TEST_F(FileUtilsTest, FileExistsFalse) {
    EXPECT_FALSE(file_exists("non_existent_file.txt"));
}

TEST_F(FileUtilsTest, DirectoryExistsTrue) {
    EXPECT_TRUE(directory_exists(test_dir));
}

TEST_F(FileUtilsTest, DirectoryExistsFalse) {
    EXPECT_FALSE(directory_exists("non_existent_directory"));
}

// Directory operations
TEST_F(FileUtilsTest, CreateDirectorySuccess) {
    std::string new_dir = test_dir + "/new_directory";
    
    bool result = create_directory(new_dir);
    EXPECT_TRUE(result);
    EXPECT_TRUE(directory_exists(new_dir));
}

TEST_F(FileUtilsTest, CreateNestedDirectories) {
    std::string nested_dir = test_dir + "/level1/level2/level3";
    
    bool result = create_directory(nested_dir);
    EXPECT_TRUE(result);
    EXPECT_TRUE(directory_exists(nested_dir));
}

// File operations
TEST_F(FileUtilsTest, RemoveFileSuccess) {
    std::string temp_file = test_dir + "/temp.txt";
    write_file(temp_file, "temp content");
    
    EXPECT_TRUE(file_exists(temp_file));
    
    bool result = remove_file(temp_file);
    EXPECT_TRUE(result);
    EXPECT_FALSE(file_exists(temp_file));
}

TEST_F(FileUtilsTest, CopyFileSuccess) {
    std::string copy_file_path = test_dir + "/copy.txt";
    
    bool result = copy_file(test_file, copy_file_path);
    EXPECT_TRUE(result);
    EXPECT_TRUE(file_exists(copy_file_path));
    
    std::string original_content = read_file(test_file);
    std::string copied_content = read_file(copy_file_path);
    EXPECT_EQ(original_content, copied_content);
}

TEST_F(FileUtilsTest, MoveFileSuccess) {
    std::string temp_file = test_dir + "/temp_move.txt";
    std::string moved_file = test_dir + "/moved.txt";
    std::string content = "content to move";
    
    write_file(temp_file, content);
    EXPECT_TRUE(file_exists(temp_file));
    
    bool result = move_file(temp_file, moved_file);
    EXPECT_TRUE(result);
    EXPECT_FALSE(file_exists(temp_file));
    EXPECT_TRUE(file_exists(moved_file));
    
    std::string moved_content = read_file(moved_file);
    EXPECT_EQ(moved_content, content);
}

// File information
TEST_F(FileUtilsTest, GetFileSize) {
    std::size_t size = get_file_size(test_file);
    EXPECT_EQ(size, test_content.length());
}

TEST_F(FileUtilsTest, GetFileSizeNonExistent) {
    std::size_t size = get_file_size("non_existent_file.txt");
    EXPECT_EQ(size, 0);
}

TEST_F(FileUtilsTest, GetExtension) {
    EXPECT_EQ(get_extension("file.txt"), ".txt");
    EXPECT_EQ(get_extension("file.tar.gz"), ".gz");
    EXPECT_EQ(get_extension("file"), "");
    EXPECT_EQ(get_extension("/path/to/file.cpp"), ".cpp");
}

TEST_F(FileUtilsTest, GetFilename) {
    EXPECT_EQ(get_filename("/path/to/file.txt"), "file.txt");
    EXPECT_EQ(get_filename("file.txt"), "file.txt");
    EXPECT_EQ(get_filename("/path/to/directory/"), "");
}

TEST_F(FileUtilsTest, GetDirectory) {
    EXPECT_EQ(get_directory("/path/to/file.txt"), "/path/to");
    EXPECT_EQ(get_directory("file.txt"), "");
}

// Directory listing
TEST_F(FileUtilsTest, ListFiles) {
    // Create additional test files
    write_file(test_dir + "/file1.txt", "content1");
    write_file(test_dir + "/file2.cpp", "content2");
    
    std::vector<std::string> files = list_files(test_dir, false);
    EXPECT_GE(files.size(), 3); // At least our 3 files
    
    // Check that all files contain our test directory path
    for (const auto& file : files) {
        EXPECT_TRUE(file.find(test_dir) != std::string::npos);
    }
}

TEST_F(FileUtilsTest, ListDirectories) {
    // Create subdirectories
    create_directory(test_dir + "/subdir1");
    create_directory(test_dir + "/subdir2");
    
    std::vector<std::string> dirs = list_directories(test_dir, false);
    EXPECT_GE(dirs.size(), 2); // At least our 2 subdirectories
}

// Path operations
TEST_F(FileUtilsTest, GetAbsolutePath) {
    std::string abs_path = get_absolute_path(test_file);
    EXPECT_TRUE(abs_path.find(test_file) != std::string::npos);
    EXPECT_TRUE(abs_path.length() > test_file.length());
}

// Current directory operations
TEST_F(FileUtilsTest, CurrentDirectoryOperations) {
    std::string original_dir = get_current_directory();
    EXPECT_FALSE(original_dir.empty());
    
    // Test setting current directory (be careful with this in tests)
    std::string abs_test_dir = get_absolute_path(test_dir);
    bool result = set_current_directory(abs_test_dir);
    EXPECT_TRUE(result);
    
    std::string new_current = get_current_directory();
    EXPECT_TRUE(new_current.find(test_dir) != std::string::npos);
    
    // Restore original directory
    set_current_directory(original_dir);
}

// Edge cases and error handling
TEST_F(FileUtilsTest, EmptyFilename) {
    EXPECT_FALSE(file_exists(""));
    EXPECT_TRUE(read_file("").empty());
    EXPECT_FALSE(write_file("", "content"));
}

TEST_F(FileUtilsTest, LargeFileOperations) {
    std::string large_file = test_dir + "/large.txt";
    std::string large_content(10000, 'A'); // 10KB of 'A's
    
    bool write_result = write_file(large_file, large_content);
    EXPECT_TRUE(write_result);
    
    std::string read_content = read_file(large_file);
    EXPECT_EQ(read_content, large_content);
    
    std::size_t size = get_file_size(large_file);
    EXPECT_EQ(size, large_content.length());
}

TEST_F(FileUtilsTest, SpecialCharactersInFilename) {
    std::string special_file = test_dir + "/file with spaces & symbols!.txt";
    std::string content = "Special file content";
    
    bool write_result = write_file(special_file, content);
    EXPECT_TRUE(write_result);
    
    EXPECT_TRUE(file_exists(special_file));
    
    std::string read_content = read_file(special_file);
    EXPECT_EQ(read_content, content);
}
