#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "m3u8.h"
#include "master.h"
#include "media.h"
#include "validate.h"

m3u8_status_t m3u8_create(m3u8_t** m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 (null)");
  }

  if ((*m3u8 = (m3u8_t*)malloc(sizeof(m3u8_t))) == NULL) {
    M3U8_RAISE(M3U8_STATUS_MEM_ALLOC_ERROR, "Failed to allocate m3u8");
  }

  memset(m3u8, 0, sizeof(m3u8_t));

  /** must be allocated before */
  (*m3u8)->master = NULL;
  (*m3u8)->media = NULL;

clean_up:
  return status;
}

m3u8_status_t m3u8_destroy(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 (null)");
  }

  if (m3u8_manifest_destroy(m3u8->master) != M3U8_MANIFEST_STATUS_NO_ERROR) {
    M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy master");
  }

  if (m3u8_media_destroy(m3u8->media) != M3U8_MEDIA_STATUS_NO_ERROR) {
    M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy media");
  }

  if (m3u8->master != NULL) {
    m3u8_manifest_destroy(m3u8->master);
  }

  if (m3u8->media != NULL) {
    m3u8_media_destroy(m3u8->media);
  }

  free(m3u8);

clean_up:
  return status;
}

m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* buffer) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL || buffer == NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG,
               "Invalid argument m3u8 or buffer (null)");
  }

  playlist_type_t type = m3u8_get_playlist_type(buffer);

  switch (type) {
    case M3U8_MASTER_PLAYLIST:
      status = m3u8_manifest_parse(m3u8, buffer);
      break;
    case M3U8_MEDIA_PLAYLIST:
      status = m3u8_playlist_parse(m3u8, buffer);
      break;
    case M3U8_UNKNOWN_PLAYLIST:
      M3U8_RAISE(M3U8_STATUS_INVALID_MASTER_PLAYLIST, "Invalid master type");
  }

clean_up:
  return status;
}

m3u8_status_t m3u8_validate(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
clean_up:
  return 0;
}

m3u8_status_t m3u8_validate_master_playlist(master_t* master) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
clean_up:
  return 0;
}

m3u8_status_t m3u8_validate_media_playlist(media_t* media) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
clean_up:
  return 0;
}