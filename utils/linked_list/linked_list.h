//
// Created by cdeler on 12/21/17.
//

#pragma once
#ifndef YBTL_LINKED_LIST_H
#define YBTL_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct _linked_list_t *linked_list_handle;
typedef struct _list_item *linked_list_item;

typedef void (*item_dealloc_t)(void *);

linked_list_handle
linked_list_open(item_dealloc_t deallocator);

void __attribute__((nonnull(1)))
linked_list_close(linked_list_handle *handle);

size_t  __attribute__((nonnull(1))) __attribute__((pure))
linked_list_size(linked_list_handle handle);

void __attribute__((nonnull(1)))
linked_list_append(linked_list_handle handle, void *element);

bool __attribute__((nonnull(1, 2)))
linked_list_iterate(linked_list_handle handle, linked_list_item *pIteratedItem);

void *
__attribute__((nonnull(1))) __attribute__((pure))
linked_list_get_value(linked_list_item item);

#endif //YBTL_LINKED_LIST_H
