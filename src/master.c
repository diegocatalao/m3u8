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
    m3u8_memory_check_dealloc(master->ext_x_start);
  }

  if (master->ext_x_session_data != NULL) {
    m3u8_ext_x_session_data_t* entry = NULL;
    m3u8_ext_x_session_data_t* head = master->ext_x_session_data;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_data_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_session_data");
      }

      m3u8_memory_check_dealloc(entry->data_id);
      m3u8_memory_check_dealloc(entry->value);
      m3u8_memory_check_dealloc(entry->uri);
      m3u8_memory_check_dealloc(entry->language);
      m3u8_memory_check_dealloc(entry);
    }

    m3u8_memory_check_dealloc(master->ext_x_session_data);
  }

  if (master->ext_x_session_key != NULL) {
    m3u8_ext_x_session_key_t* entry = NULL;
    m3u8_ext_x_session_key_t* head = master->ext_x_session_key;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_key_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_session_key");
      }

      m3u8_memory_check_dealloc(entry->method);
      m3u8_memory_check_dealloc(entry->uri);
      m3u8_memory_check_dealloc(entry->key_format);
      m3u8_memory_check_dealloc(entry->key_format_versions);
      m3u8_memory_check_dealloc(entry);
    }

    m3u8_memory_check_dealloc(master->ext_x_session_key);
  }

  if (master->ext_x_media != NULL) {
    m3u8_ext_x_media_t* entry = NULL;
    m3u8_ext_x_media_t* head = master->ext_x_media;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_media_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_media");
      }

      m3u8_memory_check_dealloc(entry->group_id);
      m3u8_memory_check_dealloc(entry->name);
      m3u8_memory_check_dealloc(entry->language);
      m3u8_memory_check_dealloc(entry->assoc_language);
      m3u8_memory_check_dealloc(entry->instream_id);
      m3u8_memory_check_dealloc(entry->characteristics);
      m3u8_memory_check_dealloc(entry->channels);
      m3u8_memory_check_dealloc(entry->uri);
      m3u8_memory_check_dealloc(entry);
    }

    m3u8_memory_check_dealloc(master->ext_x_media);
  }

  if (master->ext_x_stream_inf != NULL) {
    m3u8_ext_x_stream_inf_t* entry = NULL;
    m3u8_ext_x_stream_inf_t* head = master->ext_x_stream_inf;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_stream_inf_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_stream_inf");
      }

      m3u8_memory_check_dealloc(entry->codecs);
      m3u8_memory_check_dealloc(entry->resolution);
      m3u8_memory_check_dealloc(entry->hdcp_level);
      m3u8_memory_check_dealloc(entry->audio);
      m3u8_memory_check_dealloc(entry->video);
      m3u8_memory_check_dealloc(entry->subtitles);
      m3u8_memory_check_dealloc(entry->closed_captions);
      m3u8_memory_check_dealloc(entry->uri);
      m3u8_memory_check_dealloc(entry);
    }

    m3u8_memory_check_dealloc(master->ext_x_stream_inf);
  }

  if (master->ext_x_i_frame_stream_inf != NULL) {
    m3u8_ext_x_i_frame_stream_inf_t* entry = NULL;
    m3u8_ext_x_i_frame_stream_inf_t* head = master->ext_x_i_frame_stream_inf;

    m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_i_frame_stream_inf_t, list) {
      if (m3u8_linked_remove(&entry->list) != M3U8_LINKED_STATUS_NO_ERROR) {
        M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to remove ext_x_i_frame_stream_inf");
      }

      m3u8_memory_check_dealloc(entry->codecs);
      m3u8_memory_check_dealloc(entry->resolution);
      m3u8_memory_check_dealloc(entry->hdcp_level);
      m3u8_memory_check_dealloc(entry->video);
      m3u8_memory_check_dealloc(entry->uri);
      m3u8_memory_check_dealloc(entry);
    }

    m3u8_memory_check_dealloc(master->ext_x_i_frame_stream_inf);
  }

  if (master != NULL) {
    m3u8_memory_check_dealloc(master);
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
