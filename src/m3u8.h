/**
 * @file m3u8.h
 * @brief M3U8 playlist parser and metadata structures.
 */
#ifndef _M3U8_H_
#define _M3U8_H_

#include <stdbool.h>
#include <stdint.h>

#include "attr.h"
#include "list.h"
#include "manifest.h"
#include "playlist.h"

/** @brief Represents the status of an M3U8 operation. */
typedef enum {
  /**< No error occurred */
  M3U8_STATUS_NO_ERROR = 0,
  /**< Invalid argument provided */
  M3U8_STATUS_INVALID_ARG = 1,
  /**< Memory allocation error */
  M3U8_STATUS_MEM_ALLOC_ERROR = 2,
  /**< Invalid manifest format */
  M3U8_STATUS_INVALID_MANIFEST = 3,
  /**< Invalid playlist format */
  M3U8_STATUS_INVALID_PLAYLIST = 4,
  /**< Invalid attributes format */
  M3U8_STATUS_INVALID_ATTRS = 5,
  /**< Unknown or unexpected error */
  M3U8_STATUS_UNKNOWN_ERROR = 6,
} m3u8_status_t;

/** @brief Represents the type of an M3U8 playlist. */
typedef enum {
  /**< represents a master playlist */
  MASTER_PLAYLIST,
  /**< represents a media playlist */
  MEDIA_PLAYLIST,
  /**< represents a unknown playlist */
  UNKNOWN_PLAYLIST,
} manifest_type_t;

/** @brief Represents an M3U8 playlist, which can be a master or media. */
typedef struct {
  manifest_t* manifest; /** master playlist with differents video formats */
  playlist_t* playlist; /** media playlist with differents types and formats */
} m3u8_t;

/**
 * @brief Allocates and initializes a new m3u8_t structure.
 *
 * @param m3u8 double pointer to an m3u8_t. Must be NULL on input.
 *
 * @return M3U8_STATUS_NO_ERROR        on success;
 *         M3U8_STATUS_INVALID_ARG if  *m3u8 is not NULL;
 *         M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure;
 *         M3U8_STATUS_UNKNOWN_ERROR   on unexpected failure.
 */
m3u8_status_t m3u8_create(m3u8_t** m3u8);

/**
 * @brief Deallocates and cleans up a previously created m3u8_t structure.
 *
 * @param m3u8 pointer to an m3u8_t structure to be deallocated.
 *
 * @return M3U8_STATUS_NO_ERROR       on success.
 *         M3U8_STATUS_INVALID_ARG if m3u8 is NULL.
 */
m3u8_status_t m3u8_destroy(m3u8_t* m3u8);

/**
 * @brief Fetches and parses an M3U8 playlist from a string.
 *
 * @param m3u8   pointer to a valid m3u8_t structure to be filled.
 * @param buffer string of current manifest.
 *
 * @return M3U8_STATUS_NO_ERROR        on success.
 *         M3U8_STATUS_INVALID_ARG     if m3u8 or buffer are NULL.
 *         M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 *         M3U8_STATUS_UNKNOWN_ERROR   on unexpected failure.
 */
m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* buffer);

/**
 * @brief Check if is a master or media playlist.
 *
 * @param buffer string of current manifest.
 *
 * @return MASTER_PLAYLIST    a master playlist from buffer.
 *         MEDIA_PLAYLIST     a media playlist from buffer.
 *         UNKNOWN_PLAYLIST   a unknown playlist if no valid arguments
 *                            are received.
 */
manifest_type_t m3u8_get_playlist_type(const char* buffer);

#endif  // _M3U8_H_
