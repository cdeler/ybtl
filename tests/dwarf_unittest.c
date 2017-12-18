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

void
test01_smoke_it_works(void **state _unused)
    {
    ybtl_test_dwarf();
    }

int
main(int argc, char **argv)
    {
    const struct CMUnitTest tests[] =
            {
                    cmocka_unit_test(test01_smoke_it_works),
            };

    const size_t testsCount = sizeof(tests) / sizeof(struct CMUnitTest);

    return _cmocka_run_group_tests("dwarf_info", tests, testsCount, NULL, NULL);
    }