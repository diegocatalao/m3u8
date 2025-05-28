/**
 * @file m3u8.h
 * @brief M3U8 playlist parser and metadata structures.
 */
#ifndef __H_M3U8__
#define __H_M3U8__

#include <stdbool.h>

#define M3U8_STATUS_NO_ERROR        0x00
#define M3U8_STATUS_INVALID_ARG     0x01
#define M3U8_STATUS_MEM_ALLOC_ERROR 0x02
#define M3U8_STATUS_INIT_CURL_ERROR 0x03
#define M3U8_STATUS_FILE_IO_ERROR   0x04
#define M3U8_STATUS_CURL_OP_ERROR   0x05
#define M3U8_STATUS_UNKNOWN_ERROR   0x99

/** @brief type of M3U8 playlist: media or master */
typedef enum {
  M3U8_TYPE_MEDIA, /**< media playlist */
  M3U8_TYPE_MASTER /**< master playlist */
} m3u8_type_e;

/** @brief playlist mode: live or video-on-demand */
typedef enum {
  LIVE, /**< live streaming */
  VOD   /**< video on demand */
} m3u8_playlist_type_e;

/** @brief supported ext-x-key methods */
typedef enum {
  NONE,      /**< no encryption */
  AES_128,   /**< aes-128 encryption */
  SAMPLE_AES /**< sample-aes encryption */
} m3u8_ext_key_method_e;

/** @brief media types defined in ext-x-media */
typedef enum { AUDIO, VIDEO, SUBTITLES, CLOSED_CAPTIONS } m3u8_media_type_e;

/** @brief represents an ext-x-map directive */
typedef struct {
  char* uri;        /**< uri of the initialization segment */
  char* byte_range; /**< optional byte range of the segment */
} ext_x_map_t;

/** @brief represents an ext-x-key directive */
typedef struct {
  char* method;              /**< encryption method */
  char* uri;                 /**< uri of the key */
  char  iv;                  /**< initialization vector */
  char* keyformat;           /**< key format */
  char* key_format_versions; /**< key format versions */
} ext_x_key;

/** @brief metadata for a media playlist */
typedef struct {
  int                  version;                 /**< playlist version */
  bool                 is_independent_segments; /**< independent segments flag */
  m3u8_playlist_type_e type;                    /**< playlist type (live or vod) */
  int                  target_duration;         /**< target duration in seconds */
  int                  media_sequence;          /**< media sequence number */
  ext_x_map_t*         map;                     /**< initialization segment map */
} m3u8_media_t;

/** @brief represents an ext-x-start directive */
typedef struct {
  double time_offset; /**< playback start offset */
  bool   precise;     /**< if the offset is precise */
} ext_x_start_t;

/** @brief represents an ext-x-define directive */
typedef struct {
  char* name;      /**< variable name */
  char* value;     /**< variable value */
  bool  is_import; /**< whether it's an import definition */
} ext_x_define_t;

/** @brief represents an ext-x-media tag (for alternate renditions) */
typedef struct {
  m3u8_media_type_e type;           /**< media type (audio, video, etc) */
  char*             group_id;       /**< group id */
  char*             language;       /**< language code (iso 639-1) */
  char*             name;           /**< media name */
  bool              is_autoselect;  /**< auto-select flag */
  bool              is_default;     /**< default rendition flag */
  char*             instream_id;    /**< for closed-captions */
  char*             assoc_language; /**< associated language */
  char*             channels;       /**< channel information (for audio) */
  bool              is_forced;      /**< forced subtitle flag */
  char*             uri;            /**< uri for the media */
} ext_x_media_type_t;

/** @brief represents an ext-x-stream-inf tag */
typedef struct _ext_x_stream_inf {
  struct _ext_x_stream_inf* __next; /**< next ext_x_stream_inf_t on this list */

  char*  audio;             /**< audio group id */
  char*  subtitles;         /**< subtitles group id */
  char*  closed_captions;   /**< closed captions group id */
  int    bandwidth;         /**< peak bandwidth */
  int    average_bandwidth; /**< average bandwidth */
  char*  resolution;        /**< resolution string (e.g., 1920x1080) */
  double frame_rate;        /**< frame rate */
  char*  codecs;            /**< codec string */
  char*  video;             /**< video group id (se aplicável) */
  char*  hdcp_level;        /**< HDCP level: "TYPE-0" or "NONE" */
  char*  uri;               /**< uri for the media playlist */
} ext_x_stream_inf_t;

/** @brief root structure for an m3u8 manifest */
typedef struct {
  bool isigned;

  int                 version;                 /**< playlist version */
  bool                is_independent_segments; /**< flag for independent segments */
  ext_x_stream_inf_t* x_stream_inf;            /**< stream information with segments */
} m3u8_t;

/**
 * @brief Allocates and initializes a new m3u8_t structure.
 *
 * @param m3u8_ptr double pointer to an m3u8_t. Must be NULL on input.
 *
 * @return M3U8_STATUS_NO_ERROR        on success;
 *         M3U8_STATUS_INVALID_ARG if  *m3u8_ptr is not NULL;
 *         M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure;
 *         M3U8_STATUS_UNKNOWN_ERROR   on unexpected failure.
 */
int m3u8_create(m3u8_t** m3u8_ptr);

/**
 * @brief Deallocates and cleans up a previously created m3u8_t structure.
 *
 * @param m3u8_ptr pointer to an m3u8_t structure to be deallocated.
 *
 * @return M3U8_STATUS_NO_ERROR       on success.
 *         M3U8_STATUS_INVALID_ARG if m3u8_ptr is NULL.
 */
int m3u8_destroy(m3u8_t* m3u8_ptr);

/**
 * @brief Fetches and parses an M3U8 playlist from a remote URI.
 *
 * @param uri        remote M3U8 URI (null-terminated string).
 * @param m3u8_ptr   pointer to a valid m3u8_t structure to be filled.
 *
 * @return M3U8_STATUS_NO_ERROR        on success.
 *         M3U8_STATUS_INVALID_ARG     if uri or m3u8_ptr is NULL.
 *         M3U8_STATUS_INIT_CURL_ERROR if curl initialization or setup fails.
 *         M3U8_STATUS_CURL_OP_ERROR   if the download fails.
 *         M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 *         M3U8_STATUS_UNKNOWN_ERROR   on unexpected failure.
 */
int m3u8_open_from_remote(char* uri, m3u8_t* m3u8_ptr);

/**
 * @brief Displays parsed stream information from the M3U8 playlist.
 *
 * @param m3u8_ptr pointer to a parsed m3u8_t structure.
 *
 * @return M3U8_STATUS_NO_ERROR    on success.
 *         M3U8_STATUS_INVALID_ARG if m3u8_ptr is NULL.
 */
int m3u8_show_stream_info(m3u8_t* m3u8_ptr);

#endif  // __H_M3U8__
