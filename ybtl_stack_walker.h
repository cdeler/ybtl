//
// Created by cdeler on 12/18/17.
//

#pragma once
#ifndef YBTL_STACK_WALKER_H
#define YBTL_STACK_WALKER_H

#include "ybtl_types.h"

int
ybtl_backtrace();

size_t
ybtl_stack_depth();

const void *
ybtl_get_ip(size_t frameIndex);

const char *
ybtl_get_function_name(size_t frameIndex);

void
ybtl_cleanup();

bool
ybtl_is_overflow();

#endif //YBTL_STACK_WALKER_H
