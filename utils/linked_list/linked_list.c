//
// Created by cdeler on 12/21/17.
//

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "linked_list.h"

typedef struct _list_item _list_item_t;

struct _list_item
{
void *value;
linked_list_item next;
linked_list_item prev;
};

struct _linked_list_t
{
char eyecatcher[4];
size_t size;
linked_list_item head;
linked_list_item tail;
item_dealloc_t deallocator;
};

static void
_removeHead(linked_list_handle handle)
	{
	if (handle->size)
		{
		linked_list_item oldHead = handle->head;
		assert(oldHead);

		if (handle->head != handle->tail)
			{
			handle->head = handle->head->next;
			}
		else
			{
			assert(handle->size == 1U);
			handle->head = handle->tail = NULL;
			}

		--(handle->size);

		if (handle->deallocator && oldHead->value)
			handle->deallocator(oldHead->value);

		free(oldHead);

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

void
linked_list_append(linked_list_handle handle, void *element)
	{
	linked_list_item newItem = calloc(1, sizeof(struct _list_item));

	newItem->value = element;

	if (handle->size == 0U)
		{
		handle->head = handle->tail = newItem;
		}
	else
		{
		newItem->prev = handle->tail;
		handle->tail->next = newItem;
		handle->tail = newItem;
		}

	++(handle->size);
	}

bool
linked_list_iterate(linked_list_handle handle, linked_list_item *pIteratedItem)
	{
	assert(handle);
	assert(pIteratedItem);

	linked_list_item iteratedItem = *pIteratedItem;
	bool result;

	if (iteratedItem == NULL)
		{
		*pIteratedItem = handle->head;
		result = true;
		}
	else
		{
		if (iteratedItem == handle->tail)
			{
			result = false;
			*pIteratedItem = NULL;
			}
		else
			{
			*pIteratedItem = iteratedItem->next;
			result = true;
			}
		}


	return result;
	}

void *
linked_list_get_value(linked_list_item item)
	{
	return item->value;
	}
