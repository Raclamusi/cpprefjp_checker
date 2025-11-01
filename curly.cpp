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
        "site_generator/cpprefjp/site/lang/cpp20/conditionally_trivial_special_member_functions.md",
        "site_generator/cpprefjp/site/lang/cpp20/nodiscard_should_have_a_reason.md",
        "site_generator/cpprefjp/site/lang/cpp11/override_final.md",
        "site_generator/cpprefjp/site/lang/cpp17/attributes_for_namespaces_and_enumerators.md",
        "site_generator/cpprefjp/site/start_editing/contribution_stats_2024.md",
        "site_generator/cpprefjp/site/start_editing/markdown_cpprefjp.md",
        "site_generator/cpprefjp/site/reference/regex/regex_constants/error_type.md",
        "site_generator/cpprefjp/site/reference/cfloat/ldbl_decimal_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/dbl_decimal_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/decimal_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/dbl_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/flt_decimal_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/flt_dig.md",
        "site_generator/cpprefjp/site/reference/cfloat/ldbl_dig.md",
        "site_generator/cpprefjp/site/reference/fstream/basic_fstream/op_constructor.md",
        "site_generator/cpprefjp/site/reference/fstream/basic_ofstream/op_constructor.md",
        "site_generator/cpprefjp/site/reference/fstream/basic_ifstream/op_constructor.md",
        "site_generator/cpprefjp/site/reference/format/basic_format_parse_context/begin.md",
        "site_generator/cpprefjp/site/reference/format/formatter.md",
        "site_generator/cpprefjp/site/reference/format/vformat_to.md",
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

            std::size_t pos = 0;
            std::stack<std::size_t> paren_stack;
            while ((pos = s.find_first_of("{}", pos)) < s.size()) {
                if (s[pos] == '{') {
                    paren_stack.push(pos);
                }
                else if (s[pos] == '}') {
                    if (paren_stack.empty()) {
                        print_error(pos, "Unmatched closing bracket '}'");
                    }
                    else {
                        paren_stack.pop();
                    }
                }
                pos += 1;
            }
            while (paren_stack.size()) {
                print_error(paren_stack.top(), "Unmatched opening bracket '{'");
                paren_stack.pop();
            }
        }
    }
    if (error_count) {
        std::cerr << "\e[K" << error_count << " error" << (error_count == 1 ? "" : "s") << " generated.\n";
    }
}
