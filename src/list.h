/**
 * @file list.h
 * @brief Circular doubly-linked list implementation for general use.
 */

#ifndef __h_m3u8_list__
#define __h_m3u8_list__

#include <stdbool.h>

/**
 * @brief Operation completed successfully.
 *
 * @details Returned when a function executes without error.
 */
#define M3U8_LIST_STATUS_NO_ERROR      0x01

/**
 * @brief Received an invalid argument from function parameters.
 *
 * @details Returned when a required argument is NULL or invalid.
 */
#define M3U8_LIST_STATUS_INVALID_ARGS  (M3U8_LIST_STATUS_NO_ERROR + 0x01)

/**
 * @brief Memory allocation error.
 *
 * @details Returned when a memory allocation or reallocation fails.
 */
#define M3U8_LIST_STATUS_MEM_ALL_ERROR (M3U8_LIST_STATUS_NO_ERROR + 0x02)

/**
 * @brief Node not found in the list.
 *
 * @details Returned when a requested node is not found.
 */
#define M3U8_LIST_STATUS_NOT_FOUND     (M3U8_LIST_STATUS_NO_ERROR + 0x03)

/**
 * @brief Unknown error occurred.
 *
 * @details Returned for unexpected errors or undefined behaviors.
 */
#define M3U8_LIST_STATUS_UNKNOWN_ERROR (M3U8_LIST_STATUS_NO_ERROR + 0x99)

/**
 * @brief Iterates over all nodes in the list (excluding the head node).
 *
 * @param pos  A pointer of type m3u8_list_node_t* used as the loop iterator.
 * @param head A pointer to the list head node.
 *
 * @details This macro traverses the circular doubly-linked list from the node
 *          immediately after the head to the node just before the head,
 *          skipping the head itself.
 *
 * @note To access the actual structure containing the node, cast the `pos`
 *       pointer to your container struct type.
 *
 */
#define m3u8_list_foreach(pos, head) \
  for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/**
 * @struct m3u8_list_node
 * @brief Node structure for a circular doubly-linked list.
 */
typedef struct m3u8_list_node {
  struct m3u8_list_node* next; /**< Pointer to the next node in the list. */
  struct m3u8_list_node* prev; /**< Pointer to the previous node in the list. */
} m3u8_list_node_t;

/**
 * @brief Allocates and initializes a new circular list head node.
 *
 * @param[out] list Double pointer to a m3u8_list_node_t; will point to the newly allocated head node.
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If list is NULL.
 * @retval M3U8_LIST_STATUS_MEM_ALL_ERROR If memory allocation fails.
 */
int m3u8_list_init(m3u8_list_node_t* list);

/**
 * @brief Inserts a node immediately after the head node.
 *
 * @param[in] head Pointer to the list head node (must not be NULL).
 * @param[in] node Pointer to the new node to insert (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_add_head(m3u8_list_node_t* head, m3u8_list_node_t* node);

/**
 * @brief Inserts a node immediately before the head node (at the end of the list).
 *
 * @param[in] node Pointer to the new node to insert (must not be NULL).
 * @param[in] head Pointer to the list head node (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_add_tail(m3u8_list_node_t* node, m3u8_list_node_t* head);

/**
 * @brief Removes a node from the list. Does not free its memory.
 *
 * @param[in] node Pointer to the node to remove (must not be NULL and must be linked).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If node or its neighbors are NULL.
 */
int m3u8_list_remove(m3u8_list_node_t* node);

/**
 * @brief Checks if the list is empty.
 *
 * @param[in]  head     Pointer to the list head node (must not be NULL).
 * @param[out] is_empty Pointer to a bool where result will be stored (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If head or is_empty is NULL.
 */
int m3u8_list_empty(const m3u8_list_node_t* head, bool* is_empty);

/**
 * @brief Counts the number of nodes in the list (excluding the head node).
 *
 * @param[in]  head Pointer to the list head node (must not be NULL).
 * @param[out] size Pointer to an int where count will be stored (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If head or size is NULL.
 */
int m3u8_list_count(const m3u8_list_node_t* head, int* size);

#endif  // __h_m3u8_list__
