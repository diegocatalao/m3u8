#ifndef _M3U8_MEDIA_H_
#define _M3U8_MEDIA_H_

#include "linked.h"
#include "parser.h"

/**
 * @enum m3u8_media_status_t
 * @brief Represents the status of a media playlist operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_MEDIA_STATUS_NO_ERROR = 0,
  /** Invalid argument provided to a function. */
  M3U8_MEDIA_STATUS_INVALID_ARG = 1,
  /** Memory allocation failed. */
  M3U8_MEDIA_STATUS_MEM_ALLOC_ERROR = 2,
  /** Invalid or malformed media playlist. */
  M3U8_MEDIA_STATUS_INVALID_PLAYLIST = 3,
  /** An unknown or unspecified error occurred. */
  M3U8_MEDIA_STATUS_UNKNOWN_ERROR = 99,
} m3u8_media_status_t;

/**
 * @struct m3u8_ext_x_extinf_t
 * @brief Represents an EXTINF tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** Duration of the media segment in seconds. */
  double duration;
  /** Optional title or description of the segment. */
  char* title;
  /** URI of the media segment. */
  char* uri;
} m3u8_ext_x_extinf_t;

/**
 * @struct m3u8_ext_x_byterange_t
 * @brief Represents an EXT-X-BYTERANGE tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** Length of the byte range in bytes. */
  int length;
  /** Optional start offset in bytes. */
  int offset;
} m3u8_ext_x_byterange_t;

/**
 * @struct m3u8_ext_x_key_t
 * @brief Represents an EXT-X-KEY tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** Cryptography method. */
  char* method;
  /** Link to remote key resource. */
  char* uri;
  /** Initialization vector in hex. */
  int iv[16];
  /** Key format, default is "identity". */
  char* key_format;
  /** Key format versions, default is 1. */
  char* key_format_versions;
} m3u8_ext_x_key_t;

/**
 * @struct m3u8_ext_x_map_t
 * @brief Represents an EXT-X-MAP tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** URI to the initialization segment. */
  char* uri;
  /** Optional byte range. */
  char* byterange;
} m3u8_ext_x_map_t;

/**
 * @struct m3u8_ext_x_daterange_t
 * @brief Represents an EXT-X-DATERANGE tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** Unique identifier for the date range. */
  char* id;
  /**
   * Optional class attribute.
   *
   * NOTE: The actual name of this field is 'class' but for compatibility
   * purposes with cpp it was changed to klass.
   */
  char* klass;
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
  m3u8_parser_attr_t attributes;
} m3u8_ext_x_daterange_t;

/**
 * @struct m3u8_media_t
 * @brief Represents a complete Media Playlist.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_linked_node_t list;
  /** If the playlist is started by EXTM3U tag. */
  bool is_signed;
  /** HLS protocol version number. */
  int version;
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
  m3u8_ext_x_extinf_t* ext_x_extinf;
  /** Optional byte range for the current segment. */
  m3u8_ext_x_byterange_t* ext_x_byterange;
  /** Encryption key info. */
  m3u8_ext_x_key_t* ext_x_key;
  /** Initialization segment info. */
  m3u8_ext_x_map_t* ext_x_map;
  /** ISO 8601 timestamp for the current segment. */
  char* program_date_time;
  /** Date range info. */
  m3u8_ext_x_daterange_t* ext_x_daterange;
  /** True if the playlist ends with EXT-X-ENDLIST. */
  bool is_endlist;
} m3u8_media_t;

/**
 * @brief Creates a new M3U8 media playlist object.
 * @param media Pointer to a @ref m3u8_media_t* that will be populated.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
m3u8_media_status_t m3u8_media_create(m3u8_media_t** media);

/**
 * @brief Destroys an M3U8 media playlist object.
 * @param media The media playlist object to destroy.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_ARG if @p media is NULL.
 */
m3u8_media_status_t m3u8_media_destroy(m3u8_media_t* media);

/**
 * @brief Parses an M3U8 media playlist from a string buffer.
 * @param[out] media  A pointer to a @ref m3u8_media_t structure that will be
 *                    populated with the parsed data.
 * @param[in]  buffer The input string buffer containing the M3U8 media playlist.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_ARG if @p media or @p buffer are NULL.
 * @return @ref M3U8_MEDIA_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_PLAYLIST if the playlist is malformed.
 */
m3u8_media_status_t m3u8_media_parser_from_str(m3u8_media_t* media,
                                               const char*   buffer);

/**
 * @brief Validates the integrity and conformance of a media playlist object.
 * @param media The media playlist object to validate.
 * @return @ref M3U8_MEDIA_STATUS_NO_ERROR if the playlist is valid.
 * @return @ref M3U8_MEDIA_STATUS_INVALID_PLAYLIST if validation fails.
 */
m3u8_media_status_t m3u8_media_validate(m3u8_media_t* media);

#endif  // _M3U8_MEDIA_H_
