#ifndef _M3U8_CONATE_H_
#define _M3U8_CONATE_H_

#define CONATE_NO_ERROR        0x1000
#define CONATE_INVALID_POINTER CONATE_NO_ERROR + 0x01
#define CONATE_TIME_ERROR      CONATE_NO_ERROR + 0x02

int conate_timenow(long* obuff);

int conate_timefmt(long* tms, char* obuff, int size, const char* fmt);

#endif  // _M3U8_CONATE_H_
