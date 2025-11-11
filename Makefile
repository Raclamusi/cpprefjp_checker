CXX = g++-15
CXXFLAGS := -std=c++26 -pedantic -fmodules -Wno-attributes
FUTURE_LIST = \
	$(shell find codes/lang/cpp26 -name 'embed.*.cpp') \
	$(shell find codes/reference/linalg -name '*.cpp') \
	$(shell find codes/reference/execution/execution -name '*.cpp') \
	$(shell find codes/reference/ranges/filter_view/iterator -name '*.cpp') \
	$(shell find codes/reference/stacktrace -name '*.cpp') \
	$(shell find codes/reference/functional/copyable_function -name '*.cpp') \
	$(shell find codes/reference/functional/function_ref -name '*.cpp') \
	$(shell find codes/reference/rcu -name '*.cpp') \
	$(shell find codes/reference/debugging -name '*.cpp') \
	$(shell find codes/reference/atomic -name '*.cpp26.cpp') \
	$(shell find codes/reference/mdspan -name '*.cpp') \
	$(shell find codes/reference/random -name '*.cpp26.cpp') \
	$(shell find codes/reference/stop_token -name '*.cpp26.cpp') \
	$(shell find codes/reference/cmath -name '*.cpp26.cpp') \
	codes/reference/bitset/bitset/op_constructor.1.cpp03.cpp \
	codes/reference/ranges/chunk_view/op_constructor.1.cpp23.cpp \
	codes/reference/ranges/elements_of.1.cpp23.cpp \
	codes/reference/ranges/adjacent_view/op_deduction_guide.1.cpp23.cpp \
	codes/reference/ranges/adjacent_transform_view/op_deduction_guide.1.cpp23.cpp \
	codes/reference/ranges/elements_view/op_constructor.1.cpp20.cpp \
	codes/reference/ranges/elements_view/base.1.cpp20.cpp \
	codes/reference/ranges/elements_view/size.1.cpp20.cpp \
	codes/reference/ranges/elements_view/begin.1.cpp20.cpp \
	codes/reference/ranges/elements_view/end.1.cpp20.cpp \
	codes/reference/ranges/slide_view/op_constructor.1.cpp23.cpp \
	codes/reference/ranges/stride_view/op_constructor.1.cpp23.cpp \
	codes/reference/memory/undeclare_no_pointers.1.cpp11.cpp \
	codes/reference/memory/allocator_traits/allocate_at_least.1.cpp23.cpp \
	codes/reference/memory/allocator/allocate_at_least.1.cpp23.cpp \
	codes/reference/memory/is_sufficiently_aligned.1.cpp26.cpp \
	codes/reference/cassert/assert.3.cpp03.cpp \
	codes/reference/memory/start_lifetime_as.1.cpp23.cpp \
	codes/reference/memory/start_lifetime_as_array.1.cpp23.cpp \
	codes/reference/chrono/file_time.1.cpp20.cpp \
	codes/reference/chrono/sys_info/formatter.1.cpp20.cpp \
	codes/reference/chrono/local_info/formatter.1.cpp20.cpp \
	codes/reference/flat_map/flat_map/erase_if_free.1.cpp23.cpp \
	codes/reference/flat_map/flat_multimap/erase_if_free.1.cpp23.cpp \
	codes/reference/flat_set/flat_set/erase_if_free.1.cpp23.cpp \
	codes/reference/flat_set/flat_multiset/erase_if_free.1.cpp23.cpp \
	codes/reference/scoped_allocator/scoped_allocator_adaptor/op_deduction_guide.1.cpp17.cpp \
	codes/reference/type_traits/is_corresponding_member.1.cpp20.cpp \
	codes/reference/type_traits/is_pointer_interconvertible_with_class.1.cpp20.cpp \
	codes/reference/type_traits/is_within_lifetime.1.cpp26.cpp \
	codes/reference/type_traits/is_implicit_lifetime.1.cpp23.cpp \
	codes/reference/type_traits/is_trivially_copyable.2.cpp11.cpp \
	codes/reference/filesystem/path/formatter.1.cpp26.cpp \
	codes/reference/algorithm/ranges_ends_with.1.cpp23.cpp \
	codes/reference/algorithm/ranges_starts_with.1.cpp23.cpp \
	codes/reference/algorithm/ranges_shift_right.1.cpp23.cpp \
	codes/reference/algorithm/ranges_shift_left.1.cpp23.cpp \
	codes/reference/algorithm/ranges_replace_copy_if.2.cpp20.cpp \
	codes/reference/algorithm/replace_copy_if.2.cpp03.cpp \
	codes/reference/algorithm/ranges_replace_copy.2.cpp20.cpp \
	codes/reference/sstream/basic_stringbuf/op_constructor.2.cpp03.cpp \
	codes/reference/sstream/basic_stringbuf/str.3.cpp03.cpp \
	codes/reference/sstream/basic_ostringstream/op_constructor.2.cpp03.cpp \
	codes/reference/sstream/basic_ostringstream/str.3.cpp03.cpp \
	codes/reference/sstream/basic_stringstream/op_constructor.2.cpp03.cpp \
	codes/reference/sstream/basic_stringstream/str.3.cpp03.cpp \
	codes/reference/sstream/basic_istringstream/op_constructor.2.cpp03.cpp \
	codes/reference/sstream/basic_istringstream/str.3.cpp03.cpp \
	codes/reference/generator/generator.2.cpp23.cpp \
	codes/reference/span/span/op_deduction_guide.1.cpp20.cpp \
	codes/reference/compare/weak_order.1.cpp20.cpp \
	codes/reference/compare/partial_order.1.cpp20.cpp \
	codes/reference/compare/strong_order.1.cpp20.cpp \
	codes/reference/format/format.5.cpp20.cpp \
	codes/reference/format/make_format_args.1.cpp20.cpp \
	codes/reference/format/vformat.1.cpp20.cpp \
	codes/reference/string/basic_string/op_deduction_guide.1.cpp17.cpp \
	codes/reference/mutex/timed_mutex/native_handle.1.cpp11.cpp \
	codes/reference/mutex/recursive_timed_mutex/native_handle.1.cpp11.cpp \
	codes/reference/functional/copyable_function.1.cpp26.cpp \
	codes/reference/functional/copyable_function.2.cpp26.cpp \
	codes/reference/functional/copyable_function.3.cpp26.cpp \
	codes/reference/functional/function_ref.1.cpp26.cpp \
	codes/reference/utility/nontype_t.1.cpp26.cpp \
	codes/module/std.3.cpp23.cpp \
	codes/reference/print/print.2.cpp23.cpp \
	codes/reference/print/println.2.cpp23.cpp \
	codes/reference/string/basic_string/subview.1.cpp26.cpp \
	codes/reference/string/basic_string/subview.2.cpp26.cpp \
	codes/reference/string_view/basic_string_view/subview.1.cpp26.cpp
IGNORE_LIST = \
	codes/article/platform/locales.1.cpp03.cpp \
	codes/article/lib/at_thread_exit.1.cpp03.cpp \
	codes/article/lib/at_thread_exit.2.cpp03.cpp \
	codes/module/std.2.cpp23.cpp \
	codes/module/std.4.cpp23.cpp \
	codes/lang/cpp11/range_based_for.5.cpp11.cpp \
	codes/lang/cpp11/override_final.1.cpp11.cpp \
	codes/lang/cpp11/decltype.6.cpp11.cpp \
	codes/lang/cpp17/if_constexpr.1.cpp17.cpp \
	codes/lang/cpp17/if_constexpr.2.cpp17.cpp \
	codes/lang/cpp17/if_constexpr.3.cpp17.cpp \
	codes/lang/cpp17/if_constexpr.4.cpp17.cpp \
	codes/lang/cpp17/if_constexpr.5.cpp17.cpp \
	codes/lang/cpp17/extending_static_assert.1.cpp17.cpp \
	codes/lang/cpp17/remove_deprecated_increment_of_bool.1.cpp17.cpp \
	codes/lang/cpp17/remove_deprecated_increment_of_bool.3.cpp17.cpp \
	codes/lang/cpp17/inline_variables.1.cpp17.cpp \
	codes/lang/cpp17/using_attribute_namespaces.1.cpp17.cpp \
	codes/lang/cpp17/construction_enum_class_values.1.cpp17.cpp \
	codes/lang/cpp20/using_enum.1.cpp20.cpp \
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.1.cpp20.cpp \
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.2.cpp20.cpp \
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.3.cpp20.cpp \
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.4.cpp20.cpp \
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.5.cpp20.cpp \
	codes/lang/cpp20/immediate_functions.1.cpp20.cpp \
	codes/lang/cpp20/char8_t.1.cpp20.cpp \
	codes/lang/cpp26/erroneous_behavior_for_uninitialized_reads.4.cpp26.cpp \
	codes/lang/cpp26/user-generated_static_assert_messages.1.cpp26.cpp \
	codes/lang/future/contract-based_programming.1.future.cpp \
	codes/reference/memory/assume_aligned.2.cpp20.cpp \
	codes/reference/cstdlib/malloc.1.cpp03.cpp \
	codes/reference/cstdlib/calloc.1.cpp03.cpp \
	codes/reference/cstdlib/free.1.cpp03.cpp \
	codes/reference/memory/inout_ptr.1.cpp23.cpp \
	codes/reference/memory/out_ptr.1.cpp23.cpp \
	codes/reference/functional/function/op_constructor.1.cpp11.cpp \
	codes/reference/memory/declare_no_pointers.1.cpp11.cpp \
	codes/reference/type_traits/is_invocable_r.1.cpp17.cpp \
	codes/reference/filesystem/path.2.cpp17.cpp \
	codes/reference/filesystem/path/op_istream.2.cpp17.cpp \
	codes/reference/fstream/basic_filebuf/native_handle.1.cpp26.cpp \
	codes/reference/fstream/basic_fstream/native_handle.1.cpp26.cpp \
	codes/reference/fstream/basic_ofstream/native_handle.1.cpp26.cpp \
	codes/reference/fstream/basic_ifstream/native_handle.1.cpp26.cpp \
	codes/reference/cfenv/fegetexceptflag.1.cpp11.cpp \
	codes/reference/cfenv/fexcept_t.1.cpp11.cpp \
	codes/reference/string_view/basic_string_view/op_constructor.1.cpp17.cpp \
	codes/reference/thread/jthread/native_handle.1.cpp20.cpp \
	codes/reference/thread/thread/native_handle.1.cpp11.cpp \
	codes/reference/functional/function/assign.1.cpp11.cpp \
	codes/lang/cpp26/add_atsign_dollar_graveaccent_to_the_basic_character_set.1.cpp26.cpp \
	codes/lang/cpp11/decltype.4.cpp11.cpp \
	codes/lang/cpp11/char16_32.3.cpp11.cpp \
	codes/lang/cpp11/raw_string_literals.6.cpp11.cpp \
	codes/lang/cpp11/utf8_string_literals.1.cpp11.cpp \
	codes/reference/filesystem/u8path.1.cpp17.cpp \
	codes/reference/type_traits/reference_constructs_from_temporary.1.cpp23.cpp \
	codes/reference/type_traits/reference_converts_from_temporary.1.cpp23.cpp \
	${FUTURE_LIST}
SRC_ALL = $(shell find codes -name '*.cpp')
SRC = $(filter-out $(IGNORE_LIST), $(SRC_ALL))
OBJ = $(SRC:.cpp=.o)
EXISTING_OBJ = $(wildcard $(OBJ))
EXE = $(EXISTING_OBJ:.o=.out)
EXISTING_EXE = $(wildcard $(EXE))
STDOUT = $(EXISTING_EXE:.out=.stdout)

.PHONY: all obj exe run clean clean_obj clean_exe clean_stdout

all: obj exe

obj: $(OBJ)

exe: $(EXE)

run: $(STDOUT)

%.o: %.cpp
	@echo
	@echo
	@echo
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.out: %.o
	$(CXX) $(CXXFLAGS) $< -o $@

%.stdout: %.out
	dir=$$(mktemp -d run/XXXXXXXX); cd $$dir; gtimeout 10 ../../$< > ../../$@ 2>&1; if [ $$? -ne 0 ]; then rm -f ../../$@; false; fi

clean: clean_obj clean_exe clean_stdout

clean_obj:
	@rm -f $(EXISTING_OBJ)

clean_exe:
	@rm -f $(EXISTING_EXE)

clean_stdout:
	@rm -f $(STDOUT)
