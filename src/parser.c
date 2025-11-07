#include "parser.h"
#include "logger.h"

#include <regex.h>
#include <stdlib.h>

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
    M3U8_RAISE(M3U8_PARSER_STATUS_LIST_ERROR,
               "Fail to initialize the attribute list");
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

    if (m3u8_linked_inb(&attrs->list, &attr->list) !=
        M3U8_LINKED_STATUS_NO_ERROR) {
      M3U8_RAISE(M3U8_PARSER_STATUS_LIST_ERROR,
                 "Could not insert attr in list");
    }

    if (pmatch[0].rm_eo == 0)
      break;

    cursor += pmatch[0].rm_eo;
  }

clean_up:
  regfree(&regex);
  return status;
}

m3u8_parser_status_t m3u8_parser_attr_from_key(m3u8_parser_attr_t*  attrs,
                                               m3u8_parser_attr_t** attr,
                                               char*                key) {
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

m3u8_parser_status_t m3u8_parser_attr_count(m3u8_parser_attr_t* attrs,
                                            int*                size) {
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

m3u8_parser_status_t m3u8_parser_from_str(const char*     line,
                                          m3u8_parser_t** parser) {
  m3u8_parser_status_t status = M3U8_PARSER_STATUS_NO_ERROR;
  M3U8_RAISE(M3U8_PARSER_STATUS_UNKNOWN_ERROR, "Not implemented");
clean_up:
  return status;
}

m3u8_parser_playlist_type_t m3u8_parser_playlist_type(const char* line) {
  m3u8_parser_playlist_type_t type = M3U8_PARSER_UNKNOWN_PLAYLIST;
  return type;
}
