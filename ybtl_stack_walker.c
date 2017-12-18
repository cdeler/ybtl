//
// Created by cdeler on 12/18/17.
//
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <unwind.h>
#include <dlfcn.h>

#include "ybtl_stack_walker.h"

typedef struct
{
void *ip;
char name_buffer[STACK_WALKER_IDENTEFER_NAME_MAX_LENGTH];
} stack_chunk_t;

typedef struct
{
size_t depth;
bool overflow;
stack_chunk_t stack[STACK_WALKER_MAX_DEPTH];
} stack_handle_t;

static stack_handle_t storage;

static void
_get_symbol_name(void *addr, stack_chunk_t *data)
	{
	Dl_info info;
	int res = dladdr(addr, &info);

	if (res)
		{
		const char *fname = info.dli_sname;

		if (fname)
			strncpy(data->name_buffer, fname, STACK_WALKER_IDENTEFER_NAME_MAX_LENGTH);
		else
			strcpy(data->name_buffer, "(NULL)");
		}
	else
		{
		strcpy(data->name_buffer, "(ERROR)");
		}
	}

static _Unwind_Reason_Code
_trace_frame(struct _Unwind_Context *context, void *data)
	{
	stack_handle_t *pStorage = (stack_handle_t *) data;
	_Unwind_Reason_Code rc = _URC_NO_REASON;

	if (pStorage->depth < STACK_WALKER_MAX_DEPTH)
		{
		stack_chunk_t *stackFrame = pStorage->stack + pStorage->depth;
		void *addr = (void *) _Unwind_GetIP(context);

		_get_symbol_name(addr, stackFrame);

		pStorage->stack[pStorage->depth++].ip = addr;
		}
	else
		{
		pStorage->overflow = true;
		rc = _URC_END_OF_STACK;
		}

	return rc;
	}

int
ybtl_backtrace()
	{
	ybtl_cleanup();

	_Unwind_Reason_Code code = _Unwind_Backtrace(_trace_frame, &storage);

	return code == _URC_END_OF_STACK;
	}

size_t
ybtl_stack_depth()
	{
	return storage.depth;
	}

const void *
ybtl_get_ip(size_t frameIndex)
	{
	const void *result = (frameIndex < storage.depth) ? storage.stack[frameIndex].ip : NULL;

	return result;
	}

const char *
ybtl_get_function_name(size_t frameIndex)
	{
	const char *result = (frameIndex < storage.depth) ? storage.stack[frameIndex].name_buffer : NULL;

	return result;
	}

void
ybtl_cleanup()
	{
	memset(&storage, 0, sizeof(stack_handle_t));
	}

bool
ybtl_is_overflow()
	{
	return storage.overflow;
	}