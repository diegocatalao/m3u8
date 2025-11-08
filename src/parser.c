#include "parser.h"
#include "logger.h"

#include <regex.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct m3u8_parser_tag_entry_t
 * @brief Represents a single entry in the M3U8 tag map, associating a tag
 *        name with its enum identifier @ref m3u8_parser_tag_t.
 */
typedef struct {
  const char*       name; /** tag name from m3u8 specification */
  m3u8_parser_tag_t tag;  /** tag identifier from m3u8 specification */
} m3u8_parser_tag_entry_t;

static const m3u8_parser_tag_entry_t m3u8_parser_tag_map[] = {
  {"EXTM3U", M3U8_PARSER_EXTM3U},
  {"EXT-X-VERSION", M3U8_PARSER_EXT_X_VERSION},
  {"EXT-X-INDEPENDENT-SEGMENTS", M3U8_PARSER_EXT_X_INDEPENDENT_SEGMENTS},
  {"EXT_X_I_FRAME-STREAM_INF", M3U8_PARSER_EXT_X_I_FRAME_STREAM_INF},
  {"EXT-X-MEDIA", M3U8_PARSER_EXT_X_MEDIA},
  {"EXT-X-SESSION-DATA", M3U8_PARSER_EXT_X_SESSION_DATA},
  {"EXT-X-SESSION-KEY", M3U8_PARSER_EXT_X_SESSION_KEY},
  {"EXT-X-START", M3U8_PARSER_EXT_X_START},
  {"EXT-X-STREAM-INF", M3U8_PARSER_EXT_X_STREAM_INF},
  {"EXT-X-PLAYLIST-TYPE", M3U8_PARSER_EXT_X_PLAYLIST_TYPE},
  {"EXT-X-I-FRAMES-ONLY", M3U8_PARSER_EXT_X_I_FRAMES_ONLY},
  {"EXT-X-TARGETDURATION", M3U8_PARSER_EXT_X_TARGETDURATION},
  {"EXT-X-MEDIA-SEQUENCE", M3U8_PARSER_EXT_X_MEDIA_SEQUENCE},
  {"EXT-X-DISCONTINUITY_SEQUENCE", M3U8_PARSER_EXT_X_DISCONTINUITY_SEQUENCE},
  {"EXTINF", M3U8_PARSER_EXTINF},
  {"EXT-X-BYTERANGE", M3U8_PARSER_EXT_X_BYTERANGE},
  {"EXT-X-DISCONTINUITY", M3U8_PARSER_EXT_X_DISCONTINUITY},
  {"EXT-X-KEY", M3U8_PARSER_EXT_X_KEY},
  {"EXT-X-MAP", M3U8_PARSER_EXT_X_MAP},
  {"EXT-X-PROGRAM-DATE-TIME", M3U8_PARSER_EXT_X_PROGRAM_DATE_TIME},
  {"EXT-X-DATERANGE", M3U8_PARSER_EXT_X_DATERANGE},
  {"EXT-X-ENDLIST", M3U8_PARSER_EXT_X_ENDLIST},
  {"EXT-UNKNOWN", M3U8_PARSER_EXT_UNKNOWN},
};

m3u8_parser_status_t m3u8_parser_attr(char* line, m3u8_parser_attr_t* attrs) {
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;

  regex_t    regex;
  regmatch_t pmatch[3];  // 0: full match, 1: key, 2: value

  char* cursor = line;
  char* pattern = "([A-Z0-9_-]+)=(\"[^\"]*\"|[^,]+)";

  memset(&regex, 0, sizeof(regex_t));

  if (line == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg line (null)");
  }

  if (attrs == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (m3u8_linked_init(&attrs->list) != M3U8_LINKED_STATUS_NO_ERROR) {
    M3U8_RAISE(M3U8_PARSER_STATUS_LIST_ERROR, "Fail to initialize the attribute list");
  }

  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    M3U8_RAISE(M3U8_PARSER_STATUS_MEM_ALLOC_ERROR, "Bad regular expression");
  }

  while (regexec(&regex, cursor, 3, pmatch, 0) == 0) {
    m3u8_parser_attr_t* attr = malloc(sizeof(m3u8_parser_attr_t));

    if (attr == NULL) {
      M3U8_RAISE(M3U8_PARSER_STATUS_MEM_ALLOC_ERROR, "Unable to allocate attr");
    }

    memset(attr, 0, sizeof(m3u8_parser_attr_t));

    size_t key_s = pmatch[1].rm_eo - pmatch[1].rm_so;
    size_t value_s = pmatch[2].rm_eo - pmatch[2].rm_so;

    attr->key = strndup(cursor + pmatch[1].rm_so, key_s);
    attr->value = strndup(cursor + pmatch[2].rm_so, value_s);

    if (m3u8_linked_inb(&attrs->list, &attr->list) != M3U8_LINKED_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_PARSER_STATUS_LIST_ERROR, "Could not insert attr in list");
    }

    if (pmatch[0].rm_eo == 0)
      break;

    cursor += pmatch[0].rm_eo;
  }

clean_up:
  regfree(&regex);
  return status;
}

m3u8_parser_status_t m3u8_parser_attr_from_key(m3u8_parser_attr_t* attrs, m3u8_parser_attr_t** attr,
                                               char* key) {
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;
  m3u8_parser_attr_t*  entry = NULL;

  if (attrs == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (attr == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg attr (null)");
  }

  if (key == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg key (null)");
  }

  m3u8_linked_foreach(entry, &attrs->list, m3u8_parser_attr_t, list) {
    if (strcmp(entry->key, key) == 0) {
      *attr = entry;
      return M3U8_PARSER_STATUS_NO_ERROR;
    }
  }

  status = M3U8_PARSER_STATUS_NOT_FOUND;

clean_up:
  return status;
}

m3u8_parser_status_t m3u8_parser_attr_count(m3u8_parser_attr_t* attrs, int* size) {
  int                  count = 0;
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;

  if (attrs == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (size == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid arg size (null)");
  }

  m3u8_linked_count(&attrs->list, &count);  // NOTE: ignore any errors
  *size = count;

clean_up:
  return status;
}

m3u8_parser_status_t m3u8_parser_attr_destroy(m3u8_parser_attr_t* attrs) {
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;

  if (attrs == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid argument attr (null)");
  }

  m3u8_linked_node_t* next = NULL;
  m3u8_parser_attr_t* entry = NULL;
  m3u8_linked_node_t* pivot = attrs->list.next;

  while (pivot != &attrs->list) {
    next = pivot->next;
    entry = m3u8_linked_container_of(pivot, m3u8_parser_attr_t, list);

    m3u8_linked_remove(&entry->list);

    free(entry->key);
    free(entry->value);
    free(entry);

    pivot = next;
  }

clean_up:
  return status;
}

m3u8_parser_status_t m3u8_parser_from_str(const char* line, m3u8_parser_t** parser) {
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;

  if (line == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid argument line (null)");
  }

  if (parser == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid argument parser (null)");
  }

  if (*parser != NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_INVALID_ARG, "Invalid argument parser content (not null)");
  }

  if ((*parser = malloc(sizeof(m3u8_parser_t))) == NULL) {
    M3U8_RAISE(M3U8_PARSER_STATUS_MEM_ALLOC_ERROR, "Fail to allocate memory for parser");
  }

  memset(*parser, 0, sizeof(m3u8_parser_t));

  if (((*parser)->tag = m3u8_parser_tag_from_str(line)) == M3U8_PARSER_EXT_UNKNOWN) {
    free(*parser);
    *parser = NULL;
    M3U8_RAISE(M3U8_PARSER_STATUS_FAILURE, "Received an invalid tag from manifest line");
  }

  const char* separator = strchr(line, ':');
  (*parser)->name = strdup(m3u8_parser_tag_map[(*parser)->tag].name);
  (*parser)->value = separator ? strdup(separator + 1) : NULL;

  if ((*parser)->value != NULL) {
    if (((*parser)->attrs = malloc(sizeof(m3u8_parser_attr_t))) == NULL) {
      free((*parser)->name);
      free((*parser)->value);
      free(*parser);
      *parser = NULL;
      M3U8_RAISE(M3U8_PARSER_STATUS_MEM_ALLOC_ERROR, "Fail to allocate memory for attrs");
    }

    memset((*parser)->attrs, 0, sizeof(m3u8_parser_attr_t));

    if (m3u8_parser_attr((*parser)->value, (*parser)->attrs) != M3U8_PARSER_STATUS_NO_ERROR) {
      free((*parser)->attrs);
      (*parser)->attrs = NULL;
    }
  }

clean_up:
  return status;
}

m3u8_parser_tag_t m3u8_parser_tag_from_str(const char* line) {
  regex_t    regex;
  regmatch_t pmatch[2];  // 0: full match, 1: match

  size_t length = 0;
  char*  match = NULL;

  const char*        pivot = line;
  m3u8_parser_tag_t  tag = M3U8_PARSER_EXT_UNKNOWN;
  static const char* pattern = "^#(EXT(-X-[A-Z0-9-]+|[A-Z0-9-]+))(:.*)?";

  if (line == NULL) {
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  if (*pivot != '#' || strncmp(line, "#EXT", 4) != 0) {
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  memset(&regex, 0, sizeof(regex_t));

  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  if (regexec(&regex, line, 2, pmatch, 0) != 0) {
    regfree(&regex);
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  if (pmatch[1].rm_so < 0 || pmatch[1].rm_eo < pmatch[1].rm_so) {
    regfree(&regex);
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  length = pmatch[1].rm_eo - pmatch[1].rm_so;

  if ((match = strndup(line + pmatch[1].rm_so, length)) == NULL) {
    regfree(&regex);
    return M3U8_PARSER_EXT_UNKNOWN;
  }

  for (tag = M3U8_PARSER_EXTM3U; tag < M3U8_PARSER_EXT_UNKNOWN; tag++) {
    if (strcmp(match, m3u8_parser_tag_map[tag].name) == 0) {
      break;
    }
  }

  free(match);
  regfree(&regex);
  return tag;
}

m3u8_parser_playlist_type_t m3u8_parser_playlist_type(const char* manifest) {
  if (manifest == NULL) {
    return M3U8_PARSER_UNKNOWN_PLAYLIST;
  }

  char*       buffer = NULL;
  const char* pivot = manifest;

  while (*pivot != '\0') {
    const char* line = strchr(pivot, '\n');
    size_t      length = line == NULL ? strlen(pivot) : line - pivot;

    if (length == 0 && line == NULL) {
      // if line is empty and EOF, just not found nothing
      return M3U8_PARSER_UNKNOWN_PLAYLIST;
    } else if (length == 0) {
      // if line is empty only, go to next line find something
      pivot = line + 1;
      continue;
    }

    // allocates the line size and try find something
    if ((buffer = (char*)malloc(length + 1)) != NULL) {
      strncpy(buffer, pivot, length);
      buffer[length] = '\0';
    } else {
      return M3U8_PARSER_UNKNOWN_PLAYLIST;
    }

    if (strstr(buffer, "#EXT-X-STREAM-INF") != NULL) {
      free(buffer);  // Libera o buffer antes de retornar
      return M3U8_PARSER_MASTER_PLAYLIST;
    }

    if (strstr(buffer, "#EXTINF") != NULL) {
      free(buffer);
      return M3U8_PARSER_MEDIA_PLAYLIST;
    }

    free(buffer);
    buffer = NULL;

    if (line != NULL) {
      pivot = line + 1;
    } else {
      return M3U8_PARSER_UNKNOWN_PLAYLIST;
    }
  }

  return M3U8_PARSER_UNKNOWN_PLAYLIST;
}
