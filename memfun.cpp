#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <algorithm>
#include <regex>

int main() {
    namespace fs = std::filesystem;

    const std::regex re{ R"(\s*\*\s*(.*?)\[meta\s+(.*?)\]\s*)" };
    constexpr std::string_view ignore_list[] {
        "op_and",
        "op_or",
        "op_xor",
        "op_ostream",
        "op_istream",
        "op_less",
        "op_less_equal",
        "op_greater",
        "op_greater_equal",
        "op_equal",
        "op_not_equal",
        "op_compare_3way",
    };

    for (const auto& entry : fs::recursive_directory_iterator("site_generator/cpprefjp/site/reference/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".md" && std::ranges::count(entry.path().native(), '/') >= 6) {
            if (std::ranges::contains(ignore_list, entry.path().stem())) {
                continue;
            }
            if (entry.path().stem().native().ends_with("_free")) {
                continue;
            }

            std::cerr << "\e[K" << entry.path().native().substr(0, 100) << "\r";

            std::ifstream ifs{ entry.path() };
            std::string line;
            std::smatch match;
            bool ok = false;
            while (std::getline(ifs, line)) {
                if (std::regex_match(line, match, re)) {
                    if (match[2] == "class") {
                        ok = true;
                        break;
                    }
                }
            }

            if (not ok) {
                std::cout << entry.path().native() << ": Missing [meta class]\n";
            }
        }
    }
}