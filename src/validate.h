#ifndef _M3U8_VALIDATE_H_
#define _M3U8_VALIDATE_H_

#include "master.h"
#include "media.h"

/**
 * @enum m3u8_validate_status_t
 * @brief Represents the status of a validation operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_VALIDATE_STATUS_NO_ERROR,
  /** A validation rule was not met. */
  M3U8_VALIDATE_STATUS_FAILURE,
  /** Invalid argument provided to a function. */
  M3U8_VALIDATE_STATUS_INVALID_ARG,
  /** Memory allocation failed. */
  M3U8_VALIDATE_STATUS_MEM_ALLOC_ERROR,
  /** An unknown or unspecified error occurred. */
  M3U8_VALIDATE_STATUS_UNKNOWN_ERROR,
} m3u8_validate_status_t;

/**
 * @brief Validates the presence and placement of the EXTM3U tag.
 * @param master Pointer to the @ref m3u8_master_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_extm3u(m3u8_master_t* master);

/**
 * @brief Validates the EXT-X-VERSION tag.
 * @param master Pointer to the @ref m3u8_master_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_version(m3u8_master_t* master);

/**
 * @brief Validates the EXT-X-INDEPENDENT-SEGMENTS tag.
 * @param master Pointer to the @ref m3u8_master_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_independent_segments(
  m3u8_master_t* master);

/**
 * @brief Validates a single EXT-X-START object.
 * @param ext_x_start Pointer to the single @ref m3u8_ext_x_start_t object to
 *                    validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_start(
  m3u8_ext_x_start_t* ext_x_start);

/**
 * @brief Validates a single EXT-X-SESSION-DATA object.
 * @param session_data Pointer to the single @ref m3u8_ext_x_session_data_t object
 *                     to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_session_data(
  m3u8_ext_x_session_data_t* session_data);

/**
 * @brief Validates a single EXT-X-SESSION-KEY object.
 * @param session_key Pointer to the single @ref m3u8_ext_x_session_key_t object
 *                    to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_session_key(
  m3u8_ext_x_session_key_t* session_key);

/**
 * @brief Validates a single EXT-X-MEDIA object.
 * @param media Pointer to the single @ref m3u8_ext_x_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_media(m3u8_ext_x_media_t* media);

/**
 * @brief Validates a single EXT-X-STREAM-INF object.
 * @param stream_inf Pointer to the single @ref m3u8_ext_x_stream_inf_t object
 *                   to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_stream_inf(
  m3u8_ext_x_stream_inf_t* stream_inf);

/**
 * @brief Validates a single EXT-X-I-FRAME-STREAM-INF object.
 * @param i_frame_stream_inf Pointer to the single
 *                           @ref m3u8_ext_x_i_frame_stream_inf_t object.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_i_frame_stream_inf(
  m3u8_ext_x_i_frame_stream_inf_t* i_frame_stream_inf);

/**
 * @brief Validates the EXT-X-PLAYLIST-TYPE tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_playlist_type(m3u8_media_t* media);

/**
 * @brief Validates the EXT-X-I-FRAMES-ONLY tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_i_frames_only(m3u8_media_t* media);

/**
 * @brief Validates the EXT-X-TARGETDURATION tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_targetduration(m3u8_media_t* media);

/**
 * @brief Validates the EXT-X-MEDIA-SEQUENCE tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_media_sequence(m3u8_media_t* media);

/**
 * @brief Validates the EXT-X-DISCONTINUITY-SEQUENCE tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_discontinuity_sequence(
  m3u8_media_t* media);

/**
 * @brief Validates a single EXTINF object.
 * @param extinf Pointer to the single @ref m3u8_ext_x_extinf_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_extinf(m3u8_ext_x_extinf_t* extinf);

/**
 * @brief Validates a single EXT-X-BYTERANGE object.
 * @param byterange Pointer to the single @ref m3u8_ext_x_byterange_t object to
 *                  validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_byterange(
  m3u8_ext_x_byterange_t* byterange);

/**
 * @brief Validates the placement of EXT-X-DISCONTINUITY tags.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_discontinuity(m3u8_media_t* media);

/**
 * @brief Validates a single EXT-X-KEY object.
 * @param key Pointer to the single @ref m3u8_ext_x_key_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_key(m3u8_ext_x_key_t* key);

/**
 * @brief Validates a single EXT-X-MAP object.
 * @param map Pointer to the single @ref m3u8_ext_x_map_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_map(m3u8_ext_x_map_t* map);

/**
 * @brief Validates all EXT-X-PROGRAM-DATE-TIME tags.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_program_date_time(
  m3u8_media_t* media);

/**
 * @brief Validates a single EXT-X-DATERANGE object.
 * @param daterange Pointer to the single @ref m3u8_ext_x_daterange_t object to
 *                  validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 * @return @ref M3U8_VALIDATE_STATUS_FAILURE if validation fails.
 */
m3u8_validate_status_t m3u8_validate_ext_x_daterange(
  m3u8_ext_x_daterange_t* daterange);

/**
 * @brief Validates the EXT-X-ENDLIST tag.
 * @param media Pointer to the @ref m3u8_media_t object to validate.
 * @return @ref M3U8_VALIDATE_STATUS_NO_ERROR on success.
 */
m3u8_validate_status_t m3u8_validate_ext_x_endlist(m3u8_media_t* media);

#endif  // _M3U8_VALIDATE_H_
