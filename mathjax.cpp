#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <string_view>
#include <format>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <cstddef>

int main() {
    using namespace std::literals;
    namespace fs = std::filesystem;

    constexpr std::string_view ignore_files[] {
        "site_generator/cpprefjp/site/start_editing/cite_note_ref.md",
    };

    int error_count = 0;
    for (const auto& entry : fs::recursive_directory_iterator("site_generator/cpprefjp/site/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".md" && not std::ranges::contains(ignore_files, entry.path())) {
            std::cerr << "\e[K" << entry.path().native().substr(0, 100) << "\r";

            std::ifstream ifs{ entry.path() };
            std::string buf(std::istreambuf_iterator<char>{ ifs }, {});
            ifs.close();
            std::string_view s = buf;

            auto print_error = [&](std::size_t pos, std::string_view message) {
                ++error_count;
                if (pos == std::string_view::npos) {
                    std::cout << entry.path().native() << ":\nerror: " << message << "\n";
                    return;
                }
                auto line = std::ranges::count(s.begin(), s.begin() + pos, '\n') + 1;
                auto ch = pos - s.rfind('\n', pos - 1);
                std::cout << entry.path().native() << ":" << line << ":" << ch << ":\nerror: " << message << "\n";
                auto first = s.rfind('\n', pos - 1) + 1;
                auto last = s.find('\n', first) + 1;
                std::cout << std::setw(5) << line << " | " << s.substr(first, last - first);
            };
            auto print_warning = [&](std::size_t pos [[maybe_unused]], std::string_view message) {
                std::cout << entry.path().native() << ":\nwarning: " << message << "\n";
            };

            auto mathjax_pos = s.find("* [mathjax enable]");
            bool mathjax_enabled = mathjax_pos != std::string_view::npos;
            bool mathjax_used = false;
            std::size_t pos = 0;
            bool in_code_block = false;
            while (pos < s.size()) {
                auto new_pos = s.find('\n', pos);
                if (new_pos == std::string_view::npos) {
                    new_pos = s.size();
                }
                auto line = s.substr(pos, new_pos - pos);
                if (line.starts_with("```")) {
                    in_code_block = not in_code_block;
                }
                if (not in_code_block && std::ranges::count(line, '$') >= 2) {
                    mathjax_used = true;
                    break;
                }
                pos = new_pos + 1;
            }
            if (mathjax_used && not mathjax_enabled) {
                print_error(pos, "MathJax used but not enabled");
            }
            if (not mathjax_used && mathjax_enabled) {
                print_warning(mathjax_pos, "MathJax enabled but not used");
            }
        }
    }
    if (error_count) {
        std::cerr << "\e[K" << error_count << " error" << (error_count == 1 ? "" : "s") << " generated.\n";
    }
}
