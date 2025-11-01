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
#include <iterator>
#include <cctype>
#include <cstddef>

int main() {
    using namespace std::literals;
    namespace fs = std::filesystem;

    int error_count = 0;
    for (const auto& entry : fs::recursive_directory_iterator("site_generator/cpprefjp/site/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            std::cerr << "\e[K" << entry.path().native().substr(0, 100) << "\r";

            std::ifstream ifs{ entry.path() };
            std::string buf(std::istreambuf_iterator<char>{ ifs }, {});
            ifs.close();
            std::erase(buf, '\r');
            std::string_view s = buf;

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

            auto base_path = entry.path();
            auto parent_path = std::format("codes{}", base_path.parent_path().string().substr("site_generator/cpprefjp/site"sv.size()));
            auto cpp_version = metas["cpp"].size() ? metas["cpp"] : "cpp03";
            std::size_t pos = 0;
            std::size_t code_count = 0;
            while ((pos = s.find("```cpp example\n", pos)) < s.size()) {
                auto code_start = pos + "```cpp example\n"sv.size();
                auto code_end = s.find("```", code_start);
                if (code_end == std::string_view::npos) {
                    print_error(pos, "コードブロックが閉じられていません。");
                    break;
                }
                pos = code_end + "```"sv.size();
                auto code = s.substr(code_start, code_end - code_start);
                auto path = std::format("{}/{}.{}.{}.cpp", parent_path, base_path.stem().string(), ++code_count, cpp_version);
                fs::create_directories(parent_path);
                bool skip = false;
                if (std::ifstream ifs{ path }) {
                    std::string prev_code(std::istreambuf_iterator<char>{ ifs }, {});
                    if (prev_code == code) {
                        skip = true;
                    }
                }
                if (not skip) {
                    if (std::ofstream ofs{ path }; !ofs) {
                        print_error(std::string_view::npos, std::format("ファイル '{}' を開けません。", path));
                        break;
                    }
                    else {
                        ofs << code;
                    }
                }

                auto output_path = path.substr(0, path.size() - 4) + ".txt";
                auto actual_output_path = path.substr(0, path.size() - 4) + ".stdout";
                bool detected_output = false;
                std::size_t p = 0;
                while ((p = s.find_first_of("*\xe5", pos)) < pos + 10) {
                    if (s[p] == '*') {
                        auto line_last = s.find('\n', p);
                        auto link_first = std::ranges::min({
                            s.find("[meta", p),
                            s.find("[mathjax enable", p),
                            s.find("[link", p),
                            s.find("[color", p),
                            s.find("[italic", p),
                        });
                        if (link_first == std::string_view::npos || link_first > line_last) {
                            break;
                        }
                        pos = line_last;
                        continue;
                    }
                    if (s[p] == '\xe5') {
                        if (s.substr(p, 6) != "出力" || s.substr(p, 9) == "出力例") {
                            break;
                        }
                        pos = s.find('\n', p);
                        p = s.find("```\n", pos);
                        if (p == std::string_view::npos || p >= pos + 10) {
                            break;
                        }
                        pos = p;
                        auto output_start = pos + "```\n"sv.size();
                        auto output_end = s.find("```", output_start);
                        if (output_end == std::string_view::npos) {
                            print_error(pos, "コードブロックが閉じられていません。");
                            break;
                        }
                        pos = output_end + "```"sv.size();
                        detected_output = true;
                        std::string output{ s.substr(output_start, output_end - output_start) };
                        if (std::ifstream ifs{ actual_output_path, std::ios::binary }) {
                            std::string actual_output(std::istreambuf_iterator<char>{ ifs }, {});
                            auto expected_tailing_lf_count = output.size() - 1 - output.find_last_not_of('\n');  // オーバーフローを利用して計算
                            auto actual_tailing_lf_count = actual_output.size() - 1 - actual_output.find_last_not_of('\n');  // オーバーフローを利用して計算
                            if (expected_tailing_lf_count == 1 && actual_tailing_lf_count == 0) {
                                // 末尾改行なしを許容
                                output.pop_back();
                            }
                            else if (expected_tailing_lf_count == 1 && actual_tailing_lf_count == 2) {
                                // 末尾改行の過剰を許容
                                output += '\n';
                            }
                        }
                        bool skip = false;
                        if (std::ifstream ifs{ output_path }) {
                            std::string prev_output(std::istreambuf_iterator<char>{ ifs }, {});
                            if (prev_output == output) {
                                skip = true;
                            }
                        }
                        if (not skip) {
                            if (std::ofstream ofs{ output_path }; !ofs) {
                                print_error(std::string_view::npos, std::format("ファイル '{}' を開けません。", output_path));
                                break;
                            }
                            else {
                                ofs << output;
                            }
                        }
                        break;
                    }
                }
                if (not detected_output) {
                    fs::remove(output_path);
                }
            }

            while (true) {
                auto path = std::format("{}/{}.{}.{}.cpp", parent_path, base_path.stem().string(), ++code_count, cpp_version);
                auto output_path = path.substr(0, path.size() - 4) + ".txt";
                if (not fs::exists(path)) {
                    break;
                }
                fs::remove(path);
                fs::remove(output_path);
            }
        }
    }
    if (error_count) {
        std::cerr << "\e[K" << error_count << " error" << (error_count == 1 ? "" : "s") << " generated.\n";
    }
}
