//
// Created by iii on 12/21/17.
//

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "linked_list.h"

typedef struct _list_item _list_item_t;

struct _list_item
{
void *value;
_list_item_t *next;
_list_item_t *prev;
};

struct _linked_list_t
{
char eyecatcher[4];
size_t size;
_list_item_t *head;
_list_item_t *tail;
item_dealloc_t deallocator;
};

static void
_removeHead(linked_list_handle handle)
	{
	if (handle->size)
		{
		_list_item_t *oldHead = handle->head;
		assert(oldHead);

		if (handle->head != handle->tail)
			{
			handle->head = handle->head + 1;
			}
		else
			{
			assert(handle->size == 0U);
			handle->head = handle->tail = NULL;
			}

		--(handle->size);

		if (handle->deallocator)
			handle->deallocator(oldHead);

		}
	}

linked_list_handle
linked_list_open(item_dealloc_t deallocator)
	{
	linked_list_handle handle = calloc(1, sizeof(struct _linked_list_t));
	handle->deallocator = deallocator;

	strcpy(handle->eyecatcher, "CDL");

	return handle;
	}

void
linked_list_close(linked_list_handle *pHandle)
	{
	assert(pHandle);
	assert(*pHandle);
	
	linked_list_handle handle = *pHandle;
	while (handle->size)
		{
		_removeHead(handle);
		}

	free(handle);

	*pHandle = NULL;
	}

size_t
linked_list_size(linked_list_handle handle)
	{
	return handle->size;
	}