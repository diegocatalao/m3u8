#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "logger.h"
#include "m3u8.h"

int m3u8_list_init(m3u8_list_node_t** list) {
  int status = M3U8_LIST_STATUS_NO_ERROR;

  if (list == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS,
                 "Pointer to list pointer cannot be null");
  }

  *list = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));

  if (*list == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_MEM_ALL_ERROR,
                 "Failed to allocate memory for m3u8_list_node_t");
  }

  (*list)->next = (*list);
  (*list)->prev = (*list);

clean_up:
  return status;
}

int m3u8_list_add_head(m3u8_list_node_t* head, m3u8_list_node_t* node) {
  int status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL || node == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS,
                 "The arguments head and new cannot be null");
  }

  node->next = head->next;
  node->prev = head;

  head->next->prev = node;
  head->next = node;

clean_up:
  return status;
}

int m3u8_list_add_tail(m3u8_list_node_t* node, m3u8_list_node_t* head) {
  int status = M3U8_LIST_STATUS_NO_ERROR;

  if (node == NULL || head == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS,
                 "The arguments head and node cannot be null");
  }

  node->prev = head->prev;
  node->next = head;

  head->prev->next = node;
  head->prev = node;

clean_up:
  return status;
}

int m3u8_list_remove(m3u8_list_node_t* node) {
  int status = M3U8_LIST_STATUS_NO_ERROR;

  if (node == NULL || node->prev == NULL || node->next == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS,
                 "Node and its neighbors cannot be null");
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->next = NULL;
  node->prev = NULL;

clean_up:
  return status;
}

int m3u8_list_empty(const m3u8_list_node_t* head, bool* is_empty) {
  int status = M3U8_STATUS_NO_ERROR;

  if (head == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS, "Arg head cannot to be null");
  }

  if (is_empty == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS,
                 "Arg is_empty cannot to be null");
  }

  *is_empty = head->next == head;

clean_up:
  return status;
}

int m3u8_list_count(const m3u8_list_node_t* head, int* size) {
  int status = M3U8_STATUS_NO_ERROR;

  if (head == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS, "Arg head cannot to be null");
  }

  if (size == NULL) {
    RAISE_STATUS(M3U8_LIST_STATUS_INVALID_ARGS, "Arg size cannot to be null");
  }

  int count = 0;
  // set the head next inside node
  const m3u8_list_node_t* node = head->next;

  while (node != head) {
    count++;
    node = node->next;
  }

  *size = count;

clean_up:
  return status;
}
