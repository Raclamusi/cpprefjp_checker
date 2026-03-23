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

    int error_count = 0;
    for (const auto& entry : fs::recursive_directory_iterator("site_generator/cpprefjp/site/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            std::cerr << "\e[K" << entry.path().native().substr(0, 100) << "\r";

            std::ifstream ifs{ entry.path() };
            std::string buf(std::istreambuf_iterator<char>{ ifs }, {});
            ifs.close();
            std::string_view s = buf;

            constexpr std::string_view red_check_ignore_list[] {
                "weekday_constants",
                "month_constants",
                "duration_aliases",
                "si_prefix",
                "execution_policy",
                "negators",
                "type-seekdir",
                "type-iostate",
                "type-openmode",
                "type-fmtflags",
                "elements_view",
                "mapping",
                "allocator_traits",
                "no-throw-forward-iterator",
                "no-throw-input-iterator",
                "last_spec",
                "stdexcept",
                "gslice_array",
                "node_handle",
                "basic_syncbuf",
                "match_results",
                "atomic_flag",
                "atomic",
                "insert_iterator",
                "back_insert_iterator",
                "front_insert_iterator",
                "extents",
                "linear_congruential_engine",
                "subtract_with_carry_engine",
                "type_info",
            };
            constexpr std::string_view red_check_ignore_text_list[] {
                "chrono::from_stream",
                "result",
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
                "_view",
                "_t",
                "_time",
            };
            auto title = s.substr(2, s.find('\n') - 2);
            if (std::ranges::contains(red_check_ignore_list, entry.path().stem()) || entry.path().stem().string().starts_with("op_")) {
                title = "";
            }
            for (auto&& suffix : ignore_suffix_list) {
                if (title.ends_with(suffix)) {
                    title.remove_suffix(suffix.length());
                }
            }
            if (title.starts_with("generator::")) {
                title.remove_prefix(std::char_traits<char>::length("generator::"));
            }
            if (title.starts_with("basic_")) {
                title.remove_prefix(std::char_traits<char>::length("basic_"));
            }
            std::vector<std::string_view> titles;
            if (title.contains(',') && not title.ends_with(',')) {
                while (title.size()) {
                    titles.push_back(title.substr(0, title.find(',')));
                    title.remove_prefix(std::ranges::min(title.size(), titles.back().size() + 2));
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
            std::string text;
            std::string href;
            constexpr bool enable_space_check = false;
            while ((pos = s.find_first_of("`[*-\\", pos)) < s.size()) {
                auto key = s[pos];
                if (s[pos] == '\\') {
                    pos += 2;
                    continue;
                }
                if (s.substr(pos, 3) == "```") {
                    auto ppos = pos;
                    auto level = s.find_first_not_of("`", pos + 3) - pos;
                    pos = s.find(std::string(level, '`'), pos + level);
                    if (pos == std::string_view::npos) {
                        print_error(ppos, "Unmatched " + std::string(level, '`'));
                        break;
                    }
                    pos += level;
                    continue;
                }
                if (s[pos] == '`') {
                    if (entry.path() == "site_generator/cpprefjp/site/lang/cpp26/add_atsign_dollar_graveaccent_to_the_basic_character_set.md") {
                        pos += 1;
                        continue;
                    }
                    if constexpr (enable_space_check) {
                        if (std::isspace(s[pos + 1]) && not std::isspace(s[pos - 1])) {
                            print_error(pos + 1, "Space after `");
                        }
                    }
                    pos = s.find('`', pos + 1);
                    if (pos == std::string_view::npos) {
                        print_error(pos, "Unmatched `");
                        break;
                    }
                    if constexpr (enable_space_check) {
                        if (std::isspace(s[pos - 1]) && not std::isspace(s[pos + 1])) {
                            print_error(pos - 1, "Space before `");
                        }
                    }
                    pos += 1;
                    continue;
                }
                if (s[pos] == '[') {
                    auto text_start = pos + 1;
                    pos = s.find_first_of("]`\\", pos + 1);
                    if (pos == std::string_view::npos) {
                        print_error(pos, "Unmatched [");
                        break;
                    }
                    while (s[pos] != ']') {
                        if (s[pos] == '\\') {
                            pos += 1;
                        }
                        else if (s[pos] == '`' && entry.path() != "site_generator/cpprefjp/site/lang/cpp26/add_atsign_dollar_graveaccent_to_the_basic_character_set.md") {
                            if constexpr (enable_space_check) {
                                if (std::isspace(s[pos + 1])) {
                                    print_error(pos + 1, "Space after `");
                                }
                            }
                            auto ppos = pos;
                            pos = s.find('`', pos + 1);
                            if (pos == std::string_view::npos) {
                                pos = ppos + 1;
                            }
                            else {
                                if constexpr (enable_space_check) {
                                    if (std::isspace(s[pos - 1])) {
                                        print_error(pos - 1, "Space before `");
                                    }
                                }
                            }
                        }
                        pos = s.find_first_of("]`", pos + 1);
                        if (pos == std::string_view::npos) {
                            print_error(pos, "Unmatched [");
                            break;
                        }
                    }
                    if (pos == std::string_view::npos) {
                        break;
                    }
                    text = s.substr(text_start, pos - text_start);
                    pos += 1;
                    if (s[pos] != '(') {
                        continue;
                    }
                    auto href_start = pos + 1;
                    for (int paren_count = 1; paren_count > 0; ) {
                        pos = s.find_first_of("()", pos + 1);
                        if (pos == std::string_view::npos) {
                            break;
                        }
                        if (s[pos] == '(') {
                            ++paren_count;
                        } else if (s[pos] == ')') {
                            --paren_count;
                        }
                    }
                    if (pos == std::string_view::npos) {
                        print_error(href_start - 1, "Unmatched (");
                        break;
                    }
                    href = s.substr(href_start, pos - href_start);
                    pos += 1;
                }
                if (s[pos] == '*' || s[pos] == '-') {
                    pos += 1;
                    if (s[pos] != ' ') {
                        continue;
                    }
                    auto line_end = s.find('\n', pos);
                    if (line_end == std::string_view::npos) {
                        line_end = s.size();
                    }
                    auto text_start = pos + 1;
                    pos = s.rfind("[link ", line_end);
                    if (pos == std::string_view::npos || pos < text_start) {
                        if (entry.path().native().starts_with("site_generator/cpprefjp/site/reference/")) {
                            pos = s.rfind("[color ", line_end);
                            if (pos == std::string_view::npos || pos < text_start);
                            else {
                                text = s.substr(text_start, pos - text_start);
                                if (std::ranges::find_if(titles, [&](auto t) { return text.contains(t); }) == titles.end() && not std::ranges::contains(red_check_ignore_text_list, text)) {
                                    print_error(text_start, "Red link text must be title" + std::format(" (titles: {})", titles));
                                }
                            }
                        }
                        pos = text_start;
                        continue;
                    }
                    text = s.substr(text_start, pos - text_start);
                    pos += 6;
                    auto link_start = pos;
                    pos = s.rfind("]", line_end);
                    if (pos == std::string_view::npos || pos < link_start) {
                        pos = text_start;
                        continue;
                    }
                    href = s.substr(link_start, pos - link_start);
                    pos += 1;
                    if (text.ends_with("[color ff0000]")) {
                        text.erase(text.size() - 14);
                    }
                    if (text.ends_with("[color 0000ff]")) {
                        text.erase(text.size() - 14);
                    }
                    if (text.ends_with("[italic]")) {
                        text.erase(text.size() - 8);
                    }
                }

                // check
                if (text.empty() || href.empty()) {
                    continue;
                }
                if (key == '[') {
                    if (not (text.starts_with('`') && text.ends_with('`')) || std::ranges::count(text, '`') != 2) {
                        continue;
                    }
                    text.erase(0, 1);
                    text.pop_back();
                }
                if (text.contains('/') && not (text.ends_with('/') || text.ends_with("/="))) {
                    continue;
                }
                if (std::ranges::find_if(text, [](char c) { return (unsigned char)(c) >= 0x80; }) != text.end()) {
                    continue;
                }
                if (std::isdigit(text[0])) {
                    continue;
                }
                if (text.starts_with("sub_map_offset.")) {
                    continue;
                }
                if (href.starts_with("http") || href.starts_with('#')) {
                    continue;
                }
                if (not href.starts_with('/')) {
                    auto parent = entry.path().parent_path().string();
                    parent.erase(0, "site_generator/cpprefjp/site"sv.size());
                    href = parent + "/" + href;
                    for (auto p = href.find("/../"); p != std::string_view::npos; p = href.find("/../")) {
                        auto q = href.rfind('/', p - 1);
                        if (q == std::string_view::npos) {
                            break;
                        }
                        href.erase(q, p + 3 - q);
                    }
                    for (auto p = href.find("/./"); p != std::string_view::npos; p = href.find("/./")) {
                        href.erase(p, 2);
                    }
                }
                if (href.starts_with("/lang/")) {
                    continue;
                }
                if (href.ends_with("formatter.md")) {
                    continue;
                }
                if (href == "/reference/ios/ios_base/type-fmtflags.md") {
                    continue;
                }
                if (text == "StoreSize" && href == "/reference/ranges/subrange/op_constructor.md") {
                    continue;
                }
                if (text == "operator()" && href == "/reference/coroutine/coroutine_handle/resume.md") {
                    continue;
                }
                if (text == "to-unsigned-like" && href == "/reference/type_traits/make_unsigned.md") {
                    continue;
                }
                if (text == "weakly-equality-comparable-with" && href == "/reference/concepts/equality_comparable.md") {
                    continue;
                }
                if (text == "os.setstate" && href == "/reference/ios/basic_ios.md") {
                    continue;
                }
                if (text == "space" && href == "/reference/locale/ctype_base.md") {
                    continue;
                }
                if (text == "Period<N, D>" && href == "/reference/ratio/ratio.md") {
                    continue;
                }
                if (text == "synth-three-way" && href == "/reference/exposition-only/synth-three-way-result.md") {
                    continue;
                }
                if (text == "with-reference" && href == "/reference/iterator/dereferenceable.md") {
                    continue;
                }
                if (text == "can-reference" && href == "/reference/iterator/dereferenceable.md") {
                    continue;
                }
                if (text == "is-signed-integer-like" && href == "/reference/iterator/is_integer_like.md") {
                    continue;
                }
                if (text == "indirectly-binary-left-foldable" && href == "/reference/algorithm/ranges_fold_left.md") {
                    continue;
                }
                if (text == "indirectly-binary-right-foldable" && href == "/reference/algorithm/ranges_fold_right.md") {
                    continue;
                }
                if (text == "what()" && href == "/reference/stdexcept.md") {
                    continue;
                }
                if (text == "value" && href == "/reference/generator/generator.md") {
                    continue;
                }
                if (text == "yielded" && href == "/reference/generator/generator.md") {
                    continue;
                }
                if (text == "g.range" && href == "/reference/ranges/elements_of.md") {
                    continue;
                }
                if (text == "coroutine_" && href == "/reference/coroutine/coroutine_handle.md") {
                    continue;
                }
                if (text == "this_thread" && href == "/reference/execution/execution.md") {
                    continue;
                }
                if (text == "indices-for" && href == "/reference/execution/execution/basic-sender.md") {
                    continue;
                }
                if (text == "FWD-ENV-T" && href == "/reference/execution/forwarding_query.md") {
                    continue;
                }
                if (text == "SCHED-ENV" && href == "/reference/execution/execution/schedule.md") {
                    continue;
                }
                if (text == "MAKE-ENV" && href == "/reference/execution/queryable.md") {
                    continue;
                }
                if (text == "GET-AWAITER" && href == "/reference/execution/is-awaitable.md") {
                    continue;
                }
                if (text == "valid-completion-signatures" && href == "/reference/execution/execution/completion_signatures.md") {
                    continue;
                }
                if (text == "FWD-ENV" && href == "/reference/execution/forwarding_query.md") {
                    continue;
                }
                if (text == "run-loop-opstate-base" && href == "/reference/execution/execution/run_loop/run-loop-opstate.md") {
                    continue;
                }
                if (text == "OFFSET" && href == "/reference/mdspan/layout_stride/mapping/op_equal.md") {
                    continue;
                }
                if (text == "map-rank[k]" && href == "/reference/mdspan/submdspan_extents.md") {
                    continue;
                }
                if (text == "(*this)" && href == "/reference/mdspan/layout_stride/mapping/op_call.md") {
                    continue;
                }
                if (text == "m()" && href == "/reference/mdspan/layout_stride/mapping/op_call.md") {
                    continue;
                }
                if (text == "m(z...)" && href == "/reference/mdspan/layout_stride/mapping/op_call.md") {
                    continue;
                }
                if (text == "extents()" && href == "/reference/mdspan/layout_left/mapping.md") {
                    continue;
                }
                if (text == "(*this)" && href == "/reference/mdspan/layout_left/mapping/op_call.md") {
                    continue;
                }
                if (text == "x.extents() == y.extents()" && href == "/reference/mdspan/extents/op_equal.md") {
                    continue;
                }
                if (text == "(*this)" && href == "/reference/mdspan/layout_left_padded/mapping/op_call.md") {
                    continue;
                }
                if (text == "extents()" && href == "/reference/mdspan/layout_right/mapping.md") {
                    continue;
                }
                if (text == "(*this)" && href == "/reference/mdspan/layout_right/mapping/op_call.md") {
                    continue;
                }
                if (text == "(*this)" && href == "/reference/mdspan/layout_right_padded/mapping/op_call.md") {
                    continue;
                }
                if (text == "advanceable" && href == "/reference/ranges/iota_view/iterator.md") {
                    continue;
                }
                if (text == "extended-floating-point-type" && href == "/reference/stdfloat.md") {
                    continue;
                }
                if (text == "node_type" && href == "/reference/node_handle/node_handle.md") {
                    continue;
                }
                if (text == "is_always_unique" && href == "/reference/mdspan/mdspan.md") {
                    continue;
                }
                if (text == "is_even" && href == "/reference/algorithm/is_partitioned.md") {
                    continue;
                }
                if (text == "ECMAScript" && href == "/reference/regex/regex_constants/syntax_option_type.md") {
                    continue;
                }
                if (text == "reference" && href == "/reference/generator/generator.md") {
                    continue;
                }
                if (text == "p.value_" && href == "/reference/generator/generator/promise_type.md") {
                    continue;
                }
                if (text == "value_" && href == "/reference/generator/generator/promise_type.md") {
                    continue;
                }
                if (text == "r.range" && href == "/reference/ranges/elements_of.md") {
                    continue;
                }
                if (text == "r.allocator" && href == "/reference/ranges/elements_of.md") {
                    continue;
                }
                if (text == "ITER_TRAITS" && href == "/reference/iterator/input_iterator.md#iter_concept") {
                    continue;
                }
                if (text == "JOIN-ENV" && href == "/reference/execution/queryable.md") {
                    continue;
                }
                if (text == "SCHED-ENV" && href == "/reference/execution/execution/scheduler.md") {
                    continue;
                }
                if (text == "default-impls" && href == "/reference/execution/execution/impls-for.md") {
                    continue;
                }
                if (text == "TRY-SET-VALUE" && href == "/reference/execution/execution/set_value.md") {
                    continue;
                }
                if (text == "for-each" && href == "/reference/execution/execution/completion_signatures.md") {
                    continue;
                }
                if (text == "SCHED-ATTRS" && href == "/reference/execution/execution/scheduler.md") {
                    continue;
                }
                if (text == "apply" && href == "/reference/execution/execution/product-type.md") {
                    continue;
                }
                if (text == "count-of" && href == "/reference/execution/execution/completion_signatures.md") {
                    continue;
                }
                if (text == "is_always_lock_free" && href == "/reference/atomic/atomic.md") {
                    continue;
                }
                if (text == "TRY-EVAL" && href == "/reference/execution/execution/set_value.md") {
                    continue;
                }
                if (text == "state-type" && href == "/reference/execution/execution/basic-operation.md") {
                    continue;
                }
                if (text == "basic-state" && href == "/reference/execution/execution/basic-operation.md") {
                    continue;
                }
                if (text == "SET-VALUE-SIG" && href == "/reference/execution/execution/set_value.md") {
                    continue;
                }
                if (text == "MATCHING-SIG" && href == "/reference/execution/execution/get_completion_signatures.md") {
                    continue;
                }
                if (text == "await-result-type" && href == "/reference/execution/execution/connect.md") {
                    continue;
                }
                if (text == "template get" && href == "/reference/execution/execution/product-type.md") {
                    continue;
                }
                if (text == "push-back" && href == "/reference/execution/execution/run_loop/run.md") {
                    continue;
                }
                if (text == "execute()" && href == "/reference/execution/execution/run_loop/run-loop-opstate.md") {
                    continue;
                }
                if (text == "sync-wait-result-type" && href == "/reference/execution/this_thread/sync_wait.md") {
                    continue;
                }
                if (text == "AS-EXCEPT-PTR" && href == "/reference/execution/execution/as_awaitable.md") {
                    continue;
                }
                if (text == "sync-wait-state" && href == "/reference/execution/this_thread/sync-wait-receiver.md") {
                    continue;
                }
                if (text == "partially-ordered-with" && href == "/reference/compare/three_way_comparable.md") {
                    continue;
                }
                if (text == "ParseContext" && href == "/reference/format/basic_format_parse_context.md") {
                    continue;
                }
                if (text == "FormatContext" && href == "/reference/format/basic_format_context.md") {
                    continue;
                }
                if (text == "sub_map_offset" && href == "/reference/mdspan/submdspan_mapping_result.md") {
                    continue;
                }
                if (text == "mapping_type" && href == "/reference/mdspan/LayoutMapping.md") {
                    continue;
                }
                if (text == "OtherExtents" && href == "/reference/mdspan/extents.md") {
                    continue;
                }
                if (text == "acc_.access" && href == "/reference/mdspan/AccessorPolicy.md") {
                    continue;
                }
                if (text == "map_" && href == "/reference/mdspan/LayoutMapping.md") {
                    continue;
                }
                if (text == "Sseq" && href == "/reference/random/seed_seq.md") {
                    continue;
                }
                if (text == "feholdexcept" && href == "/reference/cfenv.md") {
                    continue;
                }
                if (text == "r.set_value()" && href == "/reference/execution/execution/system_context_replaceability/receiver_proxy.md") {
                    continue;
                }
                if (text == "r.set_error" && href == "/reference/execution/execution/system_context_replaceability/receiver_proxy.md") {
                    continue;
                }
                if (text == "r.set_stopped()" && href == "/reference/execution/execution/system_context_replaceability/receiver_proxy.md") {
                    continue;
                }
                if (text == "r.execute" && href == "/reference/execution/execution/system_context_replaceability/bulk_item_receiver_proxy.md") {
                    continue;
                }
                if (text == "scope-join-t" && href == "/reference/execution/execution/simple_counting_scope.md") {
                    continue;
                }
                if (text == "stoppable-callback-for" && href == "/reference/stop_token/stoppable_token.md") {
                    continue;
                }
                if (text == "token.wrap" && href == "/reference/execution/execution/scope_token.md") {
                    continue;
                }
                if (text == "template env_type" && href == "/reference/execution/execution/task.md") {
                    continue;
                }
                if (text == "scheduler_type" && href == "/reference/execution/execution/task.md") {
                    continue;
                }
                if (text == "scheduler_type" && href == "/reference/execution/execution/task.md") {
                    continue;
                }
                if (text == "stop_token_type" && href == "/reference/execution/execution/task.md") {
                    continue;
                }
                if (text == "assoc-t" && href == "/reference/execution/execution/counting_scope.md") {
                    continue;
                }
                // constexpr const char* alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
                auto text_small = text;
                std::ranges::transform(text_small, text_small.begin(), [](unsigned char c) {
                    return std::tolower(c);
                });
                auto href_small = href;
                std::ranges::transform(href_small, href_small.begin(), [](unsigned char c) {
                    return std::tolower(c);
                });
                std::string_view text2 = text_small;
                std::string_view href2 = href_small;
                if (href2.ends_with(".nolink")) {
                    href2.remove_suffix(7);
                }
                if (text2 == "<std>") {
                    if (href2 == "/module/std.md") {
                        continue;
                    }
                }
                else if (text2 == "<std.compat>" || text2 == "std.compat") {
                    if (href2 == "/module/std.compat.md") {
                        continue;
                    }
                }
                else if (text2.starts_with('<') && text2.ends_with('>') && not text2.contains('=')) {
                    text2.remove_prefix(1);
                    text2.remove_suffix(1);
                    if (href2 == std::format("/reference/{}.md", text2)) {
                        continue;
                    }
                }
                else
                //{continue;}if(true);else
                if (text2 == "exposition-only") {
                    if (href2 == "/reference/exposition-only.md") {
                        continue;
                    }
                }
                else if (text2.ends_with("_policy") && href2 == "/reference/execution/execution/execution_policy.md") {
                    continue;
                }
                else if (text2 == "trivially_destructible") {
                    if (href2 == "/reference/type_traits/is_trivially_destructible.md") {
                        continue;
                    }
                }
                else if (text2 == "new") {
                    if (href2 == "/reference/new/op_new.md") {
                        continue;
                    }
                }
                else if (text2 == "new[]") {
                    if (href2 == "/reference/new/op_new[].md") {
                        continue;
                    }
                }
                else if (text2.ends_with("_lock_free") && href2 == "/reference/atomic/lock_free_property.md") {
                    continue;
                }
                else if (text2.ends_with("sub_match") && href2 == "/reference/regex/sub_match.md") {
                    continue;
                }
                else if (text2.ends_with("regex") && href2 == "/reference/regex/basic_regex.md") {
                    continue;
                }
                else if (text2.ends_with("match") && href2 == "/reference/regex/match_results.md") {
                    continue;
                }
                else if (text2.ends_with("regex_iterator") && href2 == "/reference/regex/regex_iterator.md") {
                    continue;
                }
                else if (text2.ends_with("regex_token_iterator") && href2 == "/reference/regex/regex_token_iterator.md") {
                    continue;
                }
                else if (std::ranges::contains(std::vector{ "promise_already_satisfied", "no_state", "broken_promise", "future_already_retrieved" }, text2)) {
                    if (href2 == "/reference/future/future_errc.md") {
                        continue;
                    }
                }
                else if (std::ranges::contains(std::vector{ "erange", "edom", "enotdir", "etimedout" }, text2)) {
                    if (href2 == "/reference/cerrno.md") {
                        continue;
                    }
                }
                else if (text2.contains("future_errc::")) {
                    if (href2 == "/reference/future/future_errc.md") {
                        continue;
                    }
                }
                else if (text2.starts_with("file_type::")) {
                    if (href2 == "/reference/filesystem/file_type.md") {
                        continue;
                    }
                }
                else if (text2.contains("perms::")) {
                    if (href2 == "/reference/filesystem/perms.md") {
                        continue;
                    }
                }
                else if (text2.contains("perm_options::")) {
                    if (href2 == "/reference/filesystem/perm_options.md") {
                        continue;
                    }
                }
                else if (text2.contains("copy_options::")) {
                    if (href2 == "/reference/filesystem/copy_options.md") {
                        continue;
                    }
                }
                else if (text2.starts_with("space_info::")) {
                    if (href2 == "/reference/filesystem/space_info.md") {
                        continue;
                    }
                }
                else if (text2.contains("directory_options::")) {
                    if (href2 == "/reference/filesystem/directory_options.md") {
                        continue;
                    }
                }
                else if (text2.starts_with("cv_status::")) {
                    if (href2 == "/reference/condition_variable/cv_status.md") {
                        continue;
                    }
                }
                else if (text2.starts_with("future_status::")) {
                    if (href2 == "/reference/future/future_status.md") {
                        continue;
                    }
                }
                else if (text2.contains("launch::")) {
                    if (href2 == "/reference/future/launch.md") {
                        continue;
                    }
                }
                else if ((text2.starts_with("extents::") || text2.starts_with("extents_")) && href2 == "/reference/mdspan/extents.md") {
                    continue;
                }
                else if (text2.starts_with("layout_left::") && href2 == "/reference/mdspan/layout_left.md") {
                    continue;
                }
                else if (text2.starts_with("layout_right::") && href2 == "/reference/mdspan/layout_right.md") {
                    continue;
                }
                else if (text2.starts_with("layout_stride::") && href2 == "/reference/mdspan/layout_stride.md") {
                    continue;
                }
                else if (text2.contains("regex_constants::error_") && href2 == "/reference/regex/regex_constants/error_type.md") {
                    continue;
                }
                else if (text2.contains("regex_constants::format_") && href2 == "/reference/regex/regex_constants/match_flag_type.md") {
                    continue;
                }
                else if (text2.contains("regex_constants::match_") && href2 == "/reference/regex/regex_constants/match_flag_type.md") {
                    continue;
                }
                else if (text2.contains("regex_constants::") && href2 == "/reference/regex/regex_constants/syntax_option_type.md") {
                    continue;
                }
                else if (text2.starts_with("range_format::") && href2 == "/reference/format/range_format.md") {
                    continue;
                }
                else if ((text2.starts_with("mapping_type::") || text2.starts_with("map_.")) && href2 == "/reference/mdspan/layoutmapping.md") {
                    continue;
                }
                else if (text2.starts_with("std::strong_ordering::") && href2 == "/reference/compare/strong_ordering.md") {
                    continue;
                }
                else if (text2.starts_with("pool_options::") && href2 == "/reference/memory_resource/pool_options.md") {
                    continue;
                }
                else if (text2.starts_with("forward_progress_guarantee::") && href2 == "/reference/execution/execution/forward_progress_guarantee.md") {
                    continue;
                }
                else if (text2.starts_with("traits::") && href2 == "/reference/string/char_traits.md") {
                    continue;
                }
                else if (text2.starts_with("layoutleftpaddedmapping") && href2 == "/reference/mdspan/layout_left_padded/mapping.md") {
                    continue;
                }
                else if (text2.starts_with("layoutrightpaddedmapping") && href2 == "/reference/mdspan/layout_right_padded/mapping.md") {
                    continue;
                }
                else if (text2.starts_with("subrange_kind::") && href2 == "/reference/ranges/subrange_kind.md") {
                    continue;
                }
                else if (text2.contains("choose::") && href2 == "/reference/chrono/choose.md") {
                    continue;
                }
                else if (text2.contains("io_errc::") && href2 == "/reference/ios/io_errc.md") {
                    continue;
                }
                else if (text2.contains("cv_status::") && href2 == "/reference/condition_variable/cv_status.md") {
                    continue;
                }
                else if (text2.ends_with("_event") && href2 == "/reference/ios/ios_base/type-event.md") {
                    continue;
                }
                else if (text2.ends_with("_pool_resource") && href2 == "/reference/memory_resource/pool_resource.md") {
                    continue;
                }
                else if ((text2 == "in-vector" || text2 == "out-vector") && href2 == "/reference/linalg/inout-vector.md") {
                    continue;
                }
                else if ((text2 == "in-matrix" || text2 == "out-matrix") && href2 == "/reference/linalg/inout-matrix.md") {
                    continue;
                }
                else if ((text2 == "in-object" || text2 == "out-object") && href2 == "/reference/linalg/inout-object.md") {
                    continue;
                }
                else if ((text2.starts_with("accessor") || text2.ends_with("accessor") || text2.ends_with("policy")) && href2 == "/reference/mdspan/accessorpolicy.md") {
                    continue;
                }
                else if ((text2 == "layout" || text2.ends_with("policy")) && href2 == "/reference/mdspan/layoutmappingpolicy.md") {
                    continue;
                }
                else if (text2.contains("denorm_") && href2 == "/reference/limits/float_denorm_style.md") {
                    continue;
                }
                else if (text2.contains("round_") && href2 == "/reference/limits/float_round_style.md") {
                    continue;
                }
                else {
                    if (text2.contains(" - ")) {
                        text2.remove_suffix(text2.size() - text2.rfind(" - "));
                    }
                    if (text2.starts_with("template<class t> struct ")) {
                        text2.remove_prefix(25);
                    }
                    if (text2.starts_with('"')) {
                        text2.remove_prefix(text2.rfind('"') + 1);
                    }
                    if (text2.starts_with("if (")) {
                        text2 = "op_bool";
                    }
                    if (text2.ends_with(')') && href2.ends_with("op_call.md")) {
                        text2 = "op_call";
                    }
                    if (text2.ends_with('(')) {
                        text2.remove_suffix(1);
                    }
                    if (text2.ends_with(';')) {
                        text2.remove_suffix(1);
                    }
                    if (text2.ends_with("::value_type")) {
                        text2.remove_suffix(12);
                    }
                    if (text2.ends_with("::reference")) {
                        text2.remove_suffix(11);
                    }
                    if (text2.ends_with("::pointer")) {
                        text2.remove_suffix(9);
                    }
                    if (text2.ends_with('}') && std::ranges::count(text2, '{') == std::ranges::count(text2, '}')) {
                        text2.remove_suffix(1);
                        for (int nest = 1; nest > 0 && text2.size(); text2.remove_suffix(1)) {
                            if (text2.back() == '}') {
                                ++nest;
                            }
                            else if (text2.back() == '{') {
                                --nest;
                            }
                        }
                    }
                    if (text2.ends_with(')') && std::ranges::count(text2, '(') == std::ranges::count(text2, ')') && not text2.starts_with('(') && not text2.starts_with("decltype") && not text2.contains("operator()") && not text2.ends_with("(size_type)")) {
                        text2.remove_suffix(1);
                        for (int nest = 1; nest > 0 && text2.size(); text2.remove_suffix(1)) {
                            if (text2.back() == ')') {
                                ++nest;
                            }
                            else if (text2.back() == '(') {
                                --nest;
                            }
                        }
                    }
                    if (text2.ends_with('>') && std::ranges::count(text2, '<') == std::ranges::count(text2, '>') && not text2.contains("<=>")) {
                        text2.remove_suffix(1);
                        for (int nest = 1; nest > 0 && text2.size(); text2.remove_suffix(1)) {
                            if (text2.back() == '>') {
                                ++nest;
                            }
                            else if (text2.back() == '<') {
                                --nest;
                            }
                        }
                    }
                    if (text2.contains("::")) {
                        text2.remove_prefix(text2.rfind("::") + 2);
                    }
                    if (text2.contains('.') && not text2.contains("..")) {
                        text2.remove_prefix(text2.rfind('.') + 1);
                    }
                    if (text2.contains("->") && not text2.contains("operator->")) {
                        text2.remove_prefix(text2.rfind("->") + 2);
                    }
                    if (text2 == "three_way_comparable_with") {
                        text2.remove_suffix(5);
                    }
                    if (text2.starts_with("explicit ")) {
                        text2.remove_prefix(9);
                    }
                    if (text2.starts_with("template ")) {
                        text2.remove_prefix(9);
                    }
                    if (href2.contains('/')) {
                        href2.remove_prefix(href2.rfind('/') + 1);
                    }
                    if (href2.contains('#')) {
                        href2.remove_prefix(href2.find('#') + 1);
                    }
                    if (href2.ends_with(".md")) {
                        href2.remove_suffix(3);
                    }
                    if (href2.ends_with("_free") && not text2.ends_with("_free")) {
                        href2.remove_suffix(5);
                    }
                    if (href2.ends_with("_view") && (text.contains("views::") || text2 == href2.substr(0, href2.size() - 5))) {
                        href2.remove_suffix(5);
                    }
                    if (href2.starts_with("ranges_") && (text.contains("ranges::") || entry.path().string().contains("ranges"))) {
                        href2.remove_prefix(7);
                    }
                    if (href2.starts_with("basic_") && not text2.starts_with("basic_")) {
                        href2.remove_prefix(6);
                    }
                    if (href2.ends_with("_t") && not (text2.ends_with("_t") || text2.ends_with(" t") || text2.starts_with("operator"))) {
                        href2.remove_suffix(2);
                    }
                    if (text2.ends_with("_t") && not href2.ends_with("_t")) {
                        text2.remove_suffix(2);
                    }
                    if (text2.ends_with("-t") && not href2.ends_with("-t")) {
                        text2.remove_suffix(2);
                    }
                    if (text2.ends_with("_v") && not href2.ends_with("_v")) {
                        text2.remove_suffix(2);
                    }
                    if (text2.ends_with("_with") && not href2.ends_with("with")) {
                        text2.remove_suffix(5);
                    }
                    if (text2.ends_with("-with") && not href2.ends_with("with")) {
                        text2.remove_suffix(5);
                    }
                    if (text2.ends_with("_like") && not href2.ends_with("like")) {
                        text2.remove_suffix(5);
                    }
                    if (text2.ends_with("-like") && not href2.ends_with("like")) {
                        text2.remove_suffix(5);
                    }
                    if (text2.ends_with("f") && text2.substr(0, text2.size() - 1) == href2) {
                        text2.remove_suffix(1);
                    }
                    if (text2.ends_with("l") && text2.substr(0, text2.size() - 1) == href2) {
                        text2.remove_suffix(1);
                    }
                    if (href2.starts_with("type-") && not text2.starts_with("type-")) {
                        href2.remove_prefix(5);
                    }
                    if (href2.ends_with("-size_type") && not text2.contains("size_type")) {
                        href2.remove_suffix(10);
                    }
                    std::string t{ text2 };
                    if (t == href2) {
                        // No need to change
                    }
                    else if (t == "decltype(auto)") {
                        t = "decltype_auto";
                    }
                    else if (t == "unexpected_handler") {
                        t = "set_unexpected";
                    }
                    else if (t == "not1" || t == "not2" || t == "unary_negate" || t == "binary_negate") {
                        t = "negators";
                    }
                    else if (std::ranges::contains(std::vector{ "domain_error", "invalid_argument", "length_error", "logic_error", "out_of_range", "runtime_error", "range_error", "overflow_error", "underflow_error" }, t) && href2 == "stdexcept") {
                        t = "stdexcept";
                    }
                    else if (std::ranges::contains(std::vector{ "quecto", "ronto", "yocto", "zepto", "atto", "femto", "pico", "nano", "micro", "milli", "centi", "deci", "deca", "hecto", "kilo", "mega", "giga", "tera", "peta", "exa", "zetta", "yotta", "ronna", "quetta" }, t)) {
                        t = "si_prefix";
                    }
                    else if (std::ranges::contains(std::vector{ "failbit", "eofbit", "badbit", "goodbit" }, t)) {
                        t = "iostate";
                    }
                    else if (std::ranges::contains(std::vector{ "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" }, t)) {
                        t = "weekday_constants";
                    }
                    else if (std::ranges::contains(std::vector{ "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december" }, t)) {
                        t = "month_constants";
                    }
                    else if (std::ranges::contains(std::vector{ "nanoseconds", "microseconds", "milliseconds", "seconds", "minutes", "hours", "days", "weeks", "years", "months" }, t)) {
                        t = "duration_aliases";
                    }
                    else if (std::ranges::contains(std::vector{ "seq", "par", "par_unseq", "unseq" }, t)) {
                        t = "execution_policy";
                    }
                    else if (std::ranges::contains(std::vector{ "app", "ate", "binary", "in", "out", "noreplace", "trunc" }, t)) {
                        t = "openmode";
                    }
                    else if (std::ranges::contains(std::vector{ "relaxed", "consume", "acquire", "release", "acq_rel", "seq_cst" }, t)) {
                        t = "memory_order";
                    }
                    else if (std::ranges::contains(std::vector{ "is_eq", "is_neq", "is_lt", "is_lteq", "is_gt", "is_gteq" }, t)) {
                        t = "named_comparison_functions";
                    }
                    else if (std::ranges::contains(std::vector{ "atoi", "atol", "atoll" }, t)) {
                        t = "atoi_atol_atoll";
                    }
                    else if (std::ranges::contains(std::vector{ "nan", "nanl" }, t)) {
                        t = "nanf";
                    }
                    else if (std::ranges::contains(std::vector{ "labs", "llabs" }, t)) {
                        t = "abs";
                    }
                    else if (std::ranges::contains(std::vector{ "ldiv", "lldiv" }, t)) {
                        t = "div";
                    }
                    else if (std::ranges::contains(std::vector{ "dbl_mant_dig", "flt_mant_dig" }, t)) {
                        t = "ldbl_mant_dig";
                    }
                    else if (std::ranges::contains(std::vector{ "keys_view", "values_view" }, t)) {
                        t = "elements_view";
                    }
                    else if (std::ranges::contains(std::vector{
                        "resource_unavailable_try_again",
                        "operation_not_permitted",
                        "invalid_argument",
                        "no_such_file_or_directory",
                        "is_a_directory",
                        "permission_denied",
                        "resource_deadlock_would_occur",
                        "device_or_resource_busy",
                        "result_out_of_range",
                        "value_too_large",
                    }, t) && href2 == "errc") {
                        t = "errc";
                    }
                    else if (std::ranges::contains(std::vector{ "keys", "values" }, t) && href2 == "elements") {
                        t = "elements";
                    }
                    else if (std::ranges::contains(std::vector{ "keys", "values" }, t) && href2 == "containers") {
                        t = "containers";
                    }
                    else if (std::ranges::contains(std::vector{ "beg", "cur", "end" }, t) && href2 == "seekdir") {
                        t = "seekdir";
                    }
                    else if (std::ranges::contains(std::vector{ "native_format", "generic_format", "auto_format" }, t) && href2 == "format") {
                        t = "format";
                    }
                    else if (text2 == "pairwise" && href2 == "adjacent") {
                        t = "adjacent";
                    }
                    else if (text2 == "strtod") {
                        t = "stod";
                    }
                    else if (text2 == "last" && href2 == "last_spec") {
                        t = "last_spec";
                    }
                    else if (text2.starts_with("local") && href2 == "local_time") {
                        t = "local_time";
                    }
                    else if (text2.starts_with("atomic_") && text2.ends_with("signed_lock_free")) {
                        t = "atomic";
                    }
                    else if (text2.starts_with("memory_order_")) {
                        t = "memory_order";
                    }
                    else if (t.ends_with("_iterator_tag")) {
                        t = "iterator_tag";
                    }
                    else if (t.ends_with("(size_type)")) {
                        t = t.substr(0, t.size() - 11) + "-size_type";
                    }
                    else if (t.ends_with("-box")) {
                        std::ranges::replace(t, '-', '_');
                    }
                    else if (t.contains("regular_")) {
                        t.erase(t.find("regular_"), 8);
                    }
                    else if ((t == "wstring" || t == "u8string" || t == "u16string" || t == "u32string") && href2 == "string") {
                        t = "string";
                    }
                    else if (t == "wstring_view" || t == "u8string_view" || t == "u16string_view" || t == "u32string_view") {
                        t = "string_view";
                    }
                    else if (t == "make_wformat_args") {
                        t = "make_format_args";
                    }
                    else if ((t[0] == 'w' || t[0] == '_') && t.substr(1) == href2) {
                        t = t.substr(1);
                    }
                    else if (t == "dextents" || t == "dims") {
                        t = "extents";
                    }
                    else if (t == "insert_return_type") {
                        t = "map";
                    }
                    else if (t == "noop_coroutine_promise") {
                        t = "noop_coroutine_handle";
                    }
                    else if (t == "_exit") {
                        t = "exit_";
                    }
                    else if (t == "sys_days" || t == "sys_seconds") {
                        t = "sys_time";
                    }
                    else if (t == "utc_seconds") {
                        t = "utc_time";
                    }
                    else if (t == "tai_seconds") {
                        t = "tai_time";
                    }
                    else if (t == "gps_seconds") {
                        t = "gps_time";
                    }
                    else if (t == "zoned_seconds") {
                        t = "zoned_time";
                    }
                    else if (t == "binary_semaphore") {
                        t = "counting_semaphore";
                    }
                    else if (t.ends_with("_result") && href2.ends_with("_result")) {
                        t = href2;
                    }
                    else if (t[0] == '_' && href2 == "placeholders") {
                        t = "placeholders";
                    }
                    else if (href_small.substr(0, href_small.rfind('/')).ends_with(t)) {
                        t = "op_constructor";
                    }
                    else if (t.starts_with('~')) {
                        t = "op_destructor";
                    }
                    else if (t == "(constructor)") {
                        t = "op_constructor";
                    }
                    else if (t == "(destructor)") {
                        t = "op_destructor";
                    }
                    else if (t == "(deduction_guide)") {
                        t = "op_deduction_guide";
                    }
                    else if (t == "(initializer)") {
                        t = "op_initializer";
                    }
                    else if (t.starts_with("operator \"\"") || t.starts_with("operator\"\" ")) {
                        t = "op_" + t.substr(11);
                    }
                    else if (t == "operator==" || t.contains("==")) {
                        t = "op_equal";
                    }
                    else if (t == "operator!=" || t == "!=") {
                        if (href2 == "op_equal") {
                            t = "op_equal";
                        }
                        else {
                            t = "op_not_equal";
                        }
                    }
                    else if (t == "operator<" || t == "<") {
                        t = "op_less";
                    }
                    else if (t == "operator>" || t == ">") {
                        t = "op_greater";
                    }
                    else if (t == "operator<=" || t == "<=") {
                        t = "op_less_equal";
                    }
                    else if (t == "operator>=" || t == ">=") {
                        t = "op_greater_equal";
                    }
                    else if (t == "operator<=>" || t == "<=>") {
                        t = "op_compare_3way";
                    }
                    else if (t == "operator+") {
                        if (href2 == "op_unary_plus") {
                            t = "op_unary_plus";
                        }
                        else {
                            t = "op_plus";
                        }
                    }
                    else if (t == "operator-") {
                        if (href2 == "op_unary_minus") {
                            t = "op_unary_minus";
                        }
                        else {
                            t = "op_minus";
                        }
                    }
                    else if (t == "operator*" || (t.starts_with("*") && href2 == "op_deref")) {
                        if (href2 == "op_deref") {
                            t = "op_deref";
                        }
                        else {
                            t = "op_multiply";
                        }
                    }
                    else if (t == "operator/") {
                        if (href2 == "op_append") {
                            t = "op_append";
                        }
                        else {
                            t = "op_divide";
                        }
                    }
                    else if (t == "operator%") {
                        t = "op_modulo";
                    }
                    else if (t == "operator<<" || t == "<<") {
                        if (href2 == "op_ostream") {
                            t = "op_ostream";
                        }
                        else {
                            t = "op_left_shift";
                        }
                    }
                    else if (t == "operator>>" || t == ">>") {
                        if (href2 == "op_istream") {
                            t = "op_istream";
                        }
                        else {
                            t = "op_right_shift";
                        }
                    }
                    else if (t == "operator|") {
                        t = "op_or";
                    }
                    else if (t == "operator&" || t.contains(" & ")) {
                        t = "op_and";
                    }
                    else if (t == "operator^") {
                        t = "op_xor";
                    }
                    else if (t == "operator new") {
                        t = "op_new";
                    }
                    else if (t == "operator new[]") {
                        t = "op_new[]";
                    }
                    else if (t == "operator delete") {
                        t = "op_delete";
                    }
                    else if (t == "operator delete[]") {
                        t = "op_delete[]";
                    }
                    else if (t == "operator[]" || (t.ends_with(']') && href2 == "op_at")) {
                        t = "op_at";
                    }
                    else if (t == "operator=") {
                        t = "op_assign";
                    }
                    else if (t == "operator&=" || t == "&=") {
                        t = "op_and_assign";
                    }
                    else if (t == "operator|=" || t == "|=") {
                        t = "op_or_assign";
                    }
                    else if (t == "operator^=" || t == "^=") {
                        t = "op_xor_assign";
                    }
                    else if (t == "operator+=" || t == "+=" || t.contains(" += ")) {
                        t = "op_plus_assign";
                    }
                    else if (t == "operator-=" || t == "-=") {
                        t = "op_minus_assign";
                    }
                    else if (t == "operator*=" || t == "*=") {
                        t = "op_multiply_assign";
                    }
                    else if (t == "operator/=" || t == "/=") {
                        if (href2 == "op_append_assign") {
                            t = "op_append_assign";
                        }
                        else {
                            t = "op_divide_assign";
                        }
                    }
                    else if (t == "operator%=" || t == "%=") {
                        t = "op_modulo_assign";
                    }
                    else if (t == "operator<<=" || t == "<<=") {
                        t = "op_left_shift_assign";
                    }
                    else if (t == "operator>>=" || t == ">>=") {
                        t = "op_right_shift_assign";
                    }
                    else if (t == "operator~") {
                        t = "op_flip";
                    }
                    else if (t == "operator()") {
                        t = "op_call";
                    }
                    else if (t == "operator->") {
                        t = "op_arrow";
                    }
                    else if (t == "operator++" || t.ends_with("++")) {
                        t = "op_increment";
                    }
                    else if (t == "operator--" || t.ends_with("--")) {
                        t = "op_decrement";
                    }
                    else if (t == "operator!") {
                        t = "op_not";
                    }
                    else if (t == "operator&&") {
                        t = "op_logical_and";
                    }
                    else if (t == "operator||") {
                        t = "op_logical_or";
                    }
                    else if (t.starts_with("operator ")) {
                        auto type = t.substr(9);
                        if (type == "void*") {
                            type = "voidptr";
                        }
                        else if (type == "void**") {
                            type = "voidpp";
                        }
                        else if (type == "t&") {
                            type = "cast_ref_t";
                        }
                        else if (type == "pointer*") {
                            type = "pointer";
                        }
                        t = "op_" + type;
                    }
                    else if ("op_" + t == href2) {
                        t = "op_" + t;
                    }
                    else if (t.contains('-') && not href2.contains('-')) {
                        std::ranges::replace(t, '-', '_');
                    }
                    std::ranges::replace(t, ' ', '_');
                    if (t == href2) {
                        continue;
                    }
                    // std::cout << "debug: " << t << " != " << href2 << "\n";
                }
                print_error(pos, std::format("Invalid link: 🔗 {} ➡️ {}", text, href));
                // std::cout << "if (text == \"" << text << "\" && href == \"" << href << "\") {\n";
                // std::cout << "    continue;\n";
                // std::cout << "}\n";
                if (error_count > 1000) {
                    std::cout << "Too many errors, stopping.\n";
                    return 1;
                }
            }
        }
    }
    if (error_count) {
        std::cerr << "\e[K" << error_count << " error" << (error_count == 1 ? "" : "s") << " generated.\n";
    }
}
