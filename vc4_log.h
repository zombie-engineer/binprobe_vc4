#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

static inline void __logger(const char *fmt, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
#if 0
	snprintf(buf + n, sizeof(buf) - n, ": errno: %d(%s)", errno,
		strerror(errno));
#endif
	va_end(ap);
	printf("%s\n", buf);
}

#define _logger(__levelstr, __fmt, ...) __logger(__levelstr ": " __fmt , \
  ##__VA_ARGS__)

#define log_errno(__fmt, ...) _logger("ERR", __fmt ", err: %d(%s)", \
  ##__VA_ARGS__, errno, strerror(errno))
#define log_err(__fmt, ...) _logger("ERR", __fmt, ##__VA_ARGS__)
