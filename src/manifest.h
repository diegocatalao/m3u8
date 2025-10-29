#ifndef _M3U8_MANIFEST_H_
#define _M3U8_MANIFEST_H_

#include <stdbool.h>
#include <stdint.h>

#include "list.h"

typedef enum {
  /**< Operation completed successfully. */
  M3U8_MANIFEST_STATUS_NO_ERROR,
  /**< Invalid argument provided to a function. */
  M3U8_MANIFEST_STATUS_INVALID_ARG,
  /**  Invalid value of an attribute given a tag. */
  M3U8_MANIFEST_STATUS_INVALID_VALUE,
  /**< Memory allocation failed. */
  M3U8_MANIFEST_STATUS_MEM_ALLOC_ERROR,
  /**< Invalid or malformed manifest. */
  M3U8_MANIFEST_STATUS_INVALID_MANIFEST,
  /**< Invalid attributes found. */
  M3U8_MANIFEST_STATUS_INVALID_ATTRS,
  /**< An unknown or unspecified error occurred. */
  M3U8_MANIFEST_STATUS_UNKNOWN_ERROR,
} m3u8_manifest_status_t;

typedef enum {
  EXT_X_MEDIA_TYPE_AUDIO = 0,
  EXT_X_MEDIA_TYPE_VIDEO = 1,
  EXT_X_MEDIA_TYPE_SUBTITLES = 2,
  EXT_X_MEDIA_TYPE_CLOSED_CAPTIONS = 3,
} ext_x_media_type_t;

/**
 * @struct ext_x_start_t
 * @brief Represents the #EXT-X-START tag.
 * @details Specifies the preferred starting point for a playlist.
 */
typedef struct {
  /**< The time offset in seconds. */
  double time_offset;
  /**< If true, playback must start at the exact offset. */
  bool precise;
} ext_x_start_t;

/**
 * @struct ext_x_session_data_t
 * @brief Represents an #EXT-X-SESSION-DATA tag.
 * @details Allows arbitrary session data to be carried in a master playlist.
 */
typedef struct {
  /**< Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< Unique identifier for the session data. */
  char* data_id;
  /**< Inline value, mutually exclusive with uri. */
  char* value;
  /**< URI for external session data. */
  char* uri;
  /**< ISO-639-1/2 language code. */
  char* language;
} ext_x_session_data_t;

/**
 * @struct ext_x_session_key_t
 * @brief Represents an #EXT-X-SESSION-KEY tag.
 * @details Provides encryption keys that apply to all media segments.
 */
typedef struct {
  /**< Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< The encryption method (e.g., AES-128). */
  char* method;
  /**< URI to the key file. */
  char* uri;
  /**< The initialization vector. */
  uint8_t iv[16];
  /**< The key format (e.g., "identity"). */
  char* key_format;
  /**< Versions of the key format. */
  char* key_format_versions;
} ext_x_session_key_t;

/**
 * @struct ext_x_media_t
 * @brief Represents an #EXT-X-MEDIA tag.
 * @details Used to relate playlists with alternative renditions of the same content.
 */
typedef struct {
  /**< Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< Media type: AUDIO, VIDEO, SUBTITLES, etc. */
  ext_x_media_type_t type;
  /**< The group to which this rendition belongs. */
  char* group_id;
  /**< Human-readable name for the rendition. */
  char* name;
  /**< If true, this is the default rendition. */
  bool is_default;
  /**< If true, this rendition can be autoselected. */
  bool is_autoselect;
  /**< If true, this is a forced rendition. */
  bool is_forced;
  /**< The language of the rendition (ISO 639). */
  char* language;
  /**< An associated language. */
  char* assoc_language;
  /**< Service ID for closed captions. */
  char* instream_id;
  /**< Uniform Type Identifiers. */
  char* characteristics;
  /**< Audio channels information. */
  char* channels;
  /**< URI for the alternative media playlist. */
  char* uri;
} ext_x_media_t;

/**
 * @struct ext_x_stream_inf_t
 * @brief Represents an #EXT-X-STREAM-INF tag.
 * @details Specifies a variant stream, a set of renditions for playback.
 */
typedef struct {
  /**< Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< Peak segment bit rate. */
  uint32_t bandwidth;
  /**< Average segment bit rate. */
  uint32_t average_bandwidth;
  /**< A string specifying the stream's codecs. */
  char* codecs;
  /**< The optimal screen resolution. */
  char* resolution;
  /**< The stream's frame rate. */
  double frame_rate;
  /**< The HDCP level: "TYPE-0" or "NONE". */
  char* hdcp_level;
  /**< The audio group ID. */
  char* audio;
  /**< The video group ID. */
  char* video;
  /**< The subtitles group ID. */
  char* subtitles;
  /**< The closed captions group ID. */
  char* closed_captions;
  /**< The URI of the media playlist. */
  char* uri;
} ext_x_stream_inf_t;

/**
 * @struct ext_x_i_frame_stream_inf_t
 * @brief Represents an #EXT-X-I-FRAME-STREAM-INF tag.
 * @details Specifies a playlist with only I-frames for fast-forward/rewind.
 */
typedef struct {
  /**< Embedded node for circular linked list. */
  m3u8_list_node_t list;
  /**< Peak segment bit rate. */
  uint32_t bandwidth;
  /**< Average segment bit rate. */
  uint32_t average_bandwidth;
  /**< A string specifying the stream's codecs. */
  char* codecs;
  /**< The optimal screen resolution. */
  char* resolution;
  /**< The HDCP level: "TYPE-0" or "NONE". */
  char* hdcp_level;
  /**< The video group ID. */
  char* video;
  /**< The URI of the I-frame playlist. */
  char* uri;
} ext_x_i_frame_stream_inf_t;

/**
 * @struct manifest_t
 * @brief Represents a complete M3U8 master playlist.
 * @details Holds all parsed data from a master manifest.
 */
typedef struct {
  /**< the playlist starts with EXTM3U */
  bool is_signed;
  /**< HLS protocol version number. */
  uint8_t version;
  /**< If true, segments are independent. */
  bool is_independent_segments;
  /**< Optional EXT-X-START tag. */
  ext_x_start_t* ext_x_start;
  /**< List of x-session-data tags. */
  ext_x_session_data_t* ext_x_session_data;
  /**< List of SESSION-KEY tags. */
  ext_x_session_key_t* ext_x_session_key;
  /**< List of X-MEDIA tags. */
  ext_x_media_t* ext_x_media;
  /**< List of X-STREAM-INF tags. */
  ext_x_stream_inf_t* ext_x_stream_inf;
  /**< i-frame tags. */
  ext_x_i_frame_stream_inf_t* ext_x_i_frame_stream_inf;
} manifest_t;

/**
 * @brief Creates a new M3U8 manifest object.
 * @details Allocates and initializes a new @c manifest_t object. The caller is
 *          responsible for freeing the allocated memory by calling
 *          @c m3u8_manifest_destroy().
 * @param[out] manifest A pointer to a @c manifest_t* that will be populated
 *                      with the address of the newly created object.
 * @return One of the status codes from @ref m3u8_manifest_status_t.
 */
m3u8_manifest_status_t m3u8_manifest_create(manifest_t** manifest);

/**
 * @brief Destroys an M3U8 manifest object.
 * @details Frees all memory associated with a @c manifest_t object.
 * @param[in] manifest The manifest object to destroy.
 * @return One of the status codes from @ref m3u8_manifest_status_t.
 */
m3u8_manifest_status_t m3u8_manifest_destroy(manifest_t* manifest);

/**
 * @brief Validates the integrity and conformance of a manifest object.
 * @details Checks the manifest object against the HLS specification rules.
 * @param[in] manifest The manifest object to validate.
 * @return Returns @c M3U8_MANIFEST_STATUS_NO_ERROR if the manifest is valid.
 *         Otherwise, it returns a status code indicating the specific
 *         validation error, such as @c M3U8_MANIFEST_STATUS_INVALID_MANIFEST.
 */
m3u8_manifest_status_t m3u8_manifest_validate(manifest_t* manifest);

/**
 * @brief Parses an M3U8 master playlist from a string.
 * @details Fills a @c manifest_t object with data parsed from the provided
 *          buffer. The manifest object must be allocated prior to calling this
 *          function.
 * @param[in,out] manifest A pointer to a valid @c manifest_t object to be filled.
 * @param[in]     buffer   The string buffer containing the manifest content.
 * @return One of the status codes from @ref m3u8_manifest_status_t, with
 *         @c M3U8_MANIFEST_STATUS_NO_ERROR on success.
 */
m3u8_manifest_status_t m3u8_manifest_parse(manifest_t* manifest, char* buffer);

#endif  // _M3U8_MANIFEST_H_
