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
 * @details Returned when a function executes without error.
 */
#define M3U8_LIST_STATUS_NO_ERROR      0x10000000

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
 * @brief Retrieves a pointer to the structure that contains a given member 
 * pointer.
 *
 * @param ptr    A pointer to the member field within the structure.
 * @param type   The type of the structure that contains the member.
 * @param member The name of the member field within the structure.
 *
 * @return A pointer to the structure of type @p type that contains the member 
 * @p ptr.
 *
 * @details This macro computes the address of the container structure by
 *          subtracting the offset of the member field from the member's 
 *          address. It is commonly used in conjunction with generic data 
 *          structures (like linked lists) where the list node is embedded 
 *          inside user-defined structures.
 */
#define m3u8_list_container_of(ptr, type, member) \
  ((type*)((char*)(ptr) - offsetof(type, member)))

/**
 * @brief Gets the next entry in the list from the current container.
 *
 * @param pos    A pointer to the current structure.
 * @param member The name of the list field in the structure.
 *
 * @return A pointer to the next structure in the list.
 */
#define m3u8_list_next(pos, type, member) \
  m3u8_list_container_of((pos)->member.next, type, member)

/**
 * @brief Gets the previous entry in the list from the current container.
 *
 * @param pos    A pointer to the current structure.
 * @param type   The type of the structure that contains the list node.
 * @param member The name of the list field in the structure.
 *
 * @return A pointer to the previous structure in the list.
 */
#define m3u8_list_prev(pos, type, member) \
  m3u8_list_container_of((pos)->member.prev, type, member)

/**
 * @brief Iterates over each element of a circular doubly linked list.
 *
 * @param entry A pointer of type (type*) used as the loop cursor; it will 
 *              point to each structure containing a list node.
 * @param head  A pointer to the list head node (of type m3u8_list_node_t*).
 * @param type  The type of the structure embedding the list node.
 * @param member The name of the list node field within the structure.
 *
 * @details This macro abstracts the iteration over a circular doubly linked 
 *          list of embedded nodes. It automatically applies the container_of 
 *          pattern to retrieve the enclosing structure from each list node.
 */
#define m3u8_list_foreach(entry, head, type, member)           \
  for (m3u8_list_node_t* _pos = (head)->next;                  \
       _pos != (head) &&                                       \
       ((entry) = m3u8_list_container_of(_pos, type, member)); \
       _pos = _pos->next)

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
int m3u8_list_init(m3u8_list_node_t* head);

/**
 * @brief Inserts a node immediately after the head node.
 *
 * @param[in] head Pointer to the list head node (must not be NULL).
 * @param[in] node Pointer to the new node to insert (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_ina(m3u8_list_node_t* head, m3u8_list_node_t* node);

/**
 * @brief Inserts a node immediately before the head node (at the end of the list).
 *
 * @param[in] node Pointer to the new node to insert (must not be NULL).
 * @param[in] head Pointer to the list head node (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If any pointer is NULL.
 */
int m3u8_list_inb(m3u8_list_node_t* head, m3u8_list_node_t* node);

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
int m3u8_list_is_empty(const m3u8_list_node_t* head, bool* is_empty);

/**
 * @brief Counts the number of nodes in the list (excluding the head node).
 *
 * @param[in]  head Pointer to the list head node (must not be NULL).
 * @param[out] size Pointer to an int where count will be stored (must not be NULL).
 * @retval M3U8_LIST_STATUS_NO_ERROR      On success.
 * @retval M3U8_LIST_STATUS_INVALID_ARGS  If head or size is NULL.
 */
int m3u8_list_count(const m3u8_list_node_t* head, int* size);

#endif  // __H_M3U8_LIST__
