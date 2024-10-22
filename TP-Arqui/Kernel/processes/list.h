#ifndef LIST_H
#define LIST_H

#include "../memory/memoryManager.h"
#include <stddef.h>
#include <stdint.h>

typedef struct listT *listADT;

listADT create_list();

void free_list(listADT list);

uint8_t add_to_list(listADT list, void *data);

uint8_t remove_from_list(listADT list, void *data);

void *remove_first(listADT list);

void *list_next(const listADT list);

size_t list_size(const listADT list);

uint8_t is_empty(const listADT list);

void *get_element(const listADT list, void *data);

#endif