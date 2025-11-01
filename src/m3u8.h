/**
 * @file m3u8.h
 * @brief M3U8 media parser and metadata structures.
 */
#ifndef _M3U8_H_
#define _M3U8_H_

#include <stdbool.h>
#include <stdint.h>

#include "attr.h"
#include "list.h"
#include "master.h"
#include "media.h"

/** @brief Represents the status of an M3U8 operation. */
typedef enum {
  M3U8_STATUS_NO_ERROR = 0,                /**< No error occurred */
  M3U8_STATUS_INVALID_ARG = 1,             /**< Invalid argument provided */
  M3U8_STATUS_MEM_ALLOC_ERROR = 2,         /**< Memory allocation error */
  M3U8_STATUS_INVALID_MASTER_PLAYLIST = 3, /**< Invalid master format */
  M3U8_STATUS_INVALID_MEDIA_PLAYLIST = 4,  /**< Invalid media format */
  M3U8_STATUS_INVALID_ATTRS = 5,           /**< Invalid attributes format */
  M3U8_STATUS_UNKNOWN_ERROR = 6,           /**< Unknown or unexpected error */
} m3u8_status_t;

/** @brief Represents the type of an M3U8 media. */
typedef enum {
  M3U8_MASTER_PLAYLIST,  /**< Represents a master media */
  M3U8_MEDIA_PLAYLIST,   /**< Represents a media media */
  M3U8_UNKNOWN_PLAYLIST, /**< Represents a unknown media */
} playlist_type_t;

/** @brief Represents an M3U8 media, which can be a master or media. */
typedef struct {
  master_t* master; /** Master playlist with differents video formats */
  media_t*  media;  /** Media playlist with differents types and formats */
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
 * @details
 * @param m3u8 pointer to an @ref m3u8_t structure to be deallocated.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 is NULL.
 */
m3u8_status_t m3u8_destroy(m3u8_t* m3u8);

/**
 * @brief Fetches and parses an M3U8 media from a string.
 * @param m3u8   pointer to a valid @ref m3u8_t structure to be filled.
 * @param buffer string of current master.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p m3u8 or @p buffer are NULL.
 * @return @ref M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 * @return @ref M3U8_STATUS_UNKNOWN_ERROR on unexpected failure.
 */
m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* buffer);

/**
 * @brief Validates a master playlist against HLS specification rules.
 * @param master pointer to a @ref master_t structure to be validated.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p master is NULL.
 * @return @ref M3U8_STATUS_INVALID_MASTER_PLAYLIST if the playlist fails
 *              validation.
 */
m3u8_status_t m3u8_validate_master_playlist(master_t* master);

/**
 * @brief Validates a media playlist against HLS specification rules.
 * @param media pointer to a @ref media_t structure to be validated.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p media is NULL.
 * @return @ref M3U8_STATUS_INVALID_MEDIA_PLAYLIST if the playlist fails
 *              validation.
 */
m3u8_status_t m3u8_validate_media_playlist(media_t* media);

/**
 * @brief Check if is a master or media media.
 * @param buffer string of current master.
 * @return @ref M3U8_MASTER_PLAYLIST a master media from buffer.
 * @return @ref M3U8_MEDIA_PLAYLIST a media media from buffer.
 * @return @ref M3U8_UNKNOWN_PLAYLIST a unknown media if no valid arguments are
 *              received.
 */
playlist_type_t m3u8_get_playlist_type(const char* buffer);

#endif  // _M3U8_H_
