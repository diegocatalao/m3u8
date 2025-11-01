#ifndef _M3U8_CONATE_H_
#define _M3U8_CONATE_H_

/**
 * @enum conate_status_t
 * @brief Represents the status of a conate operation.
 */
typedef enum {
  M3U8_CONATE_NO_ERROR = 0,        /**< Operation completed successfully. */
  M3U8_CONATE_INVALID_POINTER = 1, /**< Invalid pointer argument provided. */
  M3U8_CONATE_TIME_ERROR = 2,      /**< System time operation failed. */
} conate_status_t;

/**
 * @brief Gets the current time as a Unix timestamp.
 * @param obuff Pointer to a long integer to store the timestamp.
 * @return @ref M3U8_CONATE_NO_ERROR on success.
 * @return @ref M3U8_CONATE_INVALID_POINTER if @p obuff is NULL.
 * @return @ref M3U8_CONATE_TIME_ERROR on failure to get time.
 */
conate_status_t conate_timenow(long* obuff);

/**
 * @brief Formats a timestamp into a string.
 * @param tms   Pointer to the timestamp to format.
 * @param obuff Output buffer for the formatted string.
 * @param size  Size of the output buffer.
 * @param fmt   The format string (e.g., "%Y-%m-%d").
 * @return @ref M3U8_CONATE_NO_ERROR on success.
 * @return @ref M3U8_CONATE_INVALID_POINTER if @p tms or @p obuff is NULL.
 */
conate_status_t conate_timefmt(long* tms, char* obuff, int size,
                               const char* fmt);

#endif  // _M3U8_CONATE_H_
