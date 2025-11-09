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

  if (*m3u8 != NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument *m3u8 (not null)");
  }

  if ((*m3u8 = (m3u8_t*)malloc(sizeof(m3u8_t))) == NULL) {
    M3U8_RAISE(M3U8_STATUS_MEM_ALLOC_ERROR, "Failed to allocate m3u8");
  }

  /** the master and media will be initialized with NULL values first */
  memset(*m3u8, 0, sizeof(m3u8_t));

clean_up:
  return status;
}

m3u8_status_t m3u8_destroy(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL) {
    M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 (null)");
  }

  if (m3u8->master != NULL) {
    /** destroy the content including data structures */
    if (m3u8_master_destroy(m3u8->master) != M3U8_MASTER_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy master");
    }
  }

  if (m3u8->media != NULL) {
    /** destroy the content including data structures and embedded list */
    if (m3u8_media_destroy(m3u8->media) != M3U8_MEDIA_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy media");
    }
  }

  /** release the memory from m3u8 struct */
  free(m3u8);

clean_up:
  return status;
}

m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* manifest) {
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
