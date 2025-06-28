/**
 * @file attr.h
 * @brief Key-value attribute parsing for M3U8 playlists.
 */

#ifndef __H_M3U8_ATTR__
#define __H_M3U8_ATTR__

#include "list.h"

/**
 * @brief Operation completed successfully.
 *
 * @details Returned when a function completes without error.
 */
#define M3U8_ATTR_STATUS_NO_ERROR          0x03

/**
 * @brief Invalid argument passed to a function.
 *
 * @details Returned when a parameter is NULL or otherwise invalid.
 */
#define M3U8_ATTR_STATUS_INVALID_ARG       (M3U8_ATTR_STATUS_NO_ERROR + 0x01)

/**
 * @brief Memory allocation error.
 *
 * @details Returned when memory allocation or reallocation fails.
 */
#define M3U8_ATTR_STATUS_MEM_ALLOC_ERROR   (M3U8_ATTR_STATUS_NO_ERROR + 0x02)

/**
 * @brief Regular expression compilation failed.
 *
 * @details Returned when the regular expression used to parse attributes 
 *          cannot be compiled.
 */
#define M3U8_ATTR_STATUS_REG_PATTERN_ERROR (M3U8_ATTR_STATUS_NO_ERROR + 0x03)

/**
 * @brief List operation failed.
 *
 * @details Returned when inserting or managing attributes in the internal 
 *          list fails.
 */
#define M3U8_ATTR_STATUS_LIST_ERROR        (M3U8_ATTR_STATUS_NO_ERROR + 0x04)

/**
 * @brief The inserted key/value was not found.
 *
 * @details Returned when given key/value from function was not found in list.
 */
#define M3U8_ATTR_STATUS_NOT_FOUND         (M3U8_ATTR_STATUS_NO_ERROR + 0x05)

/**
 * @brief Unknown error occurred.
 *
 * @details Returned when an unexpected or undefined error occurs.
 */
#define M3U8_LIST_STATUS_UNKNOWN_ERROR     (M3U8_LIST_STATUS_NO_ERROR + 0x99)

/**
 * @struct m3u8_attr_t
 * @brief Represents a single key-value attribute parsed from an M3U8 tag 
 *        attribute string.
 */
typedef struct {
  char*            key;   /**< The attribute key (e.g., "BANDWIDTH"). */
  char*            value; /**< The attribute value (e.g., "1280000"). */
  m3u8_list_node_t list;  /**< Embedded node for circular linked list. */
} m3u8_attr_t;

/**
 * @brief Parses a buffer containing key-value attributes.
 *
 * @param[in]  buffer The null-terminated string containing tag attributes.
 * @param[out] attrs  Pointer to a m3u8_attr_t structure where parsed 
 *                    attributes will be stored.
 *
 * @retval M3U8_ATTR_STATUS_NO_ERROR          On success.
 * @retval M3U8_ATTR_STATUS_INVALID_ARG       If buffer or attrs is NULL.
 * @retval M3U8_ATTR_STATUS_MEM_ALLOC_ERROR   If memory allocation fails.
 * @retval M3U8_ATTR_STATUS_REG_PATTERN_ERROR If regex compilation fails.
 * @retval M3U8_ATTR_STATUS_LIST_ERROR        If insertion into the list fails.
 */
int m3u8_attr_parse(char* buffer, m3u8_attr_t* attrs);

/**
 * @brief Retrieves the key from the first attribute in the list.
 *
 * @param[in]  attrs Pointer to the attribute list.
 * @param[in]  attr  Double pointer to found attr on list.
 * @param[out] key   Address where the key pointer will be stored.
 *
 * @retval M3U8_ATTR_STATUS_NO_ERROR     On success.
 * @retval M3U8_ATTR_STATUS_INVALID_ARG  If attrs or key_ptr is NULL.
 */
int m3u8_attr_from_key(m3u8_attr_t* attrs, m3u8_attr_t** attr, char* key);

/**
 * @brief Counts the number of attributes in the list.
 *
 * @param[in]  attrs Pointer to the attribute list.
 * @param[out] size  Pointer to an integer where the count will be stored.
 *
 * @retval M3U8_ATTR_STATUS_NO_ERROR     On success.
 * @retval M3U8_ATTR_STATUS_INVALID_ARG  If attrs or size is NULL.
 */
int m3u8_attr_count(m3u8_attr_t* attrs, int* size);

/**
 * @brief Frees all memory associated with the attribute list.
 *
 * @param[in,out] attrs Pointer to the attribute list.
 *
 * @retval M3U8_ATTR_STATUS_NO_ERROR     On success.
 * @retval M3U8_ATTR_STATUS_INVALID_ARG  If attrs is NULL.
 */
int m3u8_attr_destroy(m3u8_attr_t* attrs);

#endif  // __H_M3U8_ATTR__
