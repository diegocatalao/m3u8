/**
 * @file attr.h
 * @brief Key-value attribute parsing for M3U8 playlists.
 */

#ifndef _M3U8_ATTR_H_
#define _M3U8_ATTR_H_

#include "list.h"

/**
 * @enum m3u8_attr_status_t
 * @brief Represents the status of an attribute-related operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_ATTR_STATUS_NO_ERROR = 0,
  /** Invalid argument passed to a function. */
  M3U8_ATTR_STATUS_INVALID_ARG = 1,
  /** Memory allocation error. */
  M3U8_ATTR_STATUS_MEM_ALLOC_ERROR = 2,
  /** Regular expression compilation failed. */
  M3U8_ATTR_STATUS_REG_PATTERN_ERROR = 3,
  /** List operation failed. */
  M3U8_ATTR_STATUS_LIST_ERROR = 4,
  /** The inserted key/value was not found. */
  M3U8_ATTR_STATUS_NOT_FOUND = 5,
  /** Unknown error occurred. */
  M3U8_ATTR_STATUS_UNKNOWN_ERROR = 99,
} m3u8_attr_status_t;

/**
 * @struct m3u8_attr_t
 * @brief Represents a single key-value attribute parsed from an M3U8 tag.
 */
typedef struct {
  /** The attribute key (e.g., "BANDWIDTH"). */
  char* key;
  /** The attribute value (e.g., "1280000"). */
  char* value;
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
} m3u8_attr_t;

/**
 * @brief Parses a buffer containing key-value attributes.
 * @param buffer The null-terminated string containing tag attributes.
 * @param attrs  Pointer to an @ref m3u8_attr_t structure where parsed attributes will be stored.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p buffer or @p attrs is NULL.
 * @return @ref M3U8_ATTR_STATUS_MEM_ALLOC_ERROR if memory allocation fails.
 * @return @ref M3U8_ATTR_STATUS_REG_PATTERN_ERROR if regex compilation fails.
 * @return @ref M3U8_ATTR_STATUS_LIST_ERROR if insertion into the list fails.
 */
int m3u8_attr_parse(char* buffer, m3u8_attr_t* attrs);

/**
 * @brief Retrieves an attribute from the list by its key.
 * @param attrs Pointer to the attribute list.
 * @param attr  Double pointer to store the found attribute.
 * @param key   The key of the attribute to find.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_NOT_FOUND if the key is not found.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs, @p attr, or @p key is NULL.
 */
m3u8_attr_status_t m3u8_attr_from_key(m3u8_attr_t* attrs, m3u8_attr_t** attr,
                                      char* key);

/**
 * @brief Counts the number of attributes in the list.
 * @param attrs Pointer to the attribute list.
 * @param size  Pointer to an integer where the count will be stored.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs or @p size is NULL.
 */
m3u8_attr_status_t m3u8_attr_count(m3u8_attr_t* attrs, int* size);

/**
 * @brief Frees all memory associated with the attribute list.
 * @param attrs Pointer to the attribute list.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs is NULL.
 */
m3u8_attr_status_t m3u8_attr_destroy(m3u8_attr_t* attrs);

#endif  // _M3U8_ATTR_H_
