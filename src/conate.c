/**
 * @file conate.c
 * @brief Time utility functions for retrieving and formatting timestamps.
 */

#include "conate.h"

#include <sys/time.h>

conate_status_t conate_timenow(long* obuff) {
  conate_status_t status = M3U8_CONATE_NO_ERROR;
  struct timeval  tv;

  if (obuff == NULL) {
    M3U8_RAISE(M3U8_CONATE_INVALID_POINTER, "Invalid argument obuff (null)");
  }

  if (gettimeofday(&tv, NULL) != 0) {
    M3U8_RAISE(M3U8_CONATE_TIME_ERROR, "Invalid input type format");
  }

  if (tv.tv_sec < 0) {
    M3U8_RAISE(M3U8_CONATE_TIME_ERROR, "Invalid input type format");
  }

  *obuff = tv.tv_sec;

clean_up:
  return status;
}

conate_status_t conate_timefmt(long* tms, char* obuff, int size,
                               const char* fmt) {
  conate_status_t status = M3U8_CONATE_NO_ERROR;
  struct tm*      tm_info;
  time_t          timestamp = (time_t)(*tms);

  tm_info = localtime(&timestamp);

  if (strftime(obuff, size, fmt, tm_info) == 0) {
    M3U8_RAISE(M3U8_CONATE_TIME_ERROR, "Invalid input type format");
  }

clean_up:
  return status;
}

conate_status_t conate_timefmt_utc(long* tms, char* obuff, int size,
                                   const char* fmt) {
  conate_status_t status = M3U8_CONATE_NO_ERROR;
  struct tm*      tm_info;
  time_t          timestamp = (time_t)(*tms);

  tm_info = gmtime(&timestamp);

  if (strftime(obuff, size, fmt, tm_info) == 0) {
    M3U8_RAISE(M3U8_CONATE_TIME_ERROR, "Invalid input type format");
  }

clean_up:
  return status;
}
