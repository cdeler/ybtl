//
// Created by cdeler on 12/21/17.
//

#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "ybtl_types.h"
#include "linked_list.h"

static void
_element_deallocate(void *element)
	{
	assert_non_null(element);
	free(element);
	}

static void
test01_smoke_it_works(void **state _unused)
	{
	linked_list_handle handle = linked_list_open(_element_deallocate);
	assert_non_null(handle);

	assert_int_equal(linked_list_size(handle), 0U);

	linked_list_close(&handle);
	assert_null(handle);
	}

static void
test02_append_and_iterate_one_item(void **state _unused)
	{
	size_t i, N = 1U;
	static char buffer[256] = {0};
	linked_list_handle handle = linked_list_open(_element_deallocate);
	linked_list_item pIteratedItem = NULL;

	for (i = 0; i < N; ++i)
		{
		snprintf(buffer, sizeof(buffer), "%lu", i);
		linked_list_append(handle, strdup(buffer));
		}

	assert_int_equal(linked_list_size(handle), N);

	while (linked_list_iterate(handle, &pIteratedItem))
		{
		--N;
		}

	assert_int_equal(N, 0U);

	linked_list_close(&handle);
	}

static void
test03_append_and_iterate(void **state _unused)
	{
	size_t i, N = 10U;
	static char buffer[256] = {0};
	linked_list_handle handle = linked_list_open(_element_deallocate);
	linked_list_item pIteratedItem = NULL;

	for (i = 0; i < N; ++i)
		{
		snprintf(buffer, sizeof(buffer), "%lu", i);
		linked_list_append(handle, strdup(buffer));
		assert_int_equal(linked_list_size(handle), i + 1U);
		}

	assert_int_equal(linked_list_size(handle), N);

	pIteratedItem = NULL;
	i = 0;
	while (linked_list_iterate(handle, &pIteratedItem))
		{
		size_t value = strtoull(linked_list_get_value(pIteratedItem), NULL, 10);
		assert_int_equal(value, i);
		--N;
		++i;
		}

	assert_int_equal(N, 0U);
	linked_list_close(&handle);
	}

int
main(int argc, char **argv)
	{
	const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test01_smoke_it_works),
					cmocka_unit_test(test02_append_and_iterate_one_item),
					cmocka_unit_test(test03_append_and_iterate),
			};

	const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

	return _cmocka_run_group_tests("linked_list", tests, testsCount, NULL, NULL);
	}