#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "ext.h"
#include "logger.h"
#include "m3u8.h"

/** @brief response buffer used to store data received from an HTTP request. */
typedef struct {
  char*  data; /**< pointer to the response data buffer */
  size_t size; /**< number of bytes currently stored in the buffer */
} response_t;

/**
 * @brief Callback used by libcurl to write downloaded data into memory.
 *
 * @param contents   pointer to the incoming data buffer;
 * @param size       size of each data unit;
 * @param nmemb      number of data units;
 * @param userp      pointer to a response_t structure that accumulates the data.
 *
 * @return The number of bytes successfully handled, or 0 on memory allocation failure.
 */
static size_t __m3u8_download_handler(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t      total_size = size * nmemb;
  response_t* memory_ptr = (response_t*)userp;

  memory_ptr->data = realloc(memory_ptr->data, memory_ptr->size + total_size + 1);

  if (memory_ptr->data == NULL) {
    ERROR("Out of memory while reallocating buffer");
    return 0;
  }

  memcpy(&(memory_ptr->data[memory_ptr->size]), contents, total_size);
  memory_ptr->size += total_size;
  memory_ptr->data[memory_ptr->size] = 0;

  return total_size;
}

/**
 * @brief Allocates and initializes a new m3u8_t structure.
 *
 * @param m3u8_ptr double pointer to an m3u8_t. Must be NULL on input.
 *
 * @return M3U8_STATUS_NO_ERROR        on success;
 *         M3U8_STATUS_INVALID_ARG     if *m3u8_ptr is not NULL;
 *         M3U8_STATUS_MEM_ALLOC_ERROR on malloc or memset failure;
 *         M3U8_STATUS_UNKNOWN_ERROR   on unexpected failure.
 */
int m3u8_create(m3u8_t** m3u8_ptr) {
  int status = M3U8_STATUS_NO_ERROR;

  if (*m3u8_ptr != NULL) {
    RAISE_STATUS(M3U8_STATUS_INVALID_ARG, "Invalid argument m3u8_ptr, this pointer must be null");
  }

  if ((*m3u8_ptr = (m3u8_t*)malloc(sizeof(m3u8_t*))) == NULL) {
    RAISE_STATUS(M3U8_STATUS_MEM_ALLOC_ERROR, "Unable to allocate memory for m3u8_ptr");
  }

  if (memset(*m3u8_ptr, 0, sizeof(m3u8_t)) == NULL) {
    RAISE_STATUS(M3U8_STATUS_MEM_ALLOC_ERROR, "Fail to memset m3u8_ptr");
  }

clean_up:
  if (status != M3U8_STATUS_NO_ERROR && *m3u8_ptr != NULL) {
    free(*m3u8_ptr);
  }

  return status;
}

/**
 * @brief Deallocates and cleans up a previously created m3u8_t structure.
 *
 * @param m3u8_ptr pointer to an m3u8_t structure to be deallocated.
 *
 * @return M3U8_STATUS_NO_ERROR     on success;
 *         M3U8_STATUS_INVALID_ARG  if m3u8_ptr is NULL.
 */
int m3u8_destroy(m3u8_t* m3u8_ptr) {
  int status = M3U8_STATUS_NO_ERROR;

  if (m3u8_ptr == NULL) {
    RAISE_STATUS(M3U8_STATUS_INVALID_ARG, "Unable to deallocate a null pointer");
  }

  free(m3u8_ptr);

clean_up:
  return status;
}

/**
 * @brief Fetches and parses an M3U8 playlist from a remote URI.
 *
 * @param uri remote M3U8 URI (null-terminated string).
 * @param m3u8_ptr pointer to a valid m3u8_t structure to be filled.
 *
 * @return M3U8_STATUS_NO_ERROR           on success.
 *         M3U8_STATUS_INVALID_ARG        if uri or m3u8_ptr is NULL.
 *         M3U8_STATUS_INIT_CURL_ERROR    if curl fails to initialize or request fails.
 *         M3U8_STATUS_CURL_OP_ERROR      on libcurl transfer error.
 *         M3U8_STATUS_MEM_ALLOC_ERROR    on memory allocation failure.
 *         M3U8_STATUS_UNKNOWN_ERROR      on unexpected failure.
 */
int m3u8_open_from_remote(char* uri, m3u8_t* m3u8_ptr) {
  int status = M3U8_STATUS_NO_ERROR;

  CURLcode   status_code = CURLE_OK;
  CURL*      curl = curl_easy_init();
  response_t response = {.data = NULL, .size = 0};

  if (uri == NULL) {
    RAISE_STATUS(M3U8_STATUS_INVALID_ARG, "Invalid argument uri and m3u8_ptr cannot to be NULL");
  }

  if (curl == NULL) {
    RAISE_STATUS(M3U8_STATUS_INIT_CURL_ERROR, "Curl was bad initialized with curl_easy_init");
  }

  curl_easy_setopt(curl, CURLOPT_URL, uri);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __m3u8_download_handler);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

  status_code = curl_easy_perform(curl);

  if (status_code != CURLE_OK) {
    ERROR("The request was failed: %s", curl_easy_strerror(status_code));
    RAISE_STATUS(M3U8_STATUS_INIT_CURL_ERROR, "Something went wrong in manifest download");
  }

  if (response.size == 0) {
    RAISE_STATUS(M3U8_STATUS_CURL_OP_ERROR, "Received an empty respomse from remote");
  }

  m3u8_ext_parse(response.data, m3u8_ptr);

clean_up:
  if (curl != NULL) {
    curl_easy_cleanup(curl);
  }

  if (response.data != NULL) {
    free(response.data);
  }

  return status;
}
