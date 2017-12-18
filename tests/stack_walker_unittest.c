//
// Created by cdeler on 12/18/17.
//

#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cmocka.h>

#include "ybtl.h"

extern void
test01_smoke_it_works(void **state _unused)
	{
	int rc = ybtl_backtrace();
	assert_true(rc);

	size_t stackDepth = ybtl_stack_depth();

	assert_true(stackDepth > 0);
	size_t i, isFound = false;

	for (i = 0; i < stackDepth; ++i)
		{
		if (!strcmp(ybtl_get_function_name(i), __func__))
			{
			isFound = true;
			break;
			}
		}

	assert_true(isFound);
	assert_true(ybtl_get_ip(i) > 0);
	}

static int
_recursionStateSetup(void **state)
	{
	size_t *pState = (size_t *) state;

	*pState = 0U;

	return 0;
	}

extern void
test02_smoke_recursion(void **state)
	{
	const size_t counter = *(size_t *) state;
	static const size_t recursionDepth = 15U;

	if (counter < recursionDepth)
		{
		size_t inc = counter + 1;
		test02_smoke_recursion((void **) &inc);
		}
	else
		{
		assert_true(ybtl_backtrace());

		size_t recursionCounter = 0U;
		size_t stackDepth = ybtl_stack_depth();

		assert_true(stackDepth > recursionDepth
		                         + 1U // initial test02_smoke_recursion
		                         + 1U // ybtl_backtrace
		);

		size_t i;

		for (i = 0; i < stackDepth; ++i)
			{
			if (!strcmp(ybtl_get_function_name(i), __func__))
				{
				++recursionCounter;
				}
			}

		assert_int_equal(recursionCounter, recursionDepth + 1U /* initial test02_smoke_recursion */);
		}

	}

extern void
test03_stack_depth_exceed(void **state)
	{
	const size_t counter = *(size_t *) state;
	static const size_t recursionDepth = STACK_WALKER_MAX_DEPTH + 1U;

	if (counter < recursionDepth)
		{
		size_t inc = counter + 1;
		test03_stack_depth_exceed((void **) &inc);
		}
	else
		{
		assert_false(ybtl_backtrace());
		assert_int_equal(ybtl_stack_depth(), STACK_WALKER_MAX_DEPTH);
		assert_true(ybtl_is_overflow());
		}
	}

int
main(int argc, char **argv)
	{
	const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test01_smoke_it_works),
					cmocka_unit_test_setup(test02_smoke_recursion, _recursionStateSetup),
					cmocka_unit_test_setup(test03_stack_depth_exceed, _recursionStateSetup),
			};

	const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

	return _cmocka_run_group_tests("stack_walker", tests, testsCount, NULL, NULL);
	}