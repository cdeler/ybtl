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
#include <ybtl_types.h>

#include "ybtl.h"

static void *
__attribute__((noinline))
_get_ip()
	{
	return __builtin_return_address(0) + 4U;
	}


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

static void
test03_get_line_info(void **state _unused)
	{
	function_data_t info;
	memset(&info, 0, sizeof(function_data_t));

	void *ip = _get_ip();

	ybtl_get_line_info(ip, &info);
	size_t lineno = __LINE__ - 1;
	const char *fileName = basename(__FILE__);
	const char *fname = __func__;

	assert_int_equal(info.sourceLine, lineno);
	assert_string_equal(info.functionName, fname);
	assert_string_equal(info.sourceFileName, fileName);

	}

int
main(int argc, char **argv)
    {
    const struct CMUnitTest tests[] =
            {
		            cmocka_unit_test(test01_smoke_it_works),
		            cmocka_unit_test(test02_get_function_data),
		            cmocka_unit_test(test03_get_line_info),
            };

    const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

    return _cmocka_run_group_tests("dwarf_info", tests, testsCount, NULL, NULL);
    }