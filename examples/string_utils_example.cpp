#include <baba/string_utils.hpp>
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== String Utils Advanced Example ===" << std::endl << std::endl;
    
    // Trimming functions
    std::cout << "=== Trimming Functions ===" << std::endl;
    std::string text = "  \t  Hello, World!  \n  ";
    std::cout << "Original: '" << text << "'" << std::endl;
    std::cout << "trim(): '" << baba::core::string_utils::trim(text) << "'" << std::endl;
    std::cout << "ltrim(): '" << baba::core::string_utils::ltrim(text) << "'" << std::endl;
    std::cout << "rtrim(): '" << baba::core::string_utils::rtrim(text) << "'" << std::endl;
    std::cout << std::endl;
    
    // Splitting functions
    std::cout << "=== Splitting Functions ===" << std::endl;
    std::string csv_data = "apple,banana,cherry,date";
    auto parts1 = baba::core::string_utils::split(csv_data, ',');
    std::cout << "Split by char ',': ";
    for (const auto& part : parts1) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl;
    
    std::string delimited_data = "one::two::three::four";
    auto parts2 = baba::core::string_utils::split(delimited_data, "::");
    std::cout << "Split by string '::': ";
    for (const auto& part : parts2) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl << std::endl;
    
    // Joining function
    std::cout << "=== Joining Function ===" << std::endl;
    std::vector<std::string> words = {"Hello", "beautiful", "world"};
    std::cout << "Join with ' ': " << baba::core::string_utils::join(words, " ") << std::endl;
    std::cout << "Join with ' - ': " << baba::core::string_utils::join(words, " - ") << std::endl;
    std::cout << std::endl;
    
    // Case conversion
    std::cout << "=== Case Conversion ===" << std::endl;
    std::string mixed_case = "Hello World 123!";
    std::cout << "Original: " << mixed_case << std::endl;
    std::cout << "to_upper(): " << baba::core::string_utils::to_upper(mixed_case) << std::endl;
    std::cout << "to_lower(): " << baba::core::string_utils::to_lower(mixed_case) << std::endl;
    std::cout << std::endl;
    
    // Prefix and suffix checking
    std::cout << "=== Prefix and Suffix Checking ===" << std::endl;
    std::string filename = "document.pdf";
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "starts_with('doc'): " << (baba::core::string_utils::starts_with(filename, "doc") ? "true" : "false") << std::endl;
    std::cout << "starts_with('image'): " << (baba::core::string_utils::starts_with(filename, "image") ? "true" : "false") << std::endl;
    std::cout << "ends_with('.pdf'): " << (baba::core::string_utils::ends_with(filename, ".pdf") ? "true" : "false") << std::endl;
    std::cout << "ends_with('.txt'): " << (baba::core::string_utils::ends_with(filename, ".txt") ? "true" : "false") << std::endl;
    std::cout << std::endl;
    
    // String replacement
    std::cout << "=== String Replacement ===" << std::endl;
    std::string template_text = "Hello {name}, welcome to {place}!";
    std::cout << "Original: " << template_text << std::endl;
    std::string replaced1 = baba::core::string_utils::replace_all(template_text, "{name}", "Alice");
    std::cout << "Replace {name}: " << replaced1 << std::endl;
    std::string replaced2 = baba::core::string_utils::replace_all(replaced1, "{place}", "our website");
    std::cout << "Replace {place}: " << replaced2 << std::endl;
    std::cout << std::endl;
    
    // Substring checking
    std::cout << "=== Substring Checking ===" << std::endl;
    std::string sentence = "The quick brown fox jumps over the lazy dog";
    std::cout << "Sentence: " << sentence << std::endl;
    std::cout << "contains('fox'): " << (baba::core::string_utils::contains(sentence, "fox") ? "true" : "false") << std::endl;
    std::cout << "contains('cat'): " << (baba::core::string_utils::contains(sentence, "cat") ? "true" : "false") << std::endl;
    std::cout << "contains('quick brown'): " << (baba::core::string_utils::contains(sentence, "quick brown") ? "true" : "false") << std::endl;
    std::cout << std::endl;
    
    // String formatting
    std::cout << "=== String Formatting ===" << std::endl;
    std::string name = "Alice";
    int age = 30;
    double height = 5.6;
    std::string formatted = baba::core::string_utils::format("Name: %s, Age: %d, Height: %.1f ft", name.c_str(), age, height);
    std::cout << "Formatted string: " << formatted << std::endl;
    
    // More complex formatting
    std::string log_entry = baba::core::string_utils::format("[%s] %s: %d items processed", "INFO", "DataProcessor", 1250);
    std::cout << "Log entry: " << log_entry << std::endl;
    std::cout << std::endl;
    
    // Practical example: Processing a CSV-like data
    std::cout << "=== Practical Example: CSV Processing ===" << std::endl;
    std::string csv_line = "  John Doe  , 25 ,  Software Engineer  , New York ";
    std::cout << "Raw CSV line: '" << csv_line << "'" << std::endl;
    
    auto fields = baba::core::string_utils::split(csv_line, ',');
    std::cout << "Parsed and cleaned fields:" << std::endl;
    for (size_t i = 0; i < fields.size(); ++i) {
        std::string cleaned = baba::core::string_utils::trim(fields[i]);
        std::cout << "  Field " << (i + 1) << ": '" << cleaned << "'" << std::endl;
    }
    
    std::cout << std::endl << "=== String Utils Example Complete ===" << std::endl;
    return 0;
}
