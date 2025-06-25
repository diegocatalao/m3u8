#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/logger.h"
#include "../src/m3u8.h"

/**
 * @brief Operation completed successfully.
 *
 * @details Returned when a function executes without error. May also be used 
 * as a default or propagated status value indicating success.
 */
#define STATUS_NO_ERROR               0x00000000
/** 
 * @brief Received an invalid argument from function params.
 * 
 * @details Received an invalid argument from function params. Usually is used 
 * when some pointer is null or data structure is invalid.
 */
#define STATUS_INVALID_ARG            0x00000001

/**
 * @brief Received a bad status from CURL library or something like.
 *
 * @details When CURL is initialized and the request returns a bad status this
 * status will be returned.
 */
#define STATUS_INIT_CURL_ERROR        0x00000002

/**
 * @brief Received an error from CURL library and the operation was stopped.
 * 
 * @details When some CURL operation is bad this will be returned. Its just 
 * ignore the all returned value from CURL but you can get it from logger 
 * message as well.
 */
#define STATUS_CURL_OP_ERROR          0x00000003

/**
 * @brief Received bad status from `m3u8_load_from_str` function.
 * 
 * @details When something went wrong in the m3u8 manifest parser occurred.
 * Can describe a bad manifest structure or a bad value inside the manifest.
 */

#define STATUS_FAIL_TO_PARSE_MANIFEST 0x00000004

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
 * @param userp      pointer to a response_t structure that accumulates the 
 *                   data.
 *
 * @return The number of bytes successfully handled, or 0 on memory allocation 
 *         failure.
 */
size_t get_m3u8_handler_fn(void* data, size_t size, size_t nmemb, void* userp) {
  size_t      bytes_s = size * nmemb;
  response_t* memory_ptr = (response_t*)userp;

  memory_ptr->data = realloc(memory_ptr->data, memory_ptr->size + bytes_s + 1);

  if (memory_ptr->data == NULL) {
    ERROR("Out of memory while reallocating buffer");
    return 0;
  }

  memcpy(&(memory_ptr->data[memory_ptr->size]), data, bytes_s);

  memory_ptr->size += bytes_s;
  memory_ptr->data[memory_ptr->size] = 0;

  return bytes_s;
}

/**
 * @brief Fetches and parses an M3U8 playlist from a remote URI.
 *
 * @param uri        remote M3U8 URI (null-terminated string).
 * @param m3u8_ptr   pointer to a valid m3u8_t structure to be filled.
 *
 * @retval STATUS_NO_ERROR        on success.
 * @retval STATUS_INVALID_ARG     if uri or m3u8_ptr is NULL.
 * @retval STATUS_INIT_CURL_ERROR if curl initialization or setup fails.
 * @retval STATUS_CURL_OP_ERROR   if the download fails.
 */
int get_m3u8_from_remote(char* uri, char** m3u8_str) {
  int status = STATUS_NO_ERROR;

  CURLcode   status_code = CURLE_OK;
  CURL*      curl = curl_easy_init();
  response_t response = {.data = NULL, .size = 0};

  if (uri == NULL) {
    RAISE(STATUS_INVALID_ARG, "Invalid value for URI NULL");
  }

  if (curl == NULL) {
    RAISE(STATUS_INIT_CURL_ERROR, "Curl was bad initialized");
  }

  curl_easy_setopt(curl, CURLOPT_URL, uri);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_m3u8_handler_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

  status_code = curl_easy_perform(curl);

  if (status_code != CURLE_OK) {
    ERROR("The request was failed: %s", curl_easy_strerror(status_code));
    RAISE(STATUS_INIT_CURL_ERROR, "Bad when trying download");
  }

  if (response.size == 0) {
    RAISE(STATUS_CURL_OP_ERROR, "Received an empty response");
  }

  *m3u8_str = strdup(response.data);

clean_up:
  if (curl != NULL) {
    curl_easy_cleanup(curl);
  }

  if (response.data != NULL) {
    free(response.data);
  }

  return status;
}

int main(int argc, char** argv) {
  int status = STATUS_NO_ERROR;

  char*   m3u8_str = NULL;
  m3u8_t* m3u8_ptr = NULL;

  if (argc == 1) {
    perror("Invalid NULL URL");
    RAISE(STATUS_INVALID_ARG, "Invalid input argument");
  }

  status = get_m3u8_from_remote(argv[0], &m3u8_str);

  if (status != STATUS_NO_ERROR) {
    RAISE(-1, "Fail to download the file [0x%08x]", status);
  }

  status = m3u8_load_from_str(m3u8_str, m3u8_ptr);

  if (status != M3U8_STATUS_NO_ERROR) {
    RAISE(STATUS_FAIL_TO_PARSE_MANIFEST, "Cannot parse manifest");
  }

clean_up:
  return status;
}
