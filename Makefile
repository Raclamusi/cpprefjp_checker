CXX = /opt/gcc-head/bin/g++
CXXFLAGS := -std=c++26 -fmodules -freflection -fcontracts -pthread -pedantic -Wno-attributes -Wno-parentheses

# 将来実行できるようになるであろうが、今はコンパイルできないコードのリスト
define FUTURE_LIST
	$(wildcard codes/module/*.cpp)
	codes/lang/cpp23/lifetime_extension_in_range_based_for_loop.1.cpp23.cpp
	codes/reference/print/print.2.cpp23.cpp
	codes/reference/print/println.2.cpp23.cpp
	codes/reference/ranges/concat_view.1.cpp26.cpp

	$(shell find codes/reference/generator -name '*.cpp')
	codes/reference/ranges/elements_of.1.cpp23.cpp
	codes/reference/ranges/range_adaptor_closure.2.cpp23.cpp

	$(wildcard codes/lang/cpp26/contracts.*.cpp)
	$(shell find codes/reference/atomic -name '*.cpp26.cpp')
	$(shell find codes/reference/cmath -name '*.cpp26.cpp')
	$(shell find codes/reference/execution/execution -name '*.cpp')
	$(shell find codes/reference/functional -name '*.cpp26.cpp')
	$(shell find codes/reference/initializer_list/initializer_list -name '*.cpp26.cpp')
	$(shell find codes/reference/inplace_vector -name '*.cpp')
	$(shell find codes/reference/linalg -name '*.cpp')
	$(shell find codes/reference/mdspan -name '*.cpp')
	$(shell find codes/reference/random -name '*.cpp26.cpp')
	$(shell find codes/reference/rcu -name '*.cpp')
	$(shell find codes/reference/stacktrace -name '*.cpp')
	$(shell find codes/reference/stop_token -name '*.cpp26.cpp')

	$(shell find codes/reference/ranges -path '*/adjacent_transform_view*.cpp')
	$(shell find codes/reference/ranges -path '*/adjacent_view*.cpp')
	$(shell find codes/reference/ranges -path '*/as_const_view*.cpp')
	$(shell find codes/reference/ranges -path '*/cartesian_product_view*.cpp')
	$(shell find codes/reference/ranges -path '*/chunk_view*.cpp')
	$(shell find codes/reference/ranges -path '*/concat_view*.cpp')
	$(shell find codes/reference/ranges -path '*/elements_view*.cpp')
	$(shell find codes/reference/ranges -path '*/enumerate_view*.cpp')
	$(shell find codes/reference/ranges -path '*/slide_view*.cpp')
	$(shell find codes/reference/ranges -path '*/stride_view*.cpp')
	$(shell find codes/reference/ranges -path '*/to_input_view*.cpp')
	$(wildcard codes/reference/algorithm/ranges_fold_*.cpp)

	$(shell find codes/reference/iterator -path '*/basic_const_iterator*.cpp')
	codes/reference/iterator/const_iterator.1.cpp23.cpp
	codes/reference/iterator/const_sentinel.1.cpp23.cpp
	codes/reference/iterator/iter_const_reference_t.1.cpp23.cpp
	codes/reference/iterator/make_const_iterator.1.cpp23.cpp
	codes/reference/iterator/make_const_sentinel.1.cpp23.cpp
	codes/reference/iterator/projected_value_t.1.cpp26.cpp
	codes/reference/ranges/const_iterator_t.1.cpp23.cpp
	codes/reference/ranges/const_sentinel_t.1.cpp23.cpp
	codes/reference/ranges/constant_range.1.cpp23.cpp
	codes/reference/ranges/range_const_reference_t.1.cpp23.cpp

	$(shell find codes/reference/syncstream -name '*.cpp')
	codes/reference/ostream/emit_on_flush.1.cpp20.cpp
	codes/reference/ostream/flush_emit.1.cpp20.cpp
	codes/reference/ostream/noemit_on_flush.1.cpp20.cpp

	codes/reference/deque/deque/erase_free.2.cpp20.cpp
	codes/reference/forward_list/forward_list/erase_free.2.cpp20.cpp
	codes/reference/list/list/erase_free.2.cpp20.cpp
	codes/reference/string/basic_string/erase_free.2.cpp20.cpp
	codes/reference/vector/vector/erase_free.2.cpp20.cpp
	codes/reference/algorithm/binary_search.2.cpp03.cpp
	codes/reference/algorithm/count.2.cpp03.cpp
	codes/reference/algorithm/equal_range.2.cpp03.cpp
	codes/reference/algorithm/fill.2.cpp03.cpp
	codes/reference/algorithm/fill_n.2.cpp03.cpp
	codes/reference/algorithm/find.2.cpp03.cpp
	codes/reference/algorithm/lower_bound.2.cpp03.cpp
	codes/reference/algorithm/ranges_binary_search.2.cpp20.cpp
	codes/reference/algorithm/ranges_contains.2.cpp23.cpp
	codes/reference/algorithm/ranges_count.2.cpp20.cpp
	codes/reference/algorithm/ranges_equal_range.2.cpp20.cpp
	codes/reference/algorithm/ranges_fill.2.cpp20.cpp
	codes/reference/algorithm/ranges_find.3.cpp20.cpp
	codes/reference/algorithm/ranges_fill_n.2.cpp20.cpp
	codes/reference/algorithm/ranges_find_last.2.cpp23.cpp
	codes/reference/algorithm/ranges_fold_left.3.cpp23.cpp
	codes/reference/algorithm/ranges_fold_left_with_iter.4.cpp23.cpp
	codes/reference/algorithm/ranges_lower_bound.2.cpp20.cpp
	codes/reference/algorithm/ranges_remove.2.cpp20.cpp
	codes/reference/algorithm/ranges_remove_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_replace.2.cpp20.cpp
	codes/reference/algorithm/ranges_replace_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_replace_copy_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_search_n.2.cpp20.cpp
	codes/reference/algorithm/ranges_upper_bound.2.cpp20.cpp
	codes/reference/algorithm/remove.2.cpp03.cpp
	codes/reference/algorithm/remove_copy.2.cpp03.cpp
	codes/reference/algorithm/replace.2.cpp03.cpp
	codes/reference/algorithm/replace_copy_if.2.cpp03.cpp
	codes/reference/algorithm/replace_if.2.cpp03.cpp
	codes/reference/algorithm/search_n.2.cpp03.cpp
	codes/reference/algorithm/upper_bound.2.cpp03.cpp

	codes/reference/memory/owner_hash.1.cpp26.cpp
	codes/reference/memory/owner_equal.1.cpp26.cpp
	codes/reference/memory/shared_ptr/owner_hash.1.cpp26.cpp
	codes/reference/memory/shared_ptr/owner_equal.1.cpp26.cpp
	codes/reference/memory/weak_ptr/owner_hash.1.cpp26.cpp
	codes/reference/memory/weak_ptr/owner_equal.1.cpp26.cpp
	codes/reference/memory/atomic.1.cpp20.cpp
	codes/reference/memory/start_lifetime_as.1.cpp23.cpp
	codes/reference/memory/start_lifetime_as_array.1.cpp23.cpp

	codes/reference/execution/is_execution_policy.1.cpp17.cpp
	codes/reference/algorithm/ranges_adjacent_find.2.cpp20.cpp
	codes/reference/algorithm/ranges_all_of.2.cpp20.cpp
	codes/reference/algorithm/ranges_any_of.2.cpp20.cpp
	codes/reference/algorithm/ranges_contains.3.cpp23.cpp
	codes/reference/algorithm/ranges_contains_subrange.2.cpp23.cpp
	codes/reference/algorithm/ranges_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_copy_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_copy_n.2.cpp20.cpp
	codes/reference/algorithm/ranges_count.4.cpp20.cpp
	codes/reference/algorithm/ranges_count_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_ends_with.2.cpp23.cpp
	codes/reference/algorithm/ranges_equal.2.cpp20.cpp
	codes/reference/algorithm/ranges_fill.3.cpp20.cpp
	codes/reference/algorithm/ranges_fill_n.3.cpp20.cpp
	codes/reference/algorithm/ranges_find.4.cpp20.cpp
	codes/reference/algorithm/ranges_find_end.2.cpp20.cpp
	codes/reference/algorithm/ranges_find_first_of.2.cpp20.cpp
	codes/reference/algorithm/ranges_find_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_find_if_not.2.cpp20.cpp
	codes/reference/algorithm/ranges_find_last.3.cpp23.cpp
	codes/reference/algorithm/ranges_find_last_if.2.cpp23.cpp
	codes/reference/algorithm/ranges_find_last_if_not.2.cpp23.cpp
	codes/reference/algorithm/ranges_for_each.2.cpp20.cpp
	codes/reference/algorithm/ranges_for_each_n.2.cpp20.cpp
	codes/reference/algorithm/ranges_generate.2.cpp20.cpp
	codes/reference/algorithm/ranges_generate_n.2.cpp20.cpp
	codes/reference/algorithm/ranges_includes.2.cpp20.cpp
	codes/reference/algorithm/ranges_inplace_merge.2.cpp20.cpp
	codes/reference/algorithm/ranges_is_heap.2.cpp20.cpp
	codes/reference/algorithm/ranges_is_heap_until.2.cpp20.cpp
	codes/reference/algorithm/ranges_is_partitioned.2.cpp20.cpp
	codes/reference/algorithm/ranges_is_sorted.2.cpp20.cpp
	codes/reference/algorithm/ranges_is_sorted_until.2.cpp20.cpp
	codes/reference/algorithm/ranges_lexicographical_compare.2.cpp20.cpp
	codes/reference/algorithm/ranges_max.2.cpp20.cpp
	codes/reference/algorithm/ranges_max_element.2.cpp20.cpp
	codes/reference/algorithm/ranges_merge.2.cpp20.cpp
	codes/reference/algorithm/ranges_min.2.cpp20.cpp
	codes/reference/algorithm/ranges_min_element.2.cpp20.cpp
	codes/reference/algorithm/ranges_minmax.2.cpp20.cpp
	codes/reference/algorithm/ranges_minmax.3.cpp20.cpp
	codes/reference/algorithm/ranges_minmax_element.2.cpp20.cpp
	codes/reference/algorithm/ranges_mismatch.2.cpp20.cpp
	codes/reference/algorithm/ranges_move.2.cpp20.cpp
	codes/reference/algorithm/ranges_move_backward.2.cpp20.cpp
	codes/reference/algorithm/ranges_none_of.2.cpp20.cpp
	codes/reference/algorithm/ranges_nth_element.2.cpp20.cpp
	codes/reference/algorithm/ranges_partial_sort.2.cpp20.cpp
	codes/reference/algorithm/ranges_partial_sort_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_partition.2.cpp20.cpp
	codes/reference/algorithm/ranges_partition_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_remove.3.cpp20.cpp
	codes/reference/algorithm/ranges_remove_copy.3.cpp20.cpp
	codes/reference/algorithm/ranges_remove_copy_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_remove_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_replace.3.cpp20.cpp
	codes/reference/algorithm/ranges_replace_copy.3.cpp20.cpp
	codes/reference/algorithm/ranges_replace_copy_if.3.cpp20.cpp
	codes/reference/algorithm/ranges_replace_if.2.cpp20.cpp
	codes/reference/algorithm/ranges_reverse.2.cpp20.cpp
	codes/reference/algorithm/ranges_reverse_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_rotate.3.cpp20.cpp
	codes/reference/algorithm/ranges_rotate_copy.2.cpp20.cpp
	codes/reference/algorithm/ranges_search.2.cpp20.cpp
	codes/reference/algorithm/ranges_set_difference.2.cpp20.cpp
	codes/reference/algorithm/ranges_set_intersection.2.cpp20.cpp
	codes/reference/algorithm/ranges_set_symmetric_difference.2.cpp20.cpp
	codes/reference/algorithm/ranges_set_union.2.cpp20.cpp
	codes/reference/algorithm/ranges_shift_left.2.cpp23.cpp
	codes/reference/algorithm/ranges_shift_right.2.cpp23.cpp
	codes/reference/algorithm/ranges_sort.3.cpp20.cpp
	codes/reference/algorithm/ranges_stable_partition.2.cpp20.cpp
	codes/reference/algorithm/ranges_stable_sort.2.cpp20.cpp
	codes/reference/algorithm/ranges_starts_with.2.cpp23.cpp
	codes/reference/algorithm/ranges_swap_ranges.2.cpp20.cpp
	codes/reference/algorithm/ranges_transform.3.cpp20.cpp
	codes/reference/algorithm/ranges_unique.2.cpp20.cpp
	codes/reference/algorithm/ranges_unique_copy.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_copy.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_copy_n.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_move.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_move_n.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_fill.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_fill_n.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_default_construct.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_default_construct_n.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_value_construct.2.cpp20.cpp
	codes/reference/memory/ranges_uninitialized_value_construct_n.2.cpp20.cpp
	codes/reference/memory/ranges_destroy.2.cpp20.cpp
	codes/reference/memory/ranges_destroy_n.2.cpp20.cpp

	$(shell find codes/reference/chrono -name '*.cpp20.cpp')
	codes/reference/format/basic_format_string.1.cpp23.cpp

	codes/reference/valarray/valarray/begin.1.cpp26.cpp
	codes/reference/valarray/valarray/end.1.cpp26.cpp
	codes/reference/optional/optional/begin.1.cpp26.cpp
	codes/reference/optional/optional/end.1.cpp26.cpp
	codes/reference/span/span/cbegin.1.cpp23.cpp
	codes/reference/span/span/cend.1.cpp23.cpp
	codes/reference/span/span/crbegin.1.cpp23.cpp
	codes/reference/span/span/crend.1.cpp23.cpp

	codes/lang/cpp20/relaxing_the_structured_bindings_customization_point_finding_rules.1.cpp20.cpp
	codes/reference/cassert/assert.3.cpp03.cpp
	codes/reference/cmath/assoc_laguerre.1.cpp17.cpp
	codes/reference/cmath/assoc_legendre.1.cpp17.cpp
	codes/reference/cmath/beta.1.cpp17.cpp
	codes/reference/cmath/comp_ellint_1.1.cpp17.cpp
	codes/reference/cmath/comp_ellint_1.2.cpp17.cpp
	codes/reference/cmath/comp_ellint_2.1.cpp17.cpp
	codes/reference/cmath/comp_ellint_3.1.cpp17.cpp
	codes/reference/cmath/cyl_bessel_i.1.cpp17.cpp
	codes/reference/cmath/cyl_bessel_j.1.cpp17.cpp
	codes/reference/cmath/cyl_bessel_k.1.cpp17.cpp
	codes/reference/cmath/cyl_neumann.1.cpp17.cpp
	codes/reference/cmath/expint.1.cpp17.cpp
	codes/reference/cmath/ellint_1.1.cpp17.cpp
	codes/reference/cmath/ellint_2.1.cpp17.cpp
	codes/reference/cmath/ellint_3.1.cpp17.cpp
	codes/reference/cmath/expint.1.cpp17.cpp
	codes/reference/cmath/laguerre.1.cpp17.cpp
	codes/reference/cmath/legendre.1.cpp17.cpp
	codes/reference/cmath/riemann_zeta.1.cpp17.cpp
	codes/reference/cmath/sph_bessel.1.cpp17.cpp
	codes/reference/cmath/sph_legendre.1.cpp17.cpp
	codes/reference/cmath/sph_neumann.1.cpp17.cpp
	codes/reference/compare/partial_order.1.cpp20.cpp
	codes/reference/compare/strong_order.1.cpp20.cpp
	codes/reference/compare/weak_order.1.cpp20.cpp
	codes/reference/exception/exception_ptr_cast.1.cpp26.cpp
	codes/reference/filesystem/file_time_type.2.cpp17.cpp
	codes/reference/filesystem/path/formatter.1.cpp26.cpp
	codes/reference/format/make_format_args.1.cpp20.cpp
	codes/reference/format/vformat.1.cpp20.cpp
	codes/reference/future/packaged_task/op_constructor.1.cpp11.cpp
	codes/reference/mutex/recursive_timed_mutex/native_handle.1.cpp11.cpp
	codes/reference/mutex/timed_mutex/native_handle.1.cpp11.cpp
	codes/reference/type_traits/is_corresponding_member.1.cpp20.cpp
	codes/reference/type_traits/is_layout_compatible.1.cpp20.cpp
	codes/reference/type_traits/is_pointer_interconvertible_base_of.1.cpp20.cpp
	codes/reference/type_traits/is_pointer_interconvertible_with_class.1.cpp20.cpp
	codes/reference/type_traits/is_trivially_copyable.2.cpp11.cpp
	codes/reference/type_traits/apply_result.1.cpp26.cpp
	codes/reference/type_traits/is_applicable.1.cpp26.cpp
	codes/reference/type_traits/is_nothrow_applicable.1.cpp26.cpp
	codes/reference/utility/constant_arg_t.1.cpp26.cpp

	codes/reference/type_traits/is_within_lifetime.1.cpp26.cpp
	codes/reference/type_traits/reference_constructs_from_temporary.2.cpp23.cpp
	codes/reference/scoped_allocator/scoped_allocator_adaptor/op_deduction_guide.1.cpp17.cpp
	codes/reference/ostream/print.2.cpp23.cpp
	codes/reference/ostream/println.2.cpp23.cpp
	codes/reference/print/print.3.cpp23.cpp
	codes/reference/print/println.3.cpp23.cpp
	codes/reference/format/runtime_format.1.cpp26.cpp
	codes/reference/format/format.7.cpp20.cpp
	codes/reference/ranges/filter_view/iterator/op_constructor.1.cpp20.cpp
	codes/reference/ranges/filter_view/iterator/op_increment.1.cpp20.cpp
	codes/reference/ranges/filter_view/iterator/op_decrement.1.cpp20.cpp
	codes/reference/ranges/filter_view/iterator/op_deref.1.cpp20.cpp
	codes/reference/system_error/error_code/op_assign.1.cpp11.cpp
	codes/reference/system_error/error_code/op_constructor.1.cpp11.cpp
	$(wildcard codes/reference/numeric/*_sat.1.cpp26.cpp)
	codes/reference/numeric/saturate_cast.1.cpp26.cpp
	codes/reference/span/span/op_constructor.2.cpp20.cpp
endef
FUTURE_LIST := $(strip $(FUTURE_LIST))

# コンパイルできないコードのリスト
define IGNORE_LIST
	codes/article/platform/locales.1.cpp03.cpp
	codes/article/lib/at_thread_exit.1.cpp03.cpp
	codes/article/lib/at_thread_exit.2.cpp03.cpp
	codes/module/std.2.cpp23.cpp
	codes/module/std.4.cpp23.cpp
	codes/lang/cpp11/range_based_for.5.cpp11.cpp
	codes/lang/cpp11/override_final.1.cpp11.cpp
	codes/lang/cpp11/decltype.6.cpp11.cpp
	codes/lang/cpp11/alignas.1.cpp11.cpp
	codes/lang/cpp17/if_constexpr.1.cpp17.cpp
	codes/lang/cpp17/if_constexpr.2.cpp17.cpp
	codes/lang/cpp17/if_constexpr.3.cpp17.cpp
	codes/lang/cpp17/if_constexpr.4.cpp17.cpp
	codes/lang/cpp17/if_constexpr.5.cpp17.cpp
	codes/lang/cpp17/extending_static_assert.1.cpp17.cpp
	codes/lang/cpp17/remove_deprecated_increment_of_bool.1.cpp17.cpp
	codes/lang/cpp17/remove_deprecated_increment_of_bool.3.cpp17.cpp
	codes/lang/cpp17/inline_variables.1.cpp17.cpp
	codes/lang/cpp17/using_attribute_namespaces.1.cpp17.cpp
	codes/lang/cpp17/construction_enum_class_values.1.cpp17.cpp
	codes/lang/cpp20/using_enum.1.cpp20.cpp
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.1.cpp20.cpp
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.2.cpp20.cpp
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.3.cpp20.cpp
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.4.cpp20.cpp
	codes/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.5.cpp20.cpp
	codes/lang/cpp20/immediate_functions.1.cpp20.cpp
	codes/lang/cpp20/char8_t.1.cpp20.cpp
	codes/lang/cpp26/erroneous_behavior_for_uninitialized_reads.4.cpp26.cpp
	codes/lang/cpp26/user-generated_static_assert_messages.1.cpp26.cpp
	codes/lang/cpp26/embed.1.cpp26.cpp
	codes/lang/cpp26/embed.2.cpp26.cpp
	codes/lang/cpp26/embed.3.cpp26.cpp
	codes/lang/cpp26/embed.4.cpp26.cpp
	codes/lang/future/contract-based_programming.1.future.cpp
	codes/reference/memory/assume_aligned.2.cpp20.cpp
	codes/reference/cstdlib/malloc.1.cpp03.cpp
	codes/reference/cstdlib/calloc.1.cpp03.cpp
	codes/reference/cstdlib/free.1.cpp03.cpp
	codes/reference/memory/inout_ptr.1.cpp23.cpp
	codes/reference/memory/out_ptr.1.cpp23.cpp
	codes/reference/functional/function/op_constructor.1.cpp11.cpp
	codes/reference/memory/declare_no_pointers.1.cpp11.cpp
	codes/reference/type_traits/is_invocable_r.1.cpp17.cpp
	codes/reference/filesystem/path.2.cpp17.cpp
	codes/reference/filesystem/path/op_istream.2.cpp17.cpp
	codes/reference/fstream/basic_filebuf/native_handle.1.cpp26.cpp
	codes/reference/fstream/basic_fstream/native_handle.1.cpp26.cpp
	codes/reference/fstream/basic_ofstream/native_handle.1.cpp26.cpp
	codes/reference/fstream/basic_ifstream/native_handle.1.cpp26.cpp
	codes/reference/cfenv/fegetexceptflag.1.cpp11.cpp
	codes/reference/cfenv/fexcept_t.1.cpp11.cpp
	codes/reference/string_view/basic_string_view/op_constructor.1.cpp17.cpp
	codes/reference/thread/jthread/native_handle.1.cpp20.cpp
	codes/reference/thread/thread/native_handle.1.cpp11.cpp
	codes/reference/functional/function/assign.1.cpp11.cpp
	codes/lang/cpp26/add_atsign_dollar_graveaccent_to_the_basic_character_set.1.cpp26.cpp
	codes/lang/cpp11/decltype.4.cpp11.cpp
	codes/lang/cpp11/char16_32.3.cpp11.cpp
	codes/lang/cpp11/raw_string_literals.6.cpp11.cpp
	codes/lang/cpp11/utf8_string_literals.1.cpp11.cpp
	codes/reference/filesystem/u8path.1.cpp17.cpp
	codes/reference/type_traits/reference_constructs_from_temporary.1.cpp23.cpp
	codes/reference/type_traits/reference_converts_from_temporary.1.cpp23.cpp
	$(shell find codes/reference/locale -path '*/wstring_convert*.cpp')
	$(shell find codes/reference/codecvt -name '*.cpp')
	codes/reference/memory/shared_ptr/unique.1.cpp11.cpp
	codes/reference/memory/allocator.1.cpp03.cpp
	codes/reference/memory/allocator/construct.1.cpp03.cpp
	codes/reference/memory/allocator/destroy.1.cpp03.cpp
	codes/reference/memory/allocator/address.1.cpp03.cpp
	codes/reference/memory/allocator/max_size.1.cpp03.cpp
	codes/reference/memory/uninitialized_copy.1.cpp03.cpp
	codes/reference/memory/uninitialized_copy_n.1.cpp11.cpp
	codes/reference/memory/uninitialized_fill.1.cpp03.cpp
	codes/reference/memory/uninitialized_fill_n.1.cpp03.cpp
	codes/reference/memory/uninitialized_move.1.cpp17.cpp
	codes/reference/memory/declare_reachable.1.cpp11.cpp
	codes/reference/memory/undeclare_reachable.1.cpp11.cpp
	codes/reference/memory/undeclare_no_pointers.1.cpp11.cpp
	codes/reference/memory/return_temporary_buffer.1.cpp03.cpp
	codes/reference/memory/raw_storage_iterator.1.cpp03.cpp
	codes/reference/memory/get_temporary_buffer.1.cpp03.cpp
	codes/reference/type_traits/is_literal_type.1.cpp11.cpp
	codes/reference/type_traits/result_of.1.cpp11.cpp
	codes/reference/algorithm/random_shuffle.1.cpp03.cpp
	codes/reference/functional/negators.1.cpp03.cpp
	codes/reference/exception/bad_exception.1.cpp03.cpp
	codes/reference/exception/get_unexpected.1.cpp11.cpp
	codes/reference/exception/set_unexpected.1.cpp03.cpp
	codes/reference/exception/unexpected.1.cpp03.cpp
	codes/reference/exception/uncaught_exception.1.cpp03.cpp
	codes/reference/text_encoding.3.cpp26.cpp
endef
IGNORE_LIST := $(strip $(IGNORE_LIST)) ${FUTURE_LIST}

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
	@rm -rf run/*
