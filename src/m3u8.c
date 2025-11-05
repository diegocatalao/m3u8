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

  if (m3u8->master == NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument master (null)");
  }

  if (m3u8->media != NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument media (null)");
  }

  if (!m3u8_master_destroy(m3u8->master)) {
    M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy master");
  }

  if (!m3u8_media_destroy(m3u8->media)) {
    M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy media");
  }

  free(m3u8);

clean_up:
  return status;
}

m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* buffer) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_status_t m3u8_validate(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
