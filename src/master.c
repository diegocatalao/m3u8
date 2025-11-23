#include "master.h"
#include "logger.h"
#include "memory.h"

#include <stdlib.h>

m3u8_master_status_t m3u8_master_create(m3u8_master_t** master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;

  if (master == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ARG, "Invalid arg master (null)");
  }

  if ((*master = (m3u8_master_t*)malloc(sizeof(m3u8_master_t))) == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_MEM_ALLOC_ERROR, "Failed to allocate m3u8_master_t");
  }

  memset(*master, 0, sizeof(m3u8_master_t));
  /** "version 1 to support interoperability with older clients" */
  /** https://datatracker.ietf.org/doc/html/rfc8216#section-4.3.1.2 */
  (*master)->version = 1;

clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_destroy(m3u8_master_t* master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;

  if (master == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ARG, "Invalid arg master (null)");
  }

  if (master->ext_x_start != NULL) {
    M3U8_CHECK_DEALLOC(master->ext_x_start);
  }

  if (master->ext_x_session_data != NULL) {
    m3u8_ext_x_session_data_t* entry = NULL;
    m3u8_ext_x_session_data_t* head = master->ext_x_session_data;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_data_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_session_data");
      }

      M3U8_CHECK_DEALLOC(entry->data_id);
      M3U8_CHECK_DEALLOC(entry->value);
      M3U8_CHECK_DEALLOC(entry->uri);
      M3U8_CHECK_DEALLOC(entry->language);
      M3U8_CHECK_DEALLOC(entry);
    }

    M3U8_CHECK_DEALLOC(master->ext_x_session_data);
  }

  if (master->ext_x_session_key != NULL) {
    m3u8_ext_x_session_key_t* entry = NULL;
    m3u8_ext_x_session_key_t* head = master->ext_x_session_key;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_key_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_session_key");
      }

      M3U8_CHECK_DEALLOC(entry->method);
      M3U8_CHECK_DEALLOC(entry->uri);
      M3U8_CHECK_DEALLOC(entry->key_format);
      M3U8_CHECK_DEALLOC(entry->key_format_versions);
      M3U8_CHECK_DEALLOC(entry);
    }

    M3U8_CHECK_DEALLOC(master->ext_x_session_key);
  }

  if (master->ext_x_media != NULL) {
    m3u8_ext_x_media_t* entry = NULL;
    m3u8_ext_x_media_t* head = master->ext_x_media;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_media_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_media");
      }

      M3U8_CHECK_DEALLOC(entry->group_id);
      M3U8_CHECK_DEALLOC(entry->name);
      M3U8_CHECK_DEALLOC(entry->language);
      M3U8_CHECK_DEALLOC(entry->assoc_language);
      M3U8_CHECK_DEALLOC(entry->instream_id);
      M3U8_CHECK_DEALLOC(entry->characteristics);
      M3U8_CHECK_DEALLOC(entry->channels);
      M3U8_CHECK_DEALLOC(entry->uri);
      M3U8_CHECK_DEALLOC(entry);
    }

    M3U8_CHECK_DEALLOC(master->ext_x_media);
  }

  if (master->ext_x_stream_inf != NULL) {
    m3u8_ext_x_stream_inf_t* entry = NULL;
    m3u8_ext_x_stream_inf_t* head = master->ext_x_stream_inf;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_stream_inf_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_stream_inf");
      }

      M3U8_CHECK_DEALLOC(entry->codecs);
      M3U8_CHECK_DEALLOC(entry->resolution);
      M3U8_CHECK_DEALLOC(entry->hdcp_level);
      M3U8_CHECK_DEALLOC(entry->audio);
      M3U8_CHECK_DEALLOC(entry->video);
      M3U8_CHECK_DEALLOC(entry->subtitles);
      M3U8_CHECK_DEALLOC(entry->closed_captions);
      M3U8_CHECK_DEALLOC(entry->uri);
      M3U8_CHECK_DEALLOC(entry);
    }

    M3U8_CHECK_DEALLOC(master->ext_x_stream_inf);
  }

  if (master->ext_x_i_frame_stream_inf != NULL) {
    m3u8_ext_x_i_frame_stream_inf_t* entry = NULL;
    m3u8_ext_x_i_frame_stream_inf_t* head = master->ext_x_i_frame_stream_inf;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_i_frame_stream_inf_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_i_frame_stream_inf");
      }

      M3U8_CHECK_DEALLOC(entry->codecs);
      M3U8_CHECK_DEALLOC(entry->resolution);
      M3U8_CHECK_DEALLOC(entry->hdcp_level);
      M3U8_CHECK_DEALLOC(entry->video);
      M3U8_CHECK_DEALLOC(entry->uri);
      M3U8_CHECK_DEALLOC(entry);
    }

    M3U8_CHECK_DEALLOC(master->ext_x_i_frame_stream_inf);
  }

  if (master != NULL) {
    M3U8_CHECK_DEALLOC(master);
  }

  master = NULL;
clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_parser_from_str(m3u8_master_t* master, const char* buffer) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_master_status_t m3u8_master_validate(m3u8_master_t* master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
