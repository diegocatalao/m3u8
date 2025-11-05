#include "master.h"
#include "logger.h"

m3u8_master_status_t m3u8_master_create(master_t** master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
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
