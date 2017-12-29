//
// Created by cdeler on 12/18/17.
//

#pragma once
#ifndef YBTL_YBTL_DWARF_H
#define YBTL_YBTL_DWARF_H

#include "ybtl_types.h"


bool
ybtl_is_dwarf_enabled();

const function_data_t *
ybtl_get_function_data(const char *functionName);

void
ybtl_get_line_info(const void *ip, function_data_t *outData);


#endif //YBTL_YBTL_DWARF_H
