#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "logger.h"

m3u8_list_status_t m3u8_list_init(m3u8_list_node_t* head) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument head is NULL");
  }

  if (memset(head, 0, sizeof(m3u8_list_node_t)) == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_MEM_ALL_ERROR,
               "Method memset return null for head");
  }

  head->next = head;
  head->prev = head;

clean_up:
  return status;
}

m3u8_list_status_t m3u8_list_ina(m3u8_list_node_t* head,
                                 m3u8_list_node_t* node) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: head is NULL");
  }

  if (node == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: node is NULL");
  }

  node->next = head->next;
  node->prev = head;

  if (head->next != NULL) {
    head->next->prev = node;
  }

  head->next = node;

clean_up:
  return status;
}

m3u8_list_status_t m3u8_list_inb(m3u8_list_node_t* head,
                                 m3u8_list_node_t* node) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: head is NULL");
  }

  if (node == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: node is NULL");
  }

  node->next = head;
  node->prev = head->prev;

  head->prev->next = node;
  head->prev = node;

clean_up:
  return status;
}

m3u8_list_status_t m3u8_list_remove(m3u8_list_node_t* node) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (node == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: node is NULL");
  }

  if (node->next == node && node->prev == node) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Unable to remove the head");
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;

  node->next = NULL;
  node->prev = NULL;

clean_up:
  return status;
}

m3u8_list_status_t m3u8_list_is_empty(const m3u8_list_node_t* head,
                                      bool*                   is_empty) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: head is NULL");
  }

  *is_empty = head->next == head && head->prev == head;

clean_up:
  return status;
}

m3u8_list_status_t m3u8_list_count(const m3u8_list_node_t* head, int* size) {
  m3u8_list_status_t status = M3U8_LIST_STATUS_NO_ERROR;

  if (head == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: head is NULL");
  }

  if (size == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS, "Invalid argument: size is NULL");
  }

  if (head->next == NULL || head->prev == NULL) {
    M3U8_RAISE(M3U8_LIST_STATUS_INVALID_ARGS,
               "Invalid argument: members are NULL");
  }

  int               count = 0;
  m3u8_list_node_t* pivot = head->next;

  while (pivot != head) {
    count++;
    pivot = pivot->next;
  }

  *size = count;

clean_up:
  return status;
}
