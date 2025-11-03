#ifndef _M3U8_LOGGER_H_
#define _M3U8_LOGGER_H_

#include <string.h>

/**
 * @enum m3u8_logger_status_t
 * @brief Represents the status of a m3u8_logger operation.
 */
typedef enum {
  /** Operation completed successfully. */
  M3U8_LOGGER_NO_ERROR = 0,
  /** Invalid argument received in function */
  M3U8_LOGGER_INVALID_ARG = 1,
  /** Resource allocation problem. */
  M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM = 2,
  /** Maximum number of handlers exceeded. */
  M3U8_LOGGER_HANDLER_LIMIT_ERROR = 3,
  /** Log signature not found. */
  M3U8_LOGGER_HANDLER_NOT_FOUND = 4,
  /** Log attribute already initialized. */
  M3U8_LOGGER_LOG_ATTR_ALREADY_INITIALIZED = 5,
  /** Received an invalid time error from some function */
  M3U8_LOGGER_TIME_ERROR = 6,
} m3u8_logger_status_t;

/**
 * @enum m3u8_logger_severity_t
 * @brief Enumeration of log severity levels.
 */
typedef enum {
  /** Detailed information */
  M3U8_LOGGER_SEVERITY_VERBOSE = 0,
  /** General information */
  M3U8_LOGGER_SEVERITY_INFO = 1,
  /** Debugging information */
  M3U8_LOGGER_SEVERITY_DEBUG = 2,
  /** Warning conditions */
  M3U8_LOGGER_SEVERITY_WARN = 3,
  /** Error conditions */
  M3U8_LOGGER_SEVERITY_ERROR = 4,
  /** Critical conditions */
  M3U8_LOGGER_SEVERITY_CRIT = 5,
} m3u8_logger_severity_t;

/**
 * @struct m3u8_logger_attr_t
 * @brief Attributes for configuring log files.
 */
typedef struct {
  /** Path to the log file. */
  char* path;
  /** Maximum size of a log lines in file. */
  int max_line_size;
} m3u8_logger_attr_t;

/**
 * @struct m3u8_logger_event_t
 * @brief Structure representing a log event.
 */
typedef struct {
  /** Timestamp of the event. */
  long long timestamp;
  /** Line number in the source code. */
  int line;
  /** Relative path of the source file. */
  char* rlt;
  /** Log message. */
  char* msg;
  /** Severity of the log event. */
  m3u8_logger_severity_t severity;
} m3u8_logger_event_t;

/**
 * @struct m3u8_logger_signature_t
 * @brief Structure representing a log signature.
 */
typedef struct {
  /** Name of the signature. */
  char* name;
  /** Function pointer to the signature. */
  void* fnp;
} m3u8_logger_signature_t;

/**
 * @struct m3u8_logger_event_handler_t
 * @brief Structure representing an event and its signature.
 */
typedef struct {
  /** Pointer to the log event. */
  m3u8_logger_event_t* event;
  /** Log signature. */
  m3u8_logger_signature_t* signature;
} m3u8_logger_event_handler_t;

/**
 * @typedef m3u8_logger_handler_t
 * @brief Function pointer type for log handlers.
 */
typedef void (*m3u8_logger_handler_t)(m3u8_logger_event_t event);

/**
 * @def M3U8_LOGGER_BUFFER_SIZE
 * @brief Size of the formatted log buffer.
 */
#define M3U8_LOGGER_BUFFER_SIZE      1024

/**
 * @def M3U8_LOGGER_SET_MAX_HANDLERS
 * @brief Maximum number of global log handlers.
 */
#define M3U8_LOGGER_SET_MAX_HANDLERS 100

/**
 * @def M3U8_LOGGER_TIME_FMT
 * @brief Format for date and time in logs.
 */
#define M3U8_LOGGER_TIME_FMT         "%d-%m-%Y %H:%M:%S"

/**
 * @def M3U8_LOGGER_PFX_SRC_PATH
 * @brief Prefix for source path in log messages.
 */
#define M3U8_LOGGER_PFX_SRC_PATH     "src"

/**
 * @def __RLT__
 * @brief Macro to get the relative path of the source file.
 */
#define __RLT__                                   \
  (strstr(__FILE__, M3U8_LOGGER_PFX_SRC_PATH)     \
     ? strstr(__FILE__, M3U8_LOGGER_PFX_SRC_PATH) \
     : __FILE__)

/**
 * @def M3U8_DEBUG
 * @brief Macro to log a debug message.
 * @param message The debug message.
 * @param ... Additional arguments for the debug message.
 */
#define M3U8_DEBUG(message, ...)                                      \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_DEBUG, \
              ##__VA_ARGS__);

/**
 * @def M3U8_INFO
 * @brief Macro to log an informational message.
 * @param message The informational message.
 * @param ... Additional arguments for the informational message.
 */
#define M3U8_INFO(message, ...)                                      \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_INFO, \
              ##__VA_ARGS__);

/**
 * @def M3U8_WARN
 * @brief Macro to log a warning message.
 * @param message The warning message.
 * @param ... Additional arguments for the warning message.
 */
#define M3U8_WARN(message, ...)                                      \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_WARN, \
              ##__VA_ARGS__);

/**
 * @def M3U8_ERROR
 * @brief Macro to log an error message.
 * @param message The error message.
 * @param ... Additional arguments for the error message.
 */
#define M3U8_ERROR(message, ...)                                      \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_ERROR, \
              ##__VA_ARGS__);

/**
 * @def M3U8_CRIT
 * @brief Macro to log a critical message.
 * @param message The critical message.
 * @param ... Additional arguments for the critical message.
 */
#define M3U8_CRIT(message, ...)                                      \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_CRIT, \
              ##__VA_ARGS__);

/**
 * @def M3U8_RAISE
 * @brief Macro to change status to bad code and go to clean up.
 * @param __status The status code to set.
 * @param message The critical message.
 * @param ... Additional arguments for the critical message.
 */
#define M3U8_RAISE(__status, message, ...)                            \
  m3u8_logger(message, __RLT__, __LINE__, M3U8_LOGGER_SEVERITY_ERROR, \
              ##__VA_ARGS__);                                         \
  status = __status;                                                  \
  goto clean_up

/**
 * @brief Gets the current time as a Unix timestamp.
 * @param obuff Pointer to a long integer to store the timestamp.
 * @return @ref CONATE_NO_ERROR on success.
 * @return @ref CONATE_INVALID_POINTER if @p obuff is NULL.
 * @return @ref CONATE_TIME_ERROR on failure to get time.
 */
m3u8_logger_status_t m3u8_logger_timenow(long* obuff);

/**
 * @brief Formats a timestamp into a string.
 * @param tms   Pointer to the timestamp to format.
 * @param obuff Output buffer for the formatted string.
 * @param size  Size of the output buffer.
 * @param fmt   The format string (e.g., "%Y-%m-%d").
 * @return @ref CONATE_NO_ERROR on success.
 * @return @ref CONATE_INVALID_POINTER if @p tms or @p obuff is NULL.
 */
m3u8_logger_status_t m3u8_logger_timefmt(long* tms, char* obuff, int size,
                                         const char* fmt);

/**
 * @brief Writes a log event to stdout or stderr based on severity.
 * @param event The log event to write.
 */
static void m3u8_logger_write_stdout_handler(m3u8_logger_event_t event)
  __attribute__((used));

/**
 * @brief Dummy function for handling log events to a file.
 * @param event The log event to handle.
 */
static void m3u8_logger_write_file_handler(m3u8_logger_event_t event)
  __attribute__((used));

/**
 * @brief Call the thread-signature function with this wrapper.
 * @param vargs The `m3u8_logger_event_handler_t` will be injected here.
 */
static void* m3u8_logger_pthread_handler_fn(void* vargs) __attribute__((used));

/**
 * @brief Logs a msg with the specified severity.
 * @param msg The log msg.
 * @param rlt The relative path of the source file.
 * @param line The line number in the source file.
 * @param severity The severity of the log msg.
 * @param ... Additional arguments for the log msg.
 */
void m3u8_logger(char* msg, char* rlt, int line,
                 m3u8_logger_severity_t severity, ...);

/**
 * @brief Adds a log signature to the m3u8_logger.
 * @param name The name of the log signature.
 * @param fnp The function pointer to the log signature.
 * @return @ref M3U8_LOGGER_NO_ERROR on success.
 * @return @ref M3U8_LOGGER_HANDLER_LIMIT_ERROR if maximum number of handlers
 *              is exceeded.
 */
m3u8_logger_status_t m3u8_logger_add_log_handler(char*                 name,
                                                 m3u8_logger_handler_t fnp);

/**
 * @brief Removes a log signature from the m3u8_logger.
 * @param name The name of the log signature to remove.
 * @return @ref M3U8_LOGGER_NO_ERROR on success.
 * @return @ref M3U8_LOGGER_HANDLER_NOT_FOUND if signature is not found.
 */
m3u8_logger_status_t m3u8_logger_remove_log_handler(char* name);

/**
 * @brief Sets the log attribute for the m3u8_logger.
 * @param attr The log attribute to set.
 * @return @ref M3U8_LOGGER_NO_ERROR on success.
 * @return @ref M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM if resource allocation
 *              problem occurs.
 */
m3u8_logger_status_t m3u8_logger_set_log_attribute(m3u8_logger_attr_t attr);

#endif  // _M3U8_LOGGER_H_
