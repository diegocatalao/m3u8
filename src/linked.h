/**
 * @file linked.h
 * @brief Circular doubly-linked list implementation for general use.
 */
#ifndef _M3U8_LINKED_H_
#define _M3U8_LINKED_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * @enum m3u8_linked_status_t
 * @brief Represents the status of a list operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_LINKED_STATUS_NO_ERROR = 0,
  /** Invalid argument received. */
  M3U8_LINKED_STATUS_INVALID_ARGS = 1,
  /** Memory allocation failure. */
  M3U8_LINKED_STATUS_MEM_ALL_ERROR = 2,
  /** Node not found in the list. */
  M3U8_LINKED_STATUS_NOT_FOUND = 3,
  /** An unknown or unspecified error occurred. */
  M3U8_LINKED_STATUS_UNKNOWN_ERROR = 99
} m3u8_linked_status_t;

/**
 * @struct m3u8_linked_node_t
 * @brief Node structure for a circular doubly-linked list.
 */
typedef struct m3u8_linked_node {
  /** Pointer to the next node. */
  struct m3u8_linked_node* next;
  /** Pointer to the previous node. */
  struct m3u8_linked_node* prev;
} m3u8_linked_node_t;

/**
 * @brief Retrieves the structure containing a given member pointer.
 * @param ptr    Pointer to the member.
 * @param type   Structure type.
 * @param member Member name.
 * @return Pointer to the containing structure.
 */
#define m3u8_linked_container_of(ptr, type, member) \
  ((type*)((char*)(ptr) - offsetof(type, member)))

/**
 * @brief Gets the next entry in the list.
 * @param pos    Pointer to the current structure.
 * @param member Name of the list field in the structure.
 * @return Pointer to the next structure.
 */
#define m3u8_linked_next(pos, type, member) \
  m3u8_linked_container_of((pos)->member.next, type, member)

/**
 * @brief Gets the previous entry in the list.
 * @param pos    Pointer to the current structure.
 * @param type   Structure type.
 * @param member Name of the list field in the structure.
 * @return Pointer to the previous structure.
 */
#define m3u8_linked_prev(pos, type, member) \
  m3u8_linked_container_of((pos)->member.prev, type, member)

/**
 * @brief Iterates over elements of a circular doubly-linked list.
 * @param entry  Loop variable of type (type*).
 * @param head   Pointer to the list head node.
 * @param type   Structure type.
 * @param member List node field name.
 */
#define m3u8_linked_foreach(entry, head, type, member)            \
  for (m3u8_linked_node_t* _pos = (head)->next, *_n = _pos->next; \
       _pos != (head) &&                                          \
       ((entry) = m3u8_linked_container_of(_pos, type, member));  \
       _pos = _n, _n = _pos->next)

/**
 * @brief Initializes a circular list head node.
 * @param head Pointer to a list node to initialize.
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p head is NULL.
 */
m3u8_linked_status_t m3u8_linked_init(m3u8_linked_node_t* head);

/**
 * @brief Inserts a node after the head node.
 * @param head Pointer to the head node.
 * @param node Pointer to the node to insert.
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p head or @p node is NULL.
 */
m3u8_linked_status_t m3u8_linked_ina(m3u8_linked_node_t* head,
                                     m3u8_linked_node_t* node);

/**
 * @brief Inserts a node before the head node.
 * @param head Pointer to the head node.
 * @param node Pointer to the node to insert.
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p head or @p node is NULL.
 */
m3u8_linked_status_t m3u8_linked_inb(m3u8_linked_node_t* head,
                                     m3u8_linked_node_t* node);

/**
 * @brief Removes a node from the list.
 * @param node Pointer to the node to remove.
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p node is NULL.
 */
m3u8_linked_status_t m3u8_linked_remove(m3u8_linked_node_t* node);

/**
 * @brief Checks whether the list is empty.
 * @param head     Pointer to the list head.
 * @param is_empty Output flag (true if empty).
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p head or @p is_empty
 *                                              is NULL.
 */
m3u8_linked_status_t m3u8_linked_is_empty(const m3u8_linked_node_t* head,
                                          bool*                     is_empty);

/**
 * @brief Counts the nodes in the list (excluding head).
 * @param head Pointer to the list head.
 * @param size Output pointer for node count.
 * @return @ref M3U8_LINKED_STATUS_NO_ERROR on success.
 * @return @ref M3U8_LINKED_STATUS_INVALID_ARGS if @p head or @p size is NULL.
 */
m3u8_linked_status_t m3u8_linked_count(const m3u8_linked_node_t* head,
                                       int*                      size);

#endif  // _M3U8_LINKED_H_
