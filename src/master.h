#ifndef _M3U8_MASTER_H_
#define _M3U8_MASTER_H_

#include <stdbool.h>
#include <stdint.h>

#include "list.h"

/**
 * @enum m3u8_manifest_status_t
 * @brief Represents the status of a master playlist operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_MANIFEST_STATUS_NO_ERROR,
  /** Invalid argument provided to a function. */
  M3U8_MANIFEST_STATUS_INVALID_ARG,
  /** Invalid value of an attribute given a tag. */
  M3U8_MANIFEST_STATUS_INVALID_VALUE,
  /** Memory allocation failed. */
  M3U8_MANIFEST_STATUS_MEM_ALLOC_ERROR,
  /** Invalid or malformed master playlist. */
  M3U8_MANIFEST_STATUS_INVALID_MANIFEST,
  /** Invalid attributes found. */
  M3U8_MANIFEST_STATUS_INVALID_ATTRS,
  /** An unknown or unspecified error occurred. */
  M3U8_MANIFEST_STATUS_UNKNOWN_ERROR,
} m3u8_manifest_status_t;

/**
 * @enum ext_x_media_type_t
 * @brief Represents the type of media in an EXT-X-MEDIA tag.
 */
typedef enum {
  /** Audio media type. */
  EXT_X_MEDIA_TYPE_AUDIO,
  /** Video media type. */
  EXT_X_MEDIA_TYPE_VIDEO,
  /** Subtitles media type. */
  EXT_X_MEDIA_TYPE_SUBTITLES,
  /** Closed-captions media type. */
  EXT_X_MEDIA_TYPE_CLOSED_CAPTIONS,
} ext_x_media_type_t;

/**
 * @struct ext_x_start_t
 * @brief Represents the EXT-X-START tag.
 */
typedef struct {
  /** The time offset in seconds. */
  double time_offset;
  /** If true, playback must start at the exact offset. */
  bool precise;
} ext_x_start_t;

/**
 * @struct ext_x_session_data_t
 * @brief Represents an EXT-X-SESSION-DATA tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Unique identifier for the session data. */
  char* data_id;
  /** Inline value, mutually exclusive with uri. */
  char* value;
  /** URI for external session data. */
  char* uri;
  /** ISO-639-1/2 language code. */
  char* language;
} ext_x_session_data_t;

/**
 * @struct ext_x_session_key_t
 * @brief Represents an EXT-X-SESSION-KEY tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** The encryption method (e.g., AES-128). */
  char* method;
  /** URI to the key file. */
  char* uri;
  /** The initialization vector. */
  uint8_t iv[16];
  /** The key format (e.g., "identity"). */
  char* key_format;
  /** Versions of the key format. */
  char* key_format_versions;
} ext_x_session_key_t;

/**
 * @struct ext_x_media_t
 * @brief Represents an EXT-X-MEDIA tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Media type: AUDIO, VIDEO, SUBTITLES, etc. */
  ext_x_media_type_t type;
  /** The group to which this rendition belongs. */
  char* group_id;
  /** Human-readable name for the rendition. */
  char* name;
  /** If true, this is the default rendition. */
  bool is_default;
  /** If true, this rendition can be autoselected. */
  bool is_autoselect;
  /** If true, this is a forced rendition. */
  bool is_forced;
  /** The language of the rendition (ISO 639). */
  char* language;
  /** An associated language. */
  char* assoc_language;
  /** Service ID for closed captions. */
  char* instream_id;
  /** Uniform Type Identifiers. */
  char* characteristics;
  /** Audio channels information. */
  char* channels;
  /** URI for the alternative media playlist. */
  char* uri;
} ext_x_media_t;

/**
 * @struct ext_x_stream_inf_t
 * @brief Represents an EXT-X-STREAM-INF tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Peak segment bit rate. */
  uint32_t bandwidth;
  /** Average segment bit rate. */
  uint32_t average_bandwidth;
  /** A string specifying the stream's codecs. */
  char* codecs;
  /** The optimal screen resolution. */
  char* resolution;
  /** The stream's frame rate. */
  double frame_rate;
  /** The HDCP level: "TYPE-0" or "NONE". */
  char* hdcp_level;
  /** The audio group ID. */
  char* audio;
  /** The video group ID. */
  char* video;
  /** The subtitles group ID. */
  char* subtitles;
  /** The closed captions group ID. */
  char* closed_captions;
  /** The URI of the media playlist. */
  char* uri;
} ext_x_stream_inf_t;

/**
 * @struct ext_x_i_frame_stream_inf_t
 * @brief Represents an EXT-X-I-FRAME-STREAM-INF tag.
 */
typedef struct {
  /** Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /** Peak segment bit rate. */
  uint32_t bandwidth;
  /** Average segment bit rate. */
  uint32_t average_bandwidth;
  /** A string specifying the stream's codecs. */
  char* codecs;
  /** The optimal screen resolution. */
  char* resolution;
  /** The HDCP level: "TYPE-0" or "NONE". */
  char* hdcp_level;
  /** The video group ID. */
  char* video;
  /** The URI of the I-frame media playlist. */
  char* uri;
} ext_x_i_frame_stream_inf_t;

/**
 * @struct master_t
 * @brief Represents a complete M3U8 master playlist.
 */
typedef struct {
  /** If the playlist starts with EXTM3U. */
  bool is_signed;
  /** HLS protocol version number. */
  uint8_t version;
  /** If true, segments are independent. */
  bool is_independent_segments;
  /** Optional EXT-X-START tag. */
  ext_x_start_t* ext_x_start;
  /** List of x-session-data tags. */
  ext_x_session_data_t* ext_x_session_data;
  /** List of SESSION-KEY tags. */
  ext_x_session_key_t* ext_x_session_key;
  /** List of X-MEDIA tags. */
  ext_x_media_t* ext_x_media;
  /** List of X-STREAM-INF tags. */
  ext_x_stream_inf_t* ext_x_stream_inf;
  /** List of i-frame tags. */
  ext_x_i_frame_stream_inf_t* ext_x_i_frame_stream_inf;
} master_t;

/**
 * @brief Creates a new M3U8 master playlist object.
 * @param master A pointer to a @ref master_t* that will be populated.
 * @return @ref M3U8_MANIFEST_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MANIFEST_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
m3u8_manifest_status_t m3u8_manifest_create(master_t** master);

/**
 * @brief Destroys an M3U8 master playlist object.
 * @param master The master playlist object to destroy.
 * @return @ref M3U8_MANIFEST_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MANIFEST_STATUS_INVALID_ARG if @p master is NULL.
 */
m3u8_manifest_status_t m3u8_manifest_destroy(master_t* master);

/**
 * @brief Validates the integrity and conformance of a master playlist object.
 * @param master The master playlist object to validate.
 * @return @ref M3U8_MANIFEST_STATUS_NO_ERROR if the playlist is valid.
 * @return @ref M3U8_MANIFEST_STATUS_INVALID_MANIFEST if validation fails.
 */
m3u8_manifest_status_t m3u8_manifest_validate(master_t* master);

/**
 * @brief Parses an M3U8 master playlist from a string.
 * @param master A pointer to a valid @ref master_t object to be filled.
 * @param buffer The string buffer containing the master playlist content.
 * @return @ref M3U8_MANIFEST_STATUS_NO_ERROR on success.
 * @return @ref M3U8_MANIFEST_STATUS_INVALID_MANIFEST if parsing fails.
 */
m3u8_manifest_status_t m3u8_manifest_parse(master_t* master, char* buffer);

#endif  // _M3U8_MASTER_H_
