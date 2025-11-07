/**
 * @file m3u8_logger.c
 * @brief Implementation of logging functions.
 */
#include "logger.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static m3u8_logger_attr_t* __log_attribute = NULL;

static const char* str_log_severities[] = {
  "M3U8_LOGGER_SEVERITY_VERBOSE", "M3U8_LOGGER_SEVERITY_INFO",  "M3U8_LOGGER_SEVERITY_DEBUG",
  "M3U8_LOGGER_SEVERITY_WARN",    "M3U8_LOGGER_SEVERITY_ERROR", "M3U8_LOGGER_SEVERITY_CRIT",
};

static m3u8_logger_signature_t __log_handlers[M3U8_LOGGER_SET_MAX_HANDLERS + 1] = {
  {
    .name = "m3u8_logger_write_stdout_handler",
    .fnp = (m3u8_logger_handler_t*)m3u8_logger_write_stdout_handler,
  },
  {
    .name = "m3u8_logger_write_file_handler",
    .fnp = (m3u8_logger_handler_t*)m3u8_logger_write_file_handler,
  },
  {.name = NULL, .fnp = NULL},  // SENTINEL
};

static pthread_mutex_t write_file_handler_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t write_stdout_handler_mutex = PTHREAD_MUTEX_INITIALIZER;

m3u8_logger_status_t m3u8_logger_timenow(long* obuff) {
  m3u8_logger_status_t status = M3U8_LOGGER_NO_ERROR;
  struct timeval       tv;

  if (obuff == NULL) {
    M3U8_RAISE(M3U8_LOGGER_INVALID_ARG, "Invalid argument obuff (null)");
  }

  if (gettimeofday(&tv, NULL) != 0) {
    M3U8_RAISE(M3U8_LOGGER_TIME_ERROR, "Invalid input type format");
  }

  if (tv.tv_sec < 0) {
    M3U8_RAISE(M3U8_LOGGER_TIME_ERROR, "Invalid input type format");
  }

  *obuff = tv.tv_sec;

clean_up:
  return status;
}

m3u8_logger_status_t m3u8_logger_timefmt(long* tms, char* obuff, int size, const char* fmt) {
  m3u8_logger_status_t status = M3U8_LOGGER_NO_ERROR;
  struct tm*           tm_info;
  time_t               timestamp = (time_t)(*tms);

  tm_info = localtime(&timestamp);

  if (strftime(obuff, size, fmt, tm_info) == 0) {
    M3U8_RAISE(M3U8_LOGGER_TIME_ERROR, "Invalid input type format");
  }

clean_up:
  return status;
}

static void m3u8_logger_write_stdout_handler(m3u8_logger_event_t event) {
  pthread_mutex_lock(&write_stdout_handler_mutex);

  char        obuff[64];
  const char* msg = "[%s][%s] - %s:%d - %s\n";
  FILE*       outf = (event.severity > M3U8_LOGGER_SEVERITY_DEBUG ? stderr : stdout);

  const char* cseverity = str_log_severities[event.severity];

  long tms;

  if (m3u8_logger_timenow(&tms) != M3U8_LOGGER_NO_ERROR) {
    tms = 0;
  }

  if (m3u8_logger_timefmt(&tms, obuff, sizeof(obuff), M3U8_LOGGER_TIME_FMT) !=
      M3U8_LOGGER_NO_ERROR) {
    snprintf(obuff, sizeof(obuff), "UNKNOWN_TIME");
  }

  fprintf(outf, msg, obuff, cseverity, event.rlt, event.line, event.msg);
  fflush(outf);

  pthread_mutex_unlock(&write_stdout_handler_mutex);
}

static void m3u8_logger_write_file_handler(m3u8_logger_event_t event) {
  pthread_mutex_lock(&write_file_handler_mutex);

  char         fpath[256];
  static FILE* file = NULL;
  static int   lcount = 0, fcount = 0;

  if (__log_attribute == NULL || __log_attribute->path == NULL) {
    pthread_mutex_unlock(&write_file_handler_mutex);
    return;
  }

  if (lcount >= __log_attribute->max_line_size) {
    fclose(file);
    lcount = 0;
    fcount++;
  }

  if (fcount == 0) {
    snprintf(fpath, sizeof(fpath), "%s", __log_attribute->path);
  } else {
    snprintf(fpath, sizeof(fpath), "%s.%d", __log_attribute->path, fcount);
  }

  if ((file = fopen(fpath, "a")) == NULL) {
    pthread_mutex_unlock(&write_file_handler_mutex);
    return;
  }

  char        obuff[64];
  const char* msg = "[%s][%s] - %s:%d - %s\n";
  const char* cseverity = str_log_severities[event.severity];

  long tms;

  if (m3u8_logger_timenow(&tms) != M3U8_LOGGER_NO_ERROR) {
    tms = 0;
  }

  if (m3u8_logger_timefmt(&tms, obuff, sizeof(obuff), M3U8_LOGGER_TIME_FMT) !=
      M3U8_LOGGER_NO_ERROR) {
    snprintf(obuff, sizeof(obuff), "UNKNOWN_TIME");
  }

  fprintf(file, msg, obuff, cseverity, event.rlt, event.line, event.msg);
  fclose(file);

  lcount++;

  pthread_mutex_unlock(&write_file_handler_mutex);
}

static void* m3u8_logger_pthread_handler_fn(void* vargs) {
  m3u8_logger_event_handler_t* ptr_log_event_handler = (m3u8_logger_event_handler_t*)(vargs);
  m3u8_logger_event_t          event = *ptr_log_event_handler->event;
  m3u8_logger_handler_t        signature = (*ptr_log_event_handler).signature->fnp;

  // Calls the function with the appropriate injected event
  ((m3u8_logger_handler_t)signature)(event);

  return NULL;
}

void m3u8_logger(char* msg, char* rlt, int line, m3u8_logger_severity_t severity, ...) {
  va_list ap;
  va_start(ap, severity);

  char buffer[M3U8_LOGGER_BUFFER_SIZE];
  vsnprintf(buffer, M3U8_LOGGER_BUFFER_SIZE, msg, ap);

  va_end(ap);

  m3u8_logger_signature_t* ptr_log_handler = __log_handlers;
  m3u8_logger_event_t*     ptr_log_event = malloc(sizeof(m3u8_logger_event_t));

  ptr_log_event->line = line;
  ptr_log_event->rlt = strdup(rlt);
  ptr_log_event->msg = strdup(buffer);
  ptr_log_event->severity = severity;
  ptr_log_event->timestamp = (int)time(NULL);

  while (ptr_log_handler->name != NULL && ptr_log_handler->fnp != NULL) {
    m3u8_logger_event_handler_t* log_event_handler = malloc(sizeof(m3u8_logger_event_handler_t));

    if (log_event_handler == NULL) {
      free(ptr_log_event->msg);
      free(ptr_log_event->rlt);
      return;
    }

    log_event_handler->event = ptr_log_event;
    log_event_handler->signature = ptr_log_handler;

#ifdef M3U8_PREEMPTIVE_LOGGER
    m3u8_logger_pthread_handler_fn((void*)log_event_handler);
    free(log_event_handler);
#else
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, m3u8_logger_pthread_handler_fn, (void*)log_event_handler);
    pthread_detach(thread_id);
#endif

    ptr_log_handler++;
  }
}

m3u8_logger_status_t m3u8_logger_add_log_handler(char* name, m3u8_logger_handler_t fnp) {
  int status = M3U8_LOGGER_NO_ERROR;

  int                     sent_index = -1;
  m3u8_logger_signature_t signature = {.name = name, .fnp = (m3u8_logger_handler_t*)fnp};
  int                     nhandlers = sizeof(__log_handlers) / sizeof(__log_handlers[0]);

  for (int i = 0; i < nhandlers; i++) {
    if (__log_handlers[i].name == NULL && __log_handlers[i].fnp == NULL) {
      sent_index = i;
      break;
    }
  }

  if (sent_index == -1 || sent_index >= M3U8_LOGGER_SET_MAX_HANDLERS) {
    status = M3U8_LOGGER_HANDLER_LIMIT_ERROR;
    goto clean_up;
  }

  if (sent_index > M3U8_LOGGER_SET_MAX_HANDLERS - 1) {
    status = M3U8_LOGGER_HANDLER_LIMIT_ERROR;
    goto clean_up;
  }

  __log_handlers[sent_index + 1] = __log_handlers[sent_index];
  __log_handlers[sent_index] = signature;

clean_up:
  return status;
}

m3u8_logger_status_t m3u8_logger_remove_log_handler(char* name) {
  int status = M3U8_LOGGER_NO_ERROR;

  int n = 0;
  int found_index = -1;

  while (__log_handlers[n].name != NULL && __log_handlers[n].fnp != NULL) {
    if (strcmp(__log_handlers[n].name, name) == 0) {
      found_index = n;
      break;
    }
    n++;
  }

  if (found_index == -1) {
    status = M3U8_LOGGER_HANDLER_NOT_FOUND;
    goto clean_up;
  }

  while (__log_handlers[n].name != NULL && __log_handlers[n].fnp != NULL) {
    __log_handlers[n] = __log_handlers[n + 1];
    n++;
  }

  __log_handlers[n].name = NULL;
  __log_handlers[n].fnp = NULL;

clean_up:
  return status;
}

m3u8_logger_status_t m3u8_logger_set_log_attribute(m3u8_logger_attr_t attr) {
  int status = M3U8_LOGGER_NO_ERROR;

  if (__log_attribute != NULL) {
    status = M3U8_LOGGER_LOG_ATTR_ALREADY_INITIALIZED;
    goto clean_up;
  }

  if ((__log_attribute = malloc(sizeof(m3u8_logger_attr_t))) == NULL) {
    status = M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if (memset(__log_attribute, 0, sizeof(m3u8_logger_attr_t)) == NULL) {
    status = M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if ((__log_attribute->path = malloc(strlen(attr.path) + 1)) == NULL) {
    status = M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if (strcpy(__log_attribute->path, attr.path) == NULL) {
    status = M3U8_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  __log_attribute->max_line_size = attr.max_line_size;

clean_up:
  return status;
}
