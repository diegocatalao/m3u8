/**
 * @file m3u8.h
 * @brief M3U8 media parser and metadata structures.
 */
#ifndef _M3U8_H_
#define _M3U8_H_

#include "linked.h"
#include "master.h"
#include "media.h"
#include "parser.h"
#include "validate.h"

#include <stdbool.h>
#include <stdint.h>

/** @brief Represents the status of an M3U8 operation. */
typedef enum {
  /** No error occurred */
  M3U8_STATUS_NO_ERROR = 0,
  /** Invalid argument provided */
  M3U8_STATUS_INVALID_ARG = 1,
  /** Memory allocation error */
  M3U8_STATUS_MEM_ALLOC_ERROR = 2,
  /** Invalid master format */
  M3U8_STATUS_INVALID_MASTER_PLAYLIST = 3,
  /** Invalid media format */
  M3U8_STATUS_INVALID_MEDIA_PLAYLIST = 4,
  /** Invalid attributes format */
  M3U8_STATUS_INVALID_ATTRS = 5,
  /** Fail to parse current manifest string */
  M3U8_STATUS_MANIFEST_PARSE_FAIL = 6,
  /** When received an invalid manifest */
  M3U8_STATUS_INVALID_MANIFEST = 7,
  /** An unknown or unspecified error occurred. */
  M3U8_STATUS_UNKNOWN_ERROR = 99,
} m3u8_status_t;

/** @brief Represents an M3U8 media, which can be a master or media. */
typedef struct {
  /** Master playlist with differents video formats */
  m3u8_master_t* master;
  /** Media playlist with differents types and formats */
  m3u8_media_t* media;
} m3u8_t;

/**
 * @brief Allocates and initializes a new m3u8_t structure.
 * @details The pointer @p m3u8 cannot be NULL but the content must be NULL.
 * @param m3u8 double pointer to an @ref m3u8_t.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 is not NULL.
 * @return @ref M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 * @return @ref M3U8_STATUS_UNKNOWN_ERROR on unexpected failure.
 */
m3u8_status_t m3u8_create(m3u8_t** m3u8);

/**
 * @brief Deallocates and cleans up a previously created m3u8_t structure.
 * @param m3u8 pointer to an @ref m3u8_t structure to be deallocated.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 is NULL.
 */
m3u8_status_t m3u8_destroy(m3u8_t* m3u8);

/**
 * @brief Fetches and parses an M3U8 media from a string.
 * @param m3u8   pointer to a valid @ref m3u8_t structure to be filled.
 * @param manifest string of current master.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 or @p manifest are NULL.
 * @return @ref M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 * @return @ref M3U8_STATUS_UNKNOWN_ERROR on unexpected failure.
 */
m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* manifest);

/**
 * @brief Validates an M3U8 media.
 * @param m3u8 Pointer to the @ref m3u8_t object to validate.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 is NULL.
 * @return @ref M3U8_STATUS_INVALID_MANIFEST if the master playlist is invalid.
 * @return @ref M3U8_STATUS_INVALID_PLAYLIST if the media playlist is invalid.
 */
m3u8_status_t m3u8_validate(m3u8_t* m3u8);

#endif  // _M3U8_H_
