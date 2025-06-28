#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "attr.h"
#include "list.h"
#include "logger.h"

/**
 * @brief Removes all occurrences of a substring from a string.
 *
 * @param[in,out] str     Pointer to a null-terminated string to modify.
 * @param[in]     replace The character to remove from the string.
 */
void snipx(char* str, const char* sub) {
  char*  match = NULL;
  size_t len_sub = strlen(sub);

  if (len_sub == 0)
    return;

  while ((match = strstr(str, sub))) {
    memmove(match, match + len_sub, strlen(match + len_sub) + 1);
  }
}

int m3u8_attr_parse(char* buffer, m3u8_attr_t* attrs) {
  int status = M3U8_ATTR_STATUS_NO_ERROR;

  regex_t    regex = {0};
  regmatch_t pmatch[3];  // 0: full match, 1: key, 2: value

  char* cursor = buffer;
  char* pattern = "([A-Z0-9_-]+)=(\"[^\"]*\"|[^,]+)";

  if (buffer == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg buffer (null)");
  }

  if (attrs == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (m3u8_list_init(&attrs->list) != M3U8_LIST_STATUS_NO_ERROR) {
    RAISE(M3U8_ATTR_STATUS_LIST_ERROR, "Fail to initialize the attribute list");
  }

  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    RAISE(M3U8_ATTR_STATUS_MEM_ALLOC_ERROR, "Bad regular expression");
  }

  while (regexec(&regex, cursor, 3, pmatch, 0) == 0) {
    m3u8_attr_t* attr = (m3u8_attr_t*)malloc(sizeof(m3u8_attr_t));

    if (attr == NULL) {
      RAISE(M3U8_ATTR_STATUS_MEM_ALLOC_ERROR, "Unable to allocate attribute");
    }

    memset(attr, 0, sizeof(m3u8_attr_t));

    size_t key_s = pmatch[1].rm_eo - pmatch[1].rm_so;
    size_t value_s = pmatch[2].rm_eo - pmatch[2].rm_so;

    attr->key = strndup(cursor + pmatch[1].rm_so, key_s);
    attr->value = strndup(cursor + pmatch[2].rm_so, value_s);

    snipx(attr->value, "\"");

    if (m3u8_list_inb(&attrs->list, &attr->list) != M3U8_LIST_STATUS_NO_ERROR) {
      RAISE(M3U8_ATTR_STATUS_LIST_ERROR, "Could not insert attribute in list");
    }

    if (pmatch[0].rm_eo == 0)
      break;

    cursor += pmatch[0].rm_eo;
  }

clean_up:
  regfree(&regex);
  return status;
}

int m3u8_attr_from_key(m3u8_attr_t* attrs, m3u8_attr_t** attr, char* key) {
  int status = M3U8_ATTR_STATUS_NOT_FOUND;

  m3u8_attr_t* entry = NULL;

  if (attrs == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (attr == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg attr (null)");
  }

  if (key == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg key (null)");
  }

  m3u8_list_foreach(entry, &attrs->list, m3u8_attr_t, list) {
    if (strcmp(entry->key, key) == 0) {
      *attr = entry;
      return M3U8_ATTR_STATUS_NO_ERROR;
    }
  }

clean_up:
  return status;
}

int m3u8_attr_count(m3u8_attr_t* attrs, int* size) {
  int count = 0;
  int status = M3U8_ATTR_STATUS_NO_ERROR;

  if (attrs == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg attrs (null)");
  }

  if (size == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid arg size (null)");
  }

  m3u8_list_count(&attrs->list, &count);  // NOTE: ignore any errors

  *size = count;

clean_up:
  return status;
}

int m3u8_attr_destroy(m3u8_attr_t* attrs) {
  int status = M3U8_ATTR_STATUS_NO_ERROR;

  if (attrs == NULL) {
    RAISE(M3U8_ATTR_STATUS_INVALID_ARG, "Invalid argument attr (null)");
  }

  m3u8_list_node_t* next = NULL;
  m3u8_attr_t*      entry = NULL;
  m3u8_list_node_t* pivot = attrs->list.next;

  while (pivot != &attrs->list) {
    next = pivot->next;
    entry = m3u8_list_container_of(pivot, m3u8_attr_t, list);

    m3u8_list_remove(&entry->list);

    free(entry->key);
    free(entry->value);
    free(entry);

    pivot = next;
  }

clean_up:
  return status;
}
