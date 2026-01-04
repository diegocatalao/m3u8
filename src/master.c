#include "master.h"
#include "logger.h"
#include "memory.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>

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

  if (master == NULL || buffer == NULL) {
    M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ARG, "Invalid master or buffer (null)");
  }

  char* start = buffer;
  char* end = NULL;

  while ((end = strchr(start, '\n'))) {
    char*          line = NULL;
    size_t         length = end - start;
    m3u8_parser_t* parsed = NULL;  // the current parseed line

    if (length == 0 || (length == 1 && start[0] == '\r')) {
      start = end + 1;
      continue;
    }

    line = (char*)malloc(length + 1);

    if (line == NULL) {
      M3U8_RAISE(M3U8_MASTER_STATUS_MEM_ALLOC_ERROR, "Fail to allocate line");
    }

    memcpy(line, start, length);
    line[length] = '\0';

    if (length > 0 && line[length - 1] == '\r') {
      line[length - 1] = '\0';
    }

    if (m3u8_parser_from_str(line, &parsed) != M3U8_PARSER_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Fail to parse current string");
    }

    switch (parsed->tag) {
      case M3U8_PARSER_EXTM3U:
        master->is_signed = true;
        break;
      case M3U8_PARSER_EXT_X_VERSION:
        master->version = (uint8_t)atoi(parsed->value);
        break;
      case M3U8_PARSER_EXT_X_INDEPENDENT_SEGMENTS:
        master->is_independent_segments = true;
        break;
      case M3U8_PARSER_EXT_X_START:
        m3u8_parser_attr_t* entry = NULL;
        m3u8_parser_attr_t* head = parsed->attrs;
        m3u8_ext_x_start_t* ext_x_start = NULL;

        if (master->ext_x_start != NULL) {
          RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "EXT-X-START already defined");
        }

        m3u8_memory_check_alloc(ext_x_start, m3u8_ext_x_start_t, sizeof(m3u8_ext_x_start_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "TIME-OFFSET") == 0) {
            ext_x_start->time_offset = strtod(entry->value, NULL);
          } else if (strcmp(entry->key, "PRECISE") == 0) {
            ext_x_start->precise = (strcmp(entry->value, "YES") == 0);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Unknown attribute in EXT-X-START");
          }
        }

        master->ext_x_start = ext_x_start;
        break;
      case M3U8_PARSER_EXT_X_SESSION_DATA:
        m3u8_parser_attr_t* entry = NULL;
        m3u8_parser_attr_t* head = parsed->attrs;

        m3u8_ext_x_session_data_t* ext_x_session_data = NULL;
        m3u8_memory_check_alloc(ext_x_session_data, m3u8_ext_x_session_data_t,
                                sizeof(m3u8_ext_x_session_data_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "DATA-ID") == 0) {
            ext_x_session_data->data_id = strdup(entry->value);
          } else if (strcmp(entry->key, "VALUE") == 0) {
            ext_x_session_data->value = strdup(entry->value);
          } else if (strcmp(entry->key, "URI") == 0) {
            ext_x_session_data->uri = strdup(entry->value);
          } else if (strcmp(entry->key, "LANGUAGE") == 0) {
            ext_x_session_data->language = strdup(entry->value);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Unknown attribute in EXT-X-SESSION-DATA");
          }
        }

        if (master->ext_x_session_data == NULL) {
          if (m3u8_linked_init(&ext_x_session_data->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to init ext_x_session_data list");
          }

          master->ext_x_session_data = ext_x_session_data;
        } else {
          if (m3u8_linked_inb(&master->ext_x_session_data->list, &ext_x_session_data->list) !=
              M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to append ext_x_session_data");
          }
        }

        break;
      case M3U8_PARSER_EXT_X_SESSION_KEY:
        m3u8_parser_attr_t*       entry = NULL;
        m3u8_parser_attr_t*       head = parsed->attrs;
        m3u8_ext_x_session_key_t* m3u8_ext_x_session_key = NULL;

        m3u8_memory_check_alloc(m3u8_ext_x_session_key, m3u8_ext_x_session_key_t,
                                sizeof(m3u8_ext_x_session_key_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "METHOD") == 0) {
            m3u8_ext_x_session_key->method = strdup(entry->value);
          } else if (strcmp(entry->key, "URI") == 0) {
            m3u8_ext_x_session_key->uri = strdup(entry->value);
          } else if (strcmp(entry->key, "KEYFORMAT") == 0) {
            m3u8_ext_x_session_key->key_format = strdup(entry->value);
          } else if (strcmp(entry->key, "KEYFORMATVERSIONS") == 0) {
            m3u8_ext_x_session_key->key_format_versions = strdup(entry->value);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Unknown attribute in EXT-X-SESSION-KEY");
          }
        }

        if (master->ext_x_session_key == NULL) {
          if (m3u8_linked_init(&m3u8_ext_x_session_key->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to init ext_x_session_key list");
          }

          master->ext_x_session_key = m3u8_ext_x_session_key;
        } else {
          if (m3u8_linked_inb(&master->ext_x_session_key->list, &m3u8_ext_x_session_key->list) !=
              M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to append ext_x_session_key");
          }
        }

        break;
      case M3U8_PARSER_EXT_X_MEDIA:
        m3u8_parser_attr_t* entry = NULL;
        m3u8_parser_attr_t* head = parsed->attrs;

        m3u8_ext_x_media_t* ext_x_media = NULL;
        m3u8_memory_check_alloc(ext_x_media, m3u8_ext_x_media_t, sizeof(m3u8_ext_x_media_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "TYPE") == 0) {
            if (strcmp(entry->value, "AUDIO") == 0) {
              ext_x_media->type = M3U8_EXT_X_MEDIA_TYPE_AUDIO;
            } else if (strcmp(entry->value, "VIDEO") == 0) {
              ext_x_media->type = M3U8_EXT_X_MEDIA_TYPE_VIDEO;
            } else if (strcmp(entry->value, "SUBTITLES") == 0) {
              ext_x_media->type = M3U8_EXT_X_MEDIA_TYPE_SUBTITLES;
            } else if (strcmp(entry->value, "CLOSED-CAPTIONS") == 0) {
              ext_x_media->type = M3U8_EXT_X_MEDIA_TYPE_CLOSED_CAPTIONS;
            } else {
              M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Invalid TYPE in EXT-X-MEDIA");
            }
          } else if (strcmp(entry->key, "GROUP-ID") == 0) {
            ext_x_media->group_id = strdup(entry->value);
          } else if (strcmp(entry->key, "NAME") == 0) {
            ext_x_media->name = strdup(entry->value);
          } else if (strcmp(entry->key, "DEFAULT") == 0) {
            ext_x_media->is_default = (strcmp(entry->value, "YES") == 0);
          } else if (strcmp(entry->key, "AUTOSELECT") == 0) {
            ext_x_media->is_autoselect = (strcmp(entry->value, "YES") == 0);
          } else if (strcmp(entry->key, "FORCED") == 0) {
            ext_x_media->is_forced = (strcmp(entry->value, "YES") == 0);
          } else if (strcmp(entry->key, "LANGUAGE") == 0) {
            ext_x_media->language = strdup(entry->value);
          } else if (strcmp(entry->key, "ASSOC-LANGUAGE") == 0) {
            ext_x_media->assoc_language = strdup(entry->value);
          } else if (strcmp(entry->key, "INSTREAM-ID") == 0) {
            ext_x_media->instream_id = strdup(entry->value);
          } else if (strcmp(entry->key, "CHARACTERISTICS") == 0) {
            ext_x_media->characteristics = strdup(entry->value);
          } else if (strcmp(entry->key, "CHANNELS") == 0) {
            ext_x_media->channels = strdup(entry->value);
          } else if (strcmp(entry->key, "URI") == 0) {
            ext_x_media->uri = strdup(entry->value);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Unknown attribute in EXT-X-MEDIA");
          }
        }

        if (master->ext_x_media == NULL) {
          if (m3u8_linked_init(&ext_x_media->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to init ext_x_media list");
          }

          master->ext_x_media = ext_x_media;
        } else {
          if (m3u8_linked_inb(&master->ext_x_media->list, &ext_x_media->list) !=
              M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to append ext_x_media");
          }
        }

        break;
      case M3U8_PARSER_EXT_X_STREAM_INF:
        m3u8_parser_attr_t* entry = NULL;
        m3u8_parser_attr_t* head = parsed->attrs;

        m3u8_ext_x_stream_inf_t* ext_x_stream_inf = NULL;
        m3u8_memory_check_alloc(ext_x_stream_inf, m3u8_ext_x_stream_inf_t,
                                sizeof(m3u8_ext_x_stream_inf_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "BANDWIDTH") == 0) {
            ext_x_stream_inf->bandwidth = (uint32_t)atoi(entry->value);
          } else if (strcmp(entry->key, "AVERAGE-BANDWIDTH") == 0) {
            ext_x_stream_inf->average_bandwidth = (uint32_t)atoi(entry->value);
          } else if (strcmp(entry->key, "CODECS") == 0) {
            ext_x_stream_inf->codecs = strdup(entry->value);
          } else if (strcmp(entry->key, "RESOLUTION") == 0) {
            ext_x_stream_inf->resolution = strdup(entry->value);
          } else if (strcmp(entry->key, "FRAME-RATE") == 0) {
            ext_x_stream_inf->frame_rate = strtod(entry->value, NULL);
          } else if (strcmp(entry->key, "HDCP-LEVEL") == 0) {
            if (strcmp(entry->value, "TYPE-0") == 0) {
              ext_x_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_TYPE_0;
            } else if (strcmp(entry->value, "TYPE-1") == 0) {
              ext_x_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_TYPE_1;
            } else if (strcmp(entry->value, "NONE") == 0) {
              ext_x_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_NONE;
            } else {
              M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS,
                         "Invalid HDCP-LEVEL in EXT-X-STREAM-INF");
            }
          } else if (strcmp(entry->key, "AUDIO") == 0) {
            ext_x_stream_inf->audio = strdup(entry->value);
          } else if (strcmp(entry->key, "VIDEO") == 0) {
            ext_x_stream_inf->video = strdup(entry->value);
          } else if (strcmp(entry->key, "SUBTITLES") == 0) {
            ext_x_stream_inf->subtitles = strdup(entry->value);
          } else if (strcmp(entry->key, "CLOSED-CAPTIONS") == 0) {
            ext_x_stream_inf->closed_captions = strdup(entry->value);
          } else if (strcmp(entry->key, "URI") == 0) {
            ext_x_stream_inf->uri = strdup(entry->value);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Unknown attribute in EXT-X-STREAM-INF");
          }
        }

        if (master->ext_x_stream_inf == NULL) {
          if (m3u8_linked_init(&ext_x_stream_inf->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to init ext_x_stream_inf list");
          }

          master->ext_x_stream_inf = ext_x_stream_inf;
        } else {
          if (m3u8_linked_inb(&master->ext_x_stream_inf->list, &ext_x_stream_inf->list) !=
              M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to append ext_x_stream_inf");
          }
        }

        /**
         * The URI line that follows the EXT-X-STREAM-INF tag specifies a Media Playlist that
         * carries a Rendition of the Variant Stream.  The URI line is REQUIRED. Clients that do
         * not support multiple video Renditions SHOULD play this Rendition.
         *
         * Its format is:
         * #EXT-X-STREAM-INF:<attribute-list>
         * <URI>
         *
         * https://datatracker.ietf.org/doc/html/rfc8216#section-4.3.4.2
         */
        start = end + 1;
        end = strchr(start, '\n');
        size_t uri_length = end - start;

        if (end == NULL) {
          M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Expected URI after EXT-X-STREAM-INF");
        }

        if (uri_length == 0 || (uri_length == 1 && start[0] == '\r')) {
          M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS, "Empty URI after EXT-X-STREAM-INF");
        }

        char* temp = (char*)realloc(line, uri_length + 1);
        if (temp == NULL) {
          M3U8_RAISE(M3U8_MASTER_STATUS_MEM_ALLOC_ERROR, "Fail to reallocate line for URI");
        }
        line = temp;

        memcpy(line, start, uri_length);
        line[uri_length] = '\0';

        if (uri_length > 0 && line[uri_length - 1] == '\r') {
          line[uri_length - 1] = '\0';
        }

        ext_x_stream_inf->uri = strdup(line);

        break;
      case M3U8_PARSER_EXT_X_I_FRAME_STREAM_INF:
        m3u8_parser_attr_t* entry = NULL;
        m3u8_parser_attr_t* head = parsed->attrs;

        m3u8_ext_x_i_frame_stream_inf_t* ext_x_i_frame_stream_inf = NULL;
        m3u8_memory_check_alloc(ext_x_i_frame_stream_inf, m3u8_ext_x_i_frame_stream_inf_t,
                                sizeof(m3u8_ext_x_i_frame_stream_inf_t));

        m3u8_linked_foreach(entry, &head->list, m3u8_parser_attr_t, list) {
          if (strcmp(entry->key, "BANDWIDTH") == 0) {
            ext_x_i_frame_stream_inf->bandwidth = (uint32_t)atoi(entry->value);
          } else if (strcmp(entry->key, "CODECS") == 0) {
            ext_x_i_frame_stream_inf->codecs = strdup(entry->value);
          } else if (strcmp(entry->key, "RESOLUTION") == 0) {
            ext_x_i_frame_stream_inf->resolution = strdup(entry->value);
          } else if (strcmp(entry->key, "HDCP-LEVEL") == 0) {
            if (strcmp(entry->value, "TYPE-0") == 0) {
              ext_x_i_frame_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_TYPE_0;
            } else if (strcmp(entry->value, "TYPE-1") == 0) {
              ext_x_i_frame_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_TYPE_1;
            } else if (strcmp(entry->value, "NONE") == 0) {
              ext_x_i_frame_stream_inf->hdcp_level = M3U8_HDCP_LEVEL_NONE;
            } else {
              M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS,
                         "Invalid HDCP-LEVEL in EXT-X-I-FRAME-STREAM-INF");
            }
          } else if (strcmp(entry->key, "VIDEO") == 0) {
            ext_x_i_frame_stream_inf->video = strdup(entry->value);
          } else if (strcmp(entry->key, "URI") == 0) {
            ext_x_i_frame_stream_inf->uri = strdup(entry->value);
          } else {
            M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_ATTRS,
                       "Unknown attribute in EXT-X-I-FRAME-STREAM-INF");
          }
        }

        if (master->ext_x_i_frame_stream_inf == NULL) {
          if (m3u8_linked_init(&ext_x_i_frame_stream_inf->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR,
                       "Fail to init ext_x_i_frame_stream_inf list");
          }

          master->ext_x_i_frame_stream_inf = ext_x_i_frame_stream_inf;
        } else {
          if (m3u8_linked_inb(&master->ext_x_i_frame_stream_inf->list,
                              &ext_x_i_frame_stream_inf->list) != M3U8_LINKED_STATUS_NO_ERROR) {
            M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Fail to append ext_x_i_frame_stream_inf");
          }
        }

        break;
      case M3U8_PARSER_EXT_UNKNOWN:
      default:
        M3U8_RAISE(M3U8_MASTER_STATUS_INVALID_TAG, "Unknown tag found in master playlist");
        break;
    }

    m3u8_memory_check_dealloc(line);
    start = end + 1;
  }

clean_up:
  if (status != M3U8_MASTER_STATUS_NO_ERROR) {
    m3u8_master_destroy(master);
  }

  return status;
}

m3u8_master_status_t m3u8_master_validate(m3u8_master_t* master) {
  m3u8_master_status_t status = M3U8_MASTER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_MASTER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}
