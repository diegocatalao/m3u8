#ifndef _M3U8_PARSER_H_
#define _M3U8_PARSER_H_

#include "linked.h"

/**
 * @enum m3u8_parser_status_t
 * @brief Represents the status of a validation operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_PARSER_STATUS_NO_ERROR = 0,
  /** A validation rule was not met. */
  M3U8_PARSER_STATUS_FAILURE = 1,
  /** Invalid argument provided to a function. */
  M3U8_PARSER_STATUS_INVALID_ARG = 2,
  /** Memory allocation failed. */
  M3U8_PARSER_STATUS_MEM_ALLOC_ERROR = 3,
  /** An error occurred when trying to manipulate the list */
  M3U8_PARSER_STATUS_LIST_ERROR = 4,
  /** Given a key from list, attribute was not found */
  M3U8_PARSER_STATUS_NOT_FOUND = 5,
  /** An unknown or unspecified error occurred. */
  M3U8_PARSER_STATUS_UNKNOWN_ERROR = 99,
} m3u8_parser_status_t;

/**
 * @enum m3u8_parser_tag_t
 * @brief Represents all possible M3U8 tags identified by the parser.
 */
typedef enum {
  /** Indicates the file is an M3U8 playlist. */
  M3U8_PARSER_EXTM3U = 0,
  /** Specifies the playlist compatibility version. */
  M3U8_PARSER_EXT_X_VERSION,
  /** Samples can be decoded without other segments. */
  M3U8_PARSER_EXT_X_INDEPENDENT_SEGMENTS,
  /** Specifies a playlist of I-frames. */
  M3U8_PARSER_EXT_X_I_FRAME_STREAM_INF,
  /** Relates playlists with alternative renditions. */
  M3U8_PARSER_EXT_X_MEDIA,
  /** Allows arbitrary session data in a master playlist. */
  M3U8_PARSER_EXT_X_SESSION_DATA,
  /** Provides encryption keys for all media segments. */
  M3U8_PARSER_EXT_X_SESSION_KEY,
  /** Indicates a preferred starting point for a playlist. */
  M3U8_PARSER_EXT_X_START,
  /** Specifies a variant stream for adaptive playback. */
  M3U8_PARSER_EXT_X_STREAM_INF,
  /** Provides mutability info of the playlist (VOD/EVENT). */
  M3U8_PARSER_EXT_X_PLAYLIST_TYPE,
  /** Indicates that each segment describes a single I-frame. */
  M3U8_PARSER_EXT_X_I_FRAMES_ONLY,
  /** Specifies the maximum media segment duration. */
  M3U8_PARSER_EXT_X_TARGETDURATION,
  /** Sequence number of the first segment in a playlist. */
  M3U8_PARSER_EXT_X_MEDIA_SEQUENCE,
  /** Allows synchronization between different renditions. */
  M3U8_PARSER_EXT_X_DISCONTINUITY_SEQUENCE,
  /** Specifies the duration of a media segment. */
  M3U8_PARSER_EXTINF,
  /** Indicates a segment is a sub-range of its URI. */
  M3U8_PARSER_EXT_X_BYTERANGE,
  /** Indicates a discontinuity between segments. */
  M3U8_PARSER_EXT_X_DISCONTINUITY,
  /** Specifies how to decrypt media segments. */
  M3U8_PARSER_EXT_X_KEY,
  /** Specifies how to obtain the Media Initialization Section. */
  M3U8_PARSER_EXT_X_MAP,
  /** Associates a segment with an absolute date/time. */
  M3U8_PARSER_EXT_X_PROGRAM_DATE_TIME,
  /** Associates a date range with a set of attributes. */
  M3U8_PARSER_EXT_X_DATERANGE,
  /** Indicates no more segments will be added to the playlist. */
  M3U8_PARSER_EXT_X_ENDLIST,
  /** Indicates a unknown tag assigned, but can be a URI too */
  M3U8_PARSER_EXT_UNKNOWN,
} m3u8_parser_tag_t;

/** @brief Represents the type of an M3U8 media. */
typedef enum {
  M3U8_PARSER_MASTER_PLAYLIST,  /**< Represents a master media */
  M3U8_PARSER_MEDIA_PLAYLIST,   /**< Represents a media media */
  M3U8_PARSER_UNKNOWN_PLAYLIST, /**< Represents a unknown media */
} m3u8_parser_playlist_type_t;

/**
 * @struct m3u8_parser_attr_t
 * @brief Represents a single key-value attribute parsed from an M3U8 tag.
 */
typedef struct {
  /** The attribute key (e.g., "BANDWIDTH"). */
  char *key;
  /** The attribute value (e.g., "1280000"). */
  char *value;
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
} m3u8_parser_attr_t;

/**
 * @struct m3u8_parser_t
 * @brief Represents a parsed line from playlist.
 */
typedef struct {
  /** The tag name string from RFC tag list */
  char *name;
  /** The value of this tag, NULL if tag has no value */
  char *value;
  /** The tag identifier from RFC tag list */
  m3u8_parser_tag_t tag;
  /** Parsed attributes from this tag, NULL if tag has no attributes  */
  m3u8_parser_attr_t *attrs;
} m3u8_parser_t;

/**
 * @brief Parses a line containing key-value attributes.
 * @param line The null-terminated string containing tag attributes.
 * @param attrs  Pointer to an @ref m3u8_parser_attr_t structure where parsed
 *               attributes will be stored.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p line or @p attrs is NULL.
 * @return @ref M3U8_ATTR_STATUS_MEM_ALLOC_ERROR if memory allocation fails.
 * @return @ref M3U8_ATTR_STATUS_REG_PATTERN_ERROR if regex compilation fails.
 * @return @ref M3U8_ATTR_STATUS_LIST_ERROR if insertion into the list fails.
 */
m3u8_parser_status_t m3u8_parser_attr(char *line, m3u8_parser_attr_t *attrs);

/**
 * @brief Retrieves an attribute from the list by its key.
 * @param attrs Pointer to the attribute list.
 * @param attr  Double pointer to store the found attribute.
 * @param key   The key of the attribute to find.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_NOT_FOUND if the key is not found.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs, @p attr, or @p key is NULL.
 */
m3u8_parser_status_t m3u8_parser_attr_from_key(m3u8_parser_attr_t *attrs, m3u8_parser_attr_t **attr,
                                               char *key);

/**
 * @brief Counts the number of attributes in the list.
 * @param attrs Pointer to the attribute list.
 * @param size  Pointer to an integer where the count will be stored.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs or @p size is NULL.
 */
m3u8_parser_status_t m3u8_parser_attr_count(m3u8_parser_attr_t *attrs, int *size);

/**
 * @brief Frees all memory associated with the attribute list.
 * @param attrs Pointer to the attribute list.
 * @return @ref M3U8_ATTR_STATUS_NO_ERROR on success.
 * @return @ref M3U8_ATTR_STATUS_INVALID_ARG if @p attrs is NULL.
 */
m3u8_parser_status_t m3u8_parser_attr_destroy(m3u8_parser_attr_t *attrs);

/**
 * @brief Parses an M3U8 string and initializes a parser object.
 * @param line The M3U8 string line to be parsed.
 * @param parser Double pointer to an @ref m3u8_parser_t object that will be created.
 * @return @ref M3U8_PARSER_STATUS_NO_ERROR on success.
 * @return @ref M3U8_PARSER_STATUS_INVALID_ARG if @p line or @p parser is NULL.
 * @return @ref M3U8_PARSER_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
m3u8_parser_status_t m3u8_parser_from_str(const char *line, m3u8_parser_t **parser);

/**
 * @brief Converts a string to an M3U8 parser tag enum.
 * @param line The input string containing the M3U8 tag (e.g., "#EXTM3U").
 * @return The corresponding `m3u8_parser_tag_t` enum value if a match is found.
 */
m3u8_parser_tag_t m3u8_parser_tag_from_str(const char *line);

/**
 * @brief Check if is a master or media media.
 * @param manifest string of current manifest.
 * @return @ref M3U8_MASTER_PLAYLIST a master playlist from manifest.
 * @return @ref M3U8_MEDIA_PLAYLIST a media playlist from manifest.
 * @return @ref M3U8_UNKNOWN_PLAYLIST a unknown media if no valid arguments are received.
 */
m3u8_parser_playlist_type_t m3u8_parser_playlist_type(const char *manifest);

#endif // _M3U8_PARSER_H_
