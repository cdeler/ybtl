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
#include <ybtl.h>
#include <libgen.h>

#include "ybtl.h"

static void
test01_smoke_it_works(void **state _unused)
    {
    assert_true(ybtl_is_dwarf_enabled());
    }

static void
test02_get_function_data(void **state _unused)
	{
	size_t lineNo = __LINE__ - 2;
	const char *fileName = basename(__FILE__);
	const function_data_t *data = ybtl_get_function_data(__func__);

	assert_non_null(data);
	assert_int_equal(data->sourceLine, lineNo);
	assert_string_equal(data->functionName, __func__);
	assert_string_equal(data->sourceFileName, fileName);
	}
int
main(int argc, char **argv)
    {
    const struct CMUnitTest tests[] =
            {
					cmocka_unit_test(test01_smoke_it_works),
					cmocka_unit_test(test02_get_function_data),
            };

    const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

    return _cmocka_run_group_tests("dwarf_info", tests, testsCount, NULL, NULL);
    }