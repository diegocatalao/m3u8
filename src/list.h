/**
 * @file list.h
 * @brief Circular doubly-linked list implementation for general use.
 */

#ifndef __H_M3U8_LIST__
#define __H_M3U8_LIST__

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Operation completed successfully.
 *
 * @details Returned when a function completes without error.
 */
#define M3U8_LIST_STATUS_NO_ERROR      0x10000000

/**
 * @brief Invalid argument received.
 *
 * @details Returned when a parameter is NULL or otherwise invalid.
 */
#define M3U8_LIST_STATUS_INVALID_ARGS  (M3U8_LIST_STATUS_NO_ERROR + 0x01)

/**
 * @brief Memory allocation failure.
 *
 * @details Returned when memory allocation or reallocation fails.
 */
#define M3U8_LIST_STATUS_MEM_ALL_ERROR (M3U8_LIST_STATUS_NO_ERROR + 0x02)

/**
 * @brief Node not found in the list.
 *
 * @details Returned when a requested node cannot be located.
 */
#define M3U8_LIST_STATUS_NOT_FOUND     (M3U8_LIST_STATUS_NO_ERROR + 0x03)

/**
 * @brief Unknown error occurred.
 *
 * @details Returned when an unexpected or undefined error occurs.
 */
#define M3U8_LIST_STATUS_UNKNOWN_ERROR (M3U8_LIST_STATUS_NO_ERROR + 0x99)

/**
 * @brief Retrieves the structure containing a given member pointer.
 *
 * @param ptr    Pointer to the member.
 * @param type   Structure type.
 * @param member Member name.
 *
 * @return Pointer to the containing structure.
 */
#define m3u8_list_container_of(ptr, type, member) \
  ((type*)((char*)(ptr) - offsetof(type, member)))

/**
 * @brief Gets the next entry in the list.
 *
 * @param pos    Pointer to the current structure.
 * @param member Name of the list field in the structure.
 * 
 * @return Pointer to the next structure.
 */
#define m3u8_list_next(pos, type, member) \
  m3u8_list_container_of((pos)->member.next, type, member)

/**
 * @brief Gets the previous entry in the list.
 *
 * @param pos    Pointer to the current structure.
 * @param type   Structure type.
 * @param member Name of the list field in the structure.
 *
 * @return Pointer to the previous structure.
 */
#define m3u8_list_prev(pos, type, member) \
  m3u8_list_container_of((pos)->member.prev, type, member)

/**
 * @brief Iterates over elements of a circular doubly-linked list.
 *
 * @param entry  Loop variable of type (type*).
 * @param head   Pointer to the list head node.
 * @param type   Structure type.
 * @param member List node field name.
 */
#define m3u8_list_foreach(entry, head, type, member)           \
  for (m3u8_list_node_t* _pos = (head)->next;                  \
       _pos != (head) &&                                       \
       ((entry) = m3u8_list_container_of(_pos, type, member)); \
       _pos = _pos->next)

/**
 * @struct m3u8_list_node_t
 * @brief Node structure for a circular doubly-linked list.
 */
typedef struct m3u8_list_node {
  struct m3u8_list_node* next; /**< Pointer to the next node. */
  struct m3u8_list_node* prev; /**< Pointer to the previous node. */
} m3u8_list_node_t;

/**
 * @brief Initializes a circular list head node.
 *
 * @param[out] head Pointer to a list node to initialize.
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If head is NULL.
 */
int m3u8_list_init(m3u8_list_node_t* head);

/**
 * @brief Inserts a node after the head node.
 *
 * @param[in] head Pointer to the head node.
 * @param[in] node Pointer to the node to insert.
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_ina(m3u8_list_node_t* head, m3u8_list_node_t* node);

/**
 * @brief Inserts a node before the head node.
 *
 * @param[in] head Pointer to the head node.
 * @param[in] node Pointer to the node to insert.
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_inb(m3u8_list_node_t* head, m3u8_list_node_t* node);

/**
 * @brief Removes a node from the list.
 *
 * @param[in] node Pointer to the node to remove.
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If node or its links are NULL.
 */
int m3u8_list_remove(m3u8_list_node_t* node);

/**
 * @brief Checks whether the list is empty.
 *
 * @param[in]  head     Pointer to the list head.
 * @param[out] is_empty Output flag (true if empty).
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_is_empty(const m3u8_list_node_t* head, bool* is_empty);

/**
 * @brief Counts the nodes in the list (excluding head).
 *
 * @param[in]  head Pointer to the list head.
 * @param[out] size Output pointer for node count.
 *
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_count(const m3u8_list_node_t* head, int* size);

#endif  // __H_M3U8_LIST__
