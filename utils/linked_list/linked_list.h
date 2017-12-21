//
// Created by cdeler on 12/21/17.
//

#pragma once
#ifndef YBTL_LINKED_LIST_H
#define YBTL_LINKED_LIST_H

#include <stddef.h>

typedef struct _linked_list_t *linked_list_handle;

typedef void (*item_dealloc_t)(void *);

linked_list_handle
linked_list_open(item_dealloc_t deallocator);

void
linked_list_close(linked_list_handle *handle) __attribute__((nonnull(1)));

size_t
linked_list_size(linked_list_handle handle) __attribute__((nonnull(1)));;

#endif //YBTL_LINKED_LIST_H
