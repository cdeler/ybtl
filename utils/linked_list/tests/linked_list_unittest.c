//
// Created by cdeler on 12/21/17.
//

#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "ybtl_types.h"
#include "linked_list.h"

static void
test01_smoke_it_works(void **state _unused)
	{
	linked_list_handle handle = linked_list_open(NULL);
	assert_non_null(handle);

	assert_int_equal(linked_list_size(handle), 0U);

	linked_list_close(&handle);
	assert_null(handle);
	}

int
main(int argc, char **argv)
	{
	const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test01_smoke_it_works),
			};

	const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

	return _cmocka_run_group_tests("linked_list", tests, testsCount, NULL, NULL);
	}