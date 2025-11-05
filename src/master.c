#include "master.h"
#include "logger.h"

#include <stdlib.h>

  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;

  if (master == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ARG, "Invalid arg master (null)");
  }

  if ((*master = (m3u8_master_t*)malloc(sizeof(m3u8_master_t))) == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_MEM_ALLOC_ERROR,
               "Failed to allocate m3u8_master_t");
  }

  memset(*master, 0, sizeof(m3u8_master_t));

  /** set the initial value of this struct */
  (*master)->is_signed = false;
  (*master)->version = 1;

clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_destroy(master_t* master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_parser_from_str(master_t*   master,
                                                 const char* buffer) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_validate(master_t* master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
