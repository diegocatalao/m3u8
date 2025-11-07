#include "media.h"
#include "logger.h"

m3u8_media_status_t m3u8_media_create(m3u8_media_t** media) {
  m3u8_media_status_t status = M3U8_MEDIA_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MEDIA_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_media_status_t m3u8_media_destroy(m3u8_media_t* media) {
  m3u8_media_status_t status = M3U8_MEDIA_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MEDIA_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_media_status_t m3u8_media_parser_from_str(m3u8_media_t* media, const char* buffer) {
  m3u8_media_status_t status = M3U8_MEDIA_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MEDIA_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_media_status_t m3u8_media_validate(m3u8_media_t* media) {
  m3u8_media_status_t status = M3U8_MEDIA_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MEDIA_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
