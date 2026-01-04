#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "m3u8.h"
#include "master.h"
#include "media.h"
#include "parser.h"
#include "validate.h"

/**
 * @brief Parses a master playlist from a string manifest.
 * @param master   Pointer to a valid @ref m3u8_master_t structure to be filled.
 * @param manifest String containing the master playlist content.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p master or @p manifest are NULL.
 * @return @ref M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
static m3u8_status_t m3u8_parse_master_from_str(m3u8_master_t* master, const char* manifest) {
  return M3U8_STATUS_NO_ERROR;
}

/**
 * @brief Parses a media playlist from a string manifest.
 * @param media    Pointer to a valid @ref m3u8_media_t structure to be filled.
 * @param manifest String containing the media playlist content.
 * @return @ref M3U8_STATUS_NO_ERROR on success.
 * @return @ref M3U8_STATUS_INVALID_ARG if @p media or @p manifest are NULL.
 * @return @ref M3U8_STATUS_MEM_ALLOC_ERROR on memory allocation failure.
 */
static m3u8_status_t m3u8_parse_media_from_str(m3u8_media_t* media, const char* manifest) {
  return M3U8_STATUS_NO_ERROR;
}

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
  m3u8_status_t               status = M3U8_STATUS_NO_ERROR;
  m3u8_parser_playlist_type_t type = m3u8_parser_playlist_type(manifest);

  if (type == M3U8_PARSER_MASTER_PLAYLIST) {
    if (m3u8->master != NULL) {
      M3U8_RAISE(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8->master (not null)");
    }

    if (m3u8_parse_master_from_str(m3u8->master, manifest) != M3U8_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to parse master playlist");
    }
  } else if (type == M3U8_PARSER_MEDIA_PLAYLIST) {
    if (m3u8->media == NULL) {
      if (m3u8_media_create(&m3u8->media) != M3U8_MEDIA_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to create media playlist");
      }

      if (m3u8_linked_init(&m3u8->media->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to initialize media playlist");
      }

      if (m3u8_media_parser_from_str(m3u8->media, manifest) != M3U8_MEDIA_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to parse media playlist");
      }
    } else {
      m3u8_media_t* media = NULL;

      if (m3u8_media_create(&media) != M3U8_MEDIA_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to create media playlist");
      }

      if (m3u8_media_parser_from_str(media, manifest) != M3U8_MEDIA_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to parse media playlist");
      }

      if (m3u8_linked_ina(&m3u8->media->list, &media->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Failed to insert media playlist");
      }

      /** reset the list and destroy the container of memory */
      /** of course, ignoring the errors from this operations for now */
      m3u8_linked_init(&media->list);
      m3u8_media_destroy(media);
    }
  } else {
    M3U8_RAISE(M3U8_STATUS_INVALID_MANIFEST, "Received an unknown manifest");
  }

clean_up:
  return status;
}

m3u8_status_t m3u8_validate(m3u8_t* m3u8) {
  m3u8_status_t status = M3U8_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
