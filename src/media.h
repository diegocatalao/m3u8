#ifndef _M3U8_MEDIA_H_
#define _M3U8_MEDIA_H_

#include <stdbool.h>
#include <stdint.h>

#include "attr.h"
#include "list.h"

/**
 * @enum m3u8_media_status_t
 * @brief Represents the status of a media playlist operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_MEDIA_STATUS_NO_ERROR,
  /** Invalid argument provided to a function. */
  M3U8_MEDIA_STATUS_INVALID_ARG,
  /** Memory allocation failed. */
  M3U8_MEDIA_STATUS_MEM_ALLOC_ERROR,
  /** Invalid or malformed media playlist. */
  M3U8_MEDIA_STATUS_INVALID_PLAYLIST,
  /** An unknown or unspecified error occurred. */
  M3U8_MEDIA_STATUS_UNKNOWN_ERROR,
} m3u8_media_status_t;

/**
 * @struct ext_x_extinf_t
 * @brief Represents an EXTINF tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Duration of the media segment in seconds. */
  double duration;
  /** Optional title or description of the segment. */
  char* title;
  /** URI of the media segment. */
  char* uri;
} ext_x_extinf_t;

/**
 * @struct ext_x_byterange_t
 * @brief Represents an EXT-X-BYTERANGE tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Length of the byte range in bytes. */
  uint64_t length;
  /** Optional start offset in bytes. */
  uint64_t offset;
} ext_x_byterange_t;

/**
 * @struct ext_x_key_t
 * @brief Represents an EXT-X-KEY tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Cryptography method. */
  char* method;
  /** Link to remote key resource. */
  char* uri;
  /** Initialization vector in hex. */
  uint8_t iv[16];
  /** Key format, default is "identity". */
  char* key_format;
  /** Key format versions, default is 1. */
  char* key_format_versions;
} ext_x_key_t;

/**
 * @struct ext_x_map_t
 * @brief Represents an EXT-X-MAP tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** URI to the initialization segment. */
  char* uri;
  /** Optional byte range. */
  char* byterange;
} ext_x_map_t;

/**
 * @struct ext_x_daterange_t
 * @brief Represents an EXT-X-DATERANGE tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Unique identifier for the date range. */
  char* id;
  /** Optional class attribute. */
  char* class;
  /** ISO 8601 start datetime. */
  char* start_date;
  /** Optional ISO 8601 end datetime. */
  char* end_date;
  /** Optional duration of the event. */
  double duration;
  /** Optional planned duration. */
  double planned_duration;
  /** Optional SCTE-35 cue command. */
  char* scte35_cmd;
  /** Optional SCTE-35 out cue. */
  char* scte35_out;
  /** Optional SCTE-35 in cue. */
  char* scte35_in;
  /** Optional, stop media when in next segment. */
  bool is_end_on_next;
  /** Raw string with other optional attributes. */
  m3u8_attr_t attributes;
} ext_x_daterange_t;

/**
 * @struct media_t
 * @brief Represents a complete Media Playlist.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** If the playlist is started by EXTM3U tag. */
  bool is_signed;
  /** HLS protocol version number. */
  uint8_t version;
  /** If segments can be decoded without previous ones. */
  bool is_independent_segments;
  /** True if the next segment starts a discontinuity. */
  bool is_discontinuity;
  /** Media playlist type, e.g., VOD or EVENT. */
  char* type;
  /** True if the playlist contains only i-frames. */
  bool is_i_frames_only;
  /** Maximum segment duration in seconds. */
  int target_duration;
  /** Sequence number of the first media segment. */
  int media_sequence;
  /** Sequence number for discontinuities. */
  int discontinuity_sequence;
  /** Segment duration and info. */
  ext_x_extinf_t* ext_x_extinf;
  /** Optional byte range for the current segment. */
  ext_x_byterange_t* ext_x_byterange;
  /** Encryption key info. */
  ext_x_key_t* ext_x_key;
  /** Initialization segment info. */
  ext_x_map_t* ext_x_map;
  /** ISO 8601 timestamp for the current segment. */
  char* program_date_time;
  /** Date range info. */
  ext_x_daterange_t* ext_x_daterange;
  /** True if the playlist ends with EXT-X-ENDLIST. */
  bool is_endlist;
} media_t;

/**
 * @brief Creates a new M3U8 media playlist object.
 * @param media Pointer to a @ref media_t* that will be populated.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
m3u8_media_status_t m3u8_media_create(media_t** media);

/**
 * @brief Destroys an M3U8 media playlist object.
 * @param media The media playlist object to destroy.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_ARG if @p media is NULL.
 */
m3u8_media_status_t m3u8_media_destroy(media_t* media);

/**
 * @brief Validates the integrity and conformance of a media playlist object.
 * @param media The media playlist object to validate.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR if the playlist is valid.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_PLAYLIST if validation fails.
 */
m3u8_media_status_t m3u8_playlist_validate(media_t* media);

/**
 * @brief Parses an M3U8 media playlist from a string.
 * @param media  A pointer to a valid @ref media_t object to be filled.
 * @param buffer The string buffer containing the media playlist content.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_PLAYLIST if parsing fails.
 */
m3u8_media_status_t m3u8_playlist_parse(media_t* media, char* buffer);

#endif  // _M3U8_MEDIA_H_
