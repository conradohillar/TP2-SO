// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <list.h>

typedef struct node node;

struct node {
  void *data;
  node *next;
};

struct listT {
  node *first;
  node *next;
  node *last;
  size_t size;
};

listADT create_list() {
  listADT list = (listADT)mm_malloc(sizeof(struct listT));
  if (list == NULL)
    return NULL;

  list->first = NULL;
  list->next = NULL;
  list->last = NULL;
  list->size = 0;
  return list;
}

void free_list(listADT list) {
  if (list == NULL)
    return;

  node *current = list->first;
  node *next;
  while (list->size > 0) {
    next = current->next;
    mm_free(current);
    current = next;
    list->size--;
  }
  mm_free(list);
}

uint8_t add_to_list(listADT list, void *data) {
  if (list == NULL || data == NULL)
    return 0;

  node *new_node = (node *)mm_malloc(sizeof(node));
  if (new_node == NULL)
    return 0;

  new_node->data = data;
  if (is_empty(list)) {
    list->first = new_node;
    list->next = new_node;
    list->last = new_node;
    new_node->next = new_node;
  } else {
    list->last->next = new_node;
    list->last = new_node;
    new_node->next = list->first;
  }
  list->size++;
  return 1;
}

uint8_t remove_from_list(listADT list, void *data) {
  if (list == NULL || is_empty(list) || data == NULL)
    return 0;

  node *current = list->first;
  node *previous = list->last;
  while (current->data != data) {
    if (current == list->last)
      return 0;
    previous = current;
    current = current->next;
  }
  previous->next = current->next;
  if (current == list->first)
    list->first = current->next;
  if (current == list->next)
    list->next = current->next;
  if (current == list->last)
    list->last = previous;
  mm_free(current);
  list->size--;
  if (list->size == 0) {
    list->first = NULL;
    list->next = NULL;
    list->last = NULL;
  }
  return 1;
}

void *remove_first(listADT list) {
  if (list == NULL || is_empty(list))
    return NULL;

  node *current = list->first;
  void *data = current->data;
  list->first = current->next;
  list->last->next = list->first;
  mm_free(current);
  list->size--;
  if (list->size == 0) {
    list->first = NULL;
    list->next = NULL;
    list->last = NULL;
  }
  return data;
}

void *list_next(const listADT list) {
  if (list == NULL || is_empty(list))
    return NULL;
  void *data = list->next->data;
  list->next = list->next->next;
  return data;
}

size_t list_size(const listADT list) {
  if (list == NULL)
    return 0;
  return list->size;
}

uint8_t is_empty(const listADT list) {
  if (list == NULL)
    return 1;
  return list->size == 0;
}

void *get_element(const listADT list, void *data) {
  if (list == NULL || is_empty(list) || data == NULL)
    return NULL;

  node *current = list->first;
  while (current->data != data) {
    if (current == list->last)
      return NULL;
    current = current->next;
  }
  return current->data;
}