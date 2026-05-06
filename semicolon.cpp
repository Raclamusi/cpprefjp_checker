#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <string_view>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <cstddef>

int main() {
    namespace fs = std::filesystem;

    std::map<std::string, std::set<std::string>> headers;

    int error_count = 0;
    for (const auto& entry : fs::recursive_directory_iterator("site_generator/cpprefjp/site/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            std::cerr << "\e[K" << entry.path().native().substr(0, 100) << "\r";

            std::ifstream ifs{ entry.path() };
            std::string buf(std::istreambuf_iterator<char>{ ifs }, {});
            ifs.close();
            std::string_view s = buf;

            std::vector<std::string> path;
            {
                const auto p = entry.path().string().erase(0, std::char_traits<char>::length("site_generator/cpprefjp/site/"));
                std::size_t first = 0;
                std::size_t last = p.find('/');
                while (last != std::string::npos) {
                    path.push_back(p.substr(first, last - first));
                    first = last + 1;
                    last = p.find('/', first);
                }
                path.push_back(p.substr(first, p.size() - first - 3));
            }

            std::unordered_map<std::string, std::string> metas;
            for (std::size_t pos = 0; (pos = s.find("[meta", pos)) != std::string_view::npos; ) {
                auto newline = s.find('\n', pos);
                auto asterisk = s.find('*', s.rfind('\n', pos));
                auto target_first = s.find_first_not_of(" \t\r\f", asterisk + 1);
                auto target_last = pos;
                auto name_first = s.find_first_not_of(" \t\r\f", pos + 5);
                auto name_last = s.rfind(']', newline);
                pos = newline;
                if (asterisk == std::string_view::npos || asterisk > newline ||
                    target_first == std::string_view::npos || name_first == std::string_view::npos || name_last == std::string_view::npos ||
                    target_first >= target_last || name_first >= name_last || name_last + 1 != newline) {
                    continue;
                }
                std::string name{ s.substr(name_first, name_last - name_first) };
                std::string_view target = s.substr(target_first, target_last - target_first);
                if (name == "cpp") {
                    if (target.ends_with("deprecated")) {
                        name = "cpp_deprecated";
                    }
                    else if (target.ends_with("removed")) {
                        name = "cpp_removed";
                    }
                }
                metas.try_emplace(name, target);
            }

            bool must_be_class = metas["id-type"] == "class" ||
                                 metas["id-type"] == "class template" ||
                                 metas["id-type"] == "enum";
            bool is_class = false;
            bool first_code = (s.find("```cpp") < s.find("##")) && entry.path().native().starts_with("site_generator/cpprefjp/site/reference/") && std::ranges::count(entry.path().native(), '/') >= 5;
            
            constexpr std::string_view ignore_list[] {
                "op_constructor",
                "op_deduction_guide",
                "op_destructor",
                "op_initializer",
                "weekday_constants",
                "month_constants",
                "duration_aliases",
                "op_path",
                "lock_free_property",
                "si_prefix",
                "iterator_tag",
                "execution_policy",
                "negators",
            };
            constexpr std::string_view ignore_suffix_list[] {
                " (非メンバ関数)",
                " (単項)",
                ".void",
                "(size_type)",
                "-size_type",
                "リテラル",
                "\r",
                " (utility)",
            };
            auto title = s.substr(2, s.find('\n') - 2);
            for (auto&& suffix : ignore_suffix_list) {
                if (title.ends_with(suffix)) {
                    title.remove_suffix(suffix.length());
                }
            }
            if (title.starts_with("generator::")) {
                title.remove_prefix(std::char_traits<char>::length("generator::"));
            }
            std::vector<std::string_view> titles;
            if (title.contains(',') && not title.ends_with(',')) {
                while (title.size()) {
                    titles.push_back(title.substr(0, title.find(',')));
                    title.remove_prefix(std::ranges::min(title.size(), titles.back().size() + 1));
                }
            }
            else if (title.contains('/') && not (title.ends_with('/') || title.ends_with("/="))) {
                while (title.size()) {
                    titles.push_back(title.substr(0, title.find('/')));
                    title.remove_prefix(std::ranges::min(title.size(), titles.back().size() + 1));
                }
            }
            else {
                titles.push_back(title);
            }
            std::string title2;
            if (auto pos = title.find("operator"); pos != std::string::npos) {
                title2 = title;
                if (title2[pos + 8] == ' ') {
                    title2.erase(pos + 8);
                }
                else {
                    title2.insert(pos + 8, " ");
                }
            }
            else if (title.contains('\\')) {
                title2 = title;
                std::erase(title2, '\\');
            }

            if (title.size() && entry.path().native().starts_with("site_generator/cpprefjp/site/reference/") && std::ranges::count(entry.path().native(), '/') >= 5) {
                std::string key;
                if (metas.contains("namespace")) {
                    key += metas["namespace"];
                    key += "::";
                }
                if (metas.contains("class")) {
                    key += metas["class"];
                    key += "::";
                }
                if (metas.contains("class template")) {
                    key += metas["class template"];
                    key += "::";
                }
                key += title;
                headers[key].insert(metas["header"]);
            }

            auto print_error = [&](std::size_t pos, std::string_view message) {
                ++error_count;
                auto line_first = pos == 0 ? std::string_view::npos : s.rfind('\n', pos - 1);
                auto line = std::ranges::count(s.begin(), s.begin() + pos, '\n') + 1;
                auto ch = line_first == std::string_view::npos ? 0 : pos - s.rfind('\n', pos - 1);
                std::cout << entry.path().native() << ":" << line << ":" << ch << ": error: " << message << "\n";
                auto first = s.rfind('\n', s.rfind('\n', s.rfind('\n', pos) - 1) - 1) + 1;
                if (line <= 2) {
                    line = 3;
                    ch = 0;
                    first = 0;
                }
                for (int i = -2; i <= 2; ++i) {
                    auto last = s.find('\n', first) + 1;
                    std::cout << std::setw(5) << line + i << " | " << s.substr(first, last - first);
                    first = last;
                }
            };

            if (first_code && not std::ranges::contains(ignore_list, entry.path().stem())) {
                auto code_first = s.find("```cpp") + 6;
                auto code_last = s.find("```", code_first);
                auto code = s.substr(code_first, code_last - code_first);
                if (not (std::ranges::all_of(titles, [&](auto t) { return code.contains(t); }) || (title2.size() && code.contains(title2)) || code.contains("追加宣言なし") || code.contains("task<T, Environment>"))) {
                    print_error(code_first, "no title in first code");
                }
                if (code.contains("namespace ") && not title.contains('-') && metas["id-type"] != "namespace") {
                    auto ns_first = code.find("namespace ") + 10;
                    auto ns_last = code.find_first_of(" \n{", ns_first);
                    std::string ns{ code.substr(ns_first, ns_last - ns_first) };
                    if (entry.path() == "site_generator/cpprefjp/site/reference/ranges/subrange/get.md") {
                        ns = "std";
                    }
                    else if (code.contains("namespace chrono_literals")) {
                        ns += "::chrono_literals";
                    }
                    else if (code.contains("namespace chrono") && ns != "chrono") {
                        ns += "::chrono";
                    }
                    else if (code.contains("namespace complex_literals") && ns != "complex_literals") {
                        ns += "::complex_literals";
                    }
                    else if (code.contains("namespace string_literals") && ns != "string_literals") {
                        ns += "::string_literals";
                    }
                    else if (code.contains("namespace string_view_literals") && ns != "string_view_literals") {
                        ns += "::string_view_literals";
                    }
                    else if (code.contains("namespace regex_constants") && ns != "regex_constants") {
                        ns += "::regex_constants";
                    }
                    else if (code.contains("namespace ranges") && ns != "ranges") {
                        ns += "::ranges";
                    }
                    else if (code.contains("namespace this_thread") && ns != "this_thread") {
                        ns += "::this_thread";
                    }
                    if (metas["namespace"] != ns) {
                        print_error(code_first + ns_first, std::format("namespace mismatch (meta: '{}' but code: '{}')", metas["namespace"], ns));
                    }
                }
            }

            if (path.size() >= 2 && path[0] == "reference" && path[1] != "node_handle") {
                if (path[1] != metas["header"]) {
                    print_error(0, std::format("header mismatch (path: '{}' but meta: '{}')", path.size() >= 2 ? path[1] : "", metas["header"]));
                }
            }

            std::size_t pos = 0;
            bool head = false;
            bool pre_newline = false;
            while ((pos = s.find("```cpp", pos)) != std::string_view::npos) {
                pos += 6;
                int nest = 0;
                std::stack<int> stack;
                bool pre_template = false;
                int in_template = 0;
                bool in_comment = false;
                bool in_block_comment = false;
                bool in_string = false;
                bool in_attribute = false;
                for (; pos < s.size() && s.substr(pos, 3) != "```"; ++pos) {
                    if (in_block_comment) {
                        if (s.substr(pos, 2) == "*/") {
                            in_block_comment = false;
                            ++pos;
                        }
                        continue;
                    }
                    if (in_comment) {
                        if (s[pos] == '\n') {
                            in_comment = false;
                        }
                        continue;
                    }
                    if (in_string) {
                        if (s[pos] == '"' && (pos == 0 || s[pos - 1] != '\\')) {
                            in_string = false;
                        }
                        if (s[pos] == '\n') {
                            in_string = false;
                        }
                        continue;
                    }
                    if (in_attribute) {
                        if (s.substr(pos, 2) == "]]") {
                            in_attribute = false;
                            ++pos;
                        }
                        continue;
                    }
                    if (s[pos] == '\n') {
                        pre_newline = false;
                    }
                    else if (auto c = static_cast<unsigned char>(s[pos]); c >= 0x7f) {
                        int len = 1;
                        int codepoint = c;
                        if (0xc2 <= c && c <= 0xdf) {
                            codepoint = (((c & 0x1f) << 6) | (s[pos + 1] & 0x3f));
                            len = 2;
                        }
                        else if (0xe0 <= c && c <= 0xef) {
                            codepoint = (((c & 0x0f) << 12) | ((s[pos + 1] & 0x3f) << 6) | (s[pos + 2] & 0x3f));
                            len = 3;
                        }
                        else if (0xf0 <= c && c <= 0xf4) {
                            codepoint = (((c & 0x07) << 18) | ((s[pos + 1] & 0x3f) << 12) | ((s[pos + 2] & 0x3f) << 6) | (s[pos + 3] & 0x3f));
                            len = 4;
                        }
                        if ((c < 0xe3 || 0xe9 < c) && codepoint != U'…' && codepoint != U'π' && s[pos - 1] != '\'' && s[pos + len] != '\'') {
                            std::ostringstream oss;
                            oss << "non-ascii character '" << s.substr(pos, len) << "' U+";
                            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << codepoint;
                            print_error(pos, oss.str());
                        }
                        pos += len - 1;
                    }
                    if (stack.size() && stack.top() == nest) {
                        if (s[pos] == ';') {
                            stack.pop();
                            head = false;
                        }
                        else if (s[pos] == '{') {
                            head = false;
                        }
                        else if (s[pos] == '}') {
                            print_error(pos, "missing semicolon (unexpected '}')");
                            stack.pop();
                            head = false;
                        }
                        else if (not head && (s[pos] == '\n' || not (std::isspace(s[pos]) || std::isalpha(s[pos]) || std::isdigit(s[pos]) || s[pos] == '_' || s[pos] == '=' || s[pos] == ')' || s[pos] == '>'))) {
                            print_error(pos, "missing semicolon (unexpected next code)");
                            stack.pop();
                            head = false;
                        }
                    }
                    if (pre_template) {
                        if (not in_template && s[pos] == '\n') {
                            pre_template = false;
                        }
                        if (s[pos] == '<') {
                            pre_template = false;
                            in_template = 1;
                        }
                    }
                    else if (in_template) {
                        if (s[pos] == '<') {
                            in_template += 1;
                        }
                        if (s[pos] == '>') {
                            in_template -= 1;
                        }
                    }
                    if (s[pos] == '{' && (s[pos - 1] != '\'' || s[pos + 1] != '\'')) {
                        ++nest;
                    }
                    else if (s[pos] == '}' && (s[pos - 1] != '\'' || s[pos + 1] != '\'')) {
                        --nest;
                    }
                    else if ((s.substr(pos, 6) == "class " || s.substr(pos, 7) == "struct " || s.substr(pos, 5) == "enum " || s.substr(pos, 6) == "union " || s.substr(pos, 6) == "using " || s.substr(pos, 8) == "typedef ") && (pos == 0 || std::isspace(s[pos - 1])) && (s.substr(pos, 9) != "struct tm") && (pos < 9 || s.substr(pos - 9, 9) != "operator ")) {
                        if (not in_template) {
                            if (stack.size() && stack.top() == nest) {
                                if (not pre_newline) {
                                    print_error(pos, "missing semicolon (unexpected next class declaration)");
                                }
                            }
                            else {
                                stack.push(nest);
                                head = true;
                                pre_newline = true;
                                if (first_code) {
                                    is_class = true;
                                }
                            }
                        }
                    }
                    else if (s.substr(pos, 8) == "template") {
                        if (not in_template) {
                            pre_template = true;
                        }
                    }
                    else if (s.substr(pos, 3) == "[]<") {
                        in_template = 1;
                        pos += 2;
                    }
                    else if (s.substr(pos, 2) == "//") {
                        in_comment = true;
                    }
                    else if (s.substr(pos, 2) == "/*") {
                        in_block_comment = true;
                        ++pos;
                    }
                    else if (s[pos] == '"' && (pos == 0 || s[pos - 1] != '\\')) {
                        in_string = true;
                    }
                    else if (s.substr(pos, 2) == "[[") {
                        in_attribute = true;
                    }
                    if (nest < 0) {
                        print_error(pos, "unmatched '}'");
                        nest = 0;
                    }
                }
                if (stack.size()) {
                    print_error(pos, "missing semicolon (unexpected end of code)");
                }
                pos += 3;
                if (nest > 0) {
                    print_error(pos, "unmatched '{'");
                }
                if (first_code && must_be_class != is_class) {
                    //std::cout << entry.path().native() << ": id-type mismatch (must " << (must_be_class ? "" : "NOT ") << "be class but is " << (is_class ? "" : "NOT ") << "class)\n";
                }
                first_code = false;
            }
        }
    }

    if (error_count) {
        std::cerr << "\e[K" << error_count << " error" << (error_count == 1 ? "" : "s") << " generated.\n";
    }
}
