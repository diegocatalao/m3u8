#ifndef _M3U8_PLAYLIST_H_
#define _M3U8_PLAYLIST_H_

#include <stdbool.h>
#include <stdint.h>

#include "attr.h"
#include "list.h"

typedef enum {
  /**< Operation completed successfully. */
  M3U8_PLAYLIST_STATUS_NO_ERROR,
  /**< Invalid argument provided to a function. */
  M3U8_PLAYLIST_STATUS_INVALID_ARG,
  /**< Memory allocation failed. */
  M3U8_PLAYLIST_STATUS_MEM_ALLOC_ERROR,
  /**< Invalid or malformed playlist. */
  M3U8_PLAYLIST_STATUS_INVALID_PLAYLIST,
  /**< An unknown or unspecified error occurred. */
  M3U8_PLAYLIST_STATUS_UNKNOWN_ERROR,
} m3u8_playlist_status_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< duration of the media segment in seconds (#EXTINF) */
  double duration;
  /**< optional title or description of the segment */
  char* title;
  /**< URI of the media segment */
  char* uri;
} ext_x_extinf_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< length of the byte range in bytes */
  uint64_t length;
  /**< optional start offset in bytes */
  uint64_t offset;
} ext_x_byterange_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< cryptography method  */
  char* method;
  /**< link to remote key resource, usually an .key file */
  char* uri;
  /**< an initialization vector in hex used by AES */
  uint8_t iv[16];
  char*   key_format; /** indicates the key format, default is "identity" */
  /**< indicates the version of the key, default 1 */
  char* key_format_versions;
} ext_x_key_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< URI to the initialization segment */
  char* uri;
  /**< optional byte range length */
  char* byterange;
} ext_x_map_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< unique identifier for the date range */
  char* id;
  /**< optional class attribute */
  char* class;
  /**< ISO 8601 start datetime */
  char* start_date;
  /**< ISO 8601 end datetime, optional */
  char* end_date;
  /**< optional, duration of the event */
  double duration;
  /**< optional, planned duration */
  double planned_duration;
  /**< optional, SCTE-35 cue command */
  char* scte35_cmd;
  /**< optional, SCTE-35 out cue */
  char* scte35_out;
  /**< optional, SCTE-35 in cue */
  char* scte35_in;
  /**< optional, stop playlist when in next segment */
  bool is_end_on_next;
  /**< raw string with other optional attributes */
  m3u8_attr_t attributes;
} ext_x_daterange_t;

typedef struct {
  /**< embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< if the playlist is started by EXTM3U taga */
  bool is_signed;
  /**< HLS protocol version number, only accepts > 7 */
  uint8_t version;
  /**< if can be decoded without prev. segments */
  bool is_independent_segments;
  /** true if the next segment starts a discontinuity */
  bool is_discontinuity;
  /**< playlist type, e.g., VOD or EVENT */
  char* type;
  /**< true if the playlist contains only i-frames */
  bool is_i_frames_only;
  /**< maximum segment duration in seconds */
  int target_duration;
  /**< sequence number of the first media segment */
  int media_sequence;
  /**< sequence number for discontinuities */
  int discontinuity_sequence;
  /**< segment duration and info */
  ext_x_extinf_t* ext_x_extinf;
  /**< optional byte range for the current segment */
  ext_x_byterange_t* ext_x_byterange;
  /**< encryption key info */
  ext_x_key_t* ext_x_key;
  /**< initialization segment info */
  ext_x_map_t* ext_x_map;
  /**< ISO 8601 timestamp for the current segment */
  char* program_date_time;
  /**< date range info */
  ext_x_daterange_t* ext_x_daterange;
  /**< true if the playlist ends with #EXT-X-ENDLIST */
  bool is_endlist;
} playlist_t;

/**
 * @brief Creates a new M3U8 media playlist object.
 * @details Allocates and initializes a new @c playlist_t object. The caller is
 *          responsible for freeing the allocated memory by calling
 *          @c m3u8_playlist_destroy().
 * @param[out] playlist A pointer to a @c playlist_t* that will be populated
 *                      with the address of the newly created object.
 * @return One of the status codes from @ref m3u8_playlist_status_t.
 */
m3u8_playlist_status_t m3u8_playlist_create(playlist_t** playlist);

/**
 * @brief Destroys an M3U8 media playlist object.
 * @details Frees all memory associated with a @c playlist_t object.
 * @param[in] playlist The media playlist object to destroy.
 * @return One of the status codes from @ref m3u8_playlist_status_t.
 */
m3u8_playlist_status_t m3u8_playlist_destroy(playlist_t* playlist);

/**
 * @brief Validates the integrity and conformance of a media playlist object.
 * @details Checks the media playlist object against the HLS specification rules.
 * @param[in] playlist The media playlist object to validate.
 * @return Returns @c M3U8_PLAYLIST_STATUS_NO_ERROR if the playlist is valid.
 *         Otherwise, it returns a status code indicating the specific
 *         validation error, such as @c M3U8_PLAYLIST_STATUS_INVALID_PLAYLIST.
 */
m3u8_playlist_status_t m3u8_playlist_validate(playlist_t* playlist);

/**
 * @brief Parses an M3U8 media playlist from a string.
 * @details Fills a @c playlist_t object with data parsed from the provided
 *          buffer. The playlist object must be allocated prior to calling this
 *          function.
 * @param[in,out] playlist A pointer to a valid @c playlist_t object to be filled.
 * @param[in]     buffer   The string buffer containing the media playlist content.
 * @return One of the status codes from @ref m3u8_playlist_status_t, with
 *         @c M3U8_PLAYLIST_STATUS_NO_ERROR on success.
 */
m3u8_playlist_status_t m3u8_playlist_parse(playlist_t* playlist, char* buffer);

#endif  // _M3U8_PLAYLIST_H_
