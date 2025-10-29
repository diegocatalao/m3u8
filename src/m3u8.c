#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "m3u8.h"
#include "manifest.h"
#include "playlist.h"

m3u8_status_t m3u8_create(m3u8_t** m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL) {
    RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 (null)");
  }

  if ((*m3u8 = (m3u8_t*)malloc(sizeof(m3u8_t))) == NULL) {
    RAISE(M3U8_STATUS_MEM_ALLOC_ERROR, "Failed to allocate m3u8");
  }

  memset(m3u8, 0, sizeof(m3u8_t));

  /** must be allocated before */
  (*m3u8)->manifest = NULL;
  (*m3u8)->playlist = NULL;

clean_up:
  return status;
}

m3u8_status_t m3u8_destroy(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL) {
    RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 (null)");
  }

  if (m3u8_manifest_destroy(m3u8->manifest) != M3U8_MANIFEST_STATUS_NO_ERROR) {
    RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy manifest");
  }

  if (m3u8_playlist_destroy(m3u8->playlist) != M3U8_PLAYLIST_STATUS_NO_ERROR) {
    RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to destroy playlist");
  }

  if (m3u8->manifest != NULL) {
    m3u8_manifest_destroy(m3u8->manifest);
  }

  if (m3u8->playlist != NULL) {
    m3u8_playlist_destroy(m3u8->playlist);
  }

  free(m3u8);

clean_up:
  return status;
}

m3u8_status_t m3u8_parse_from_str(m3u8_t* m3u8, const char* buffer) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;

  if (m3u8 == NULL || buffer == NULL) {
    RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8 or buffer (null)");
  }

  manifest_type_t type = m3u8_get_playlist_type(buffer);

  switch (type) {
    case MASTER_PLAYLIST:
      status = m3u8_manifest_parse(m3u8, buffer);
      break;
    case MEDIA_PLAYLIST:
      status = m3u8_playlist_parse(m3u8, buffer);
      break;
    case UNKNOWN_PLAYLIST:
      RAISE(M3U8_STATUS_INVALID_MANIFEST, "Invalid manifest type");
  }

clean_up:
  return status;
}

manifest_type_t m3u8_get_playlist_type(const char* buffer) {
  if (buffer == NULL) {
    return UNKNOWN_PLAYLIST;
  }

  const char* cursor = buffer;

  while (cursor && *cursor != '\0') {
    while (*cursor == '\r' || *cursor == '\n' || *cursor == '\t') {
      cursor++;
    }

    if (*cursor == '\0') {
      break;
    }

    if (strncmp(cursor, "#EXT-X-STREAM-INF:", 18) == 0) {
      /** https://datatracker.ietf.org/doc/html/rfc8216#section-4.3.4.2 */
      return MASTER_PLAYLIST;
    } else if (strncmp(cursor, "#EXT-X-TARGETDURATION:", 22) == 0) {
      /** https://datatracker.ietf.org/doc/html/rfc8216#section-4.3.3.1 */
      return MEDIA_PLAYLIST;
    } else if (strncmp(cursor, "#EXTINF:", 8) == 0) {
      /** https://datatracker.ietf.org/doc/html/rfc8216#section-4.3.2.1 */
      return MEDIA_PLAYLIST;
    } else {
      DEBUG("No markup tag was found in the buffer: %s", cursor);
    }

    if ((cursor = strchr(cursor, '\n')) != NULL) {
      cursor++;
    }
  }

  return UNKNOWN_PLAYLIST;
}
