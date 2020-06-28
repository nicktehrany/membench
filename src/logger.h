#ifndef LOGGER_H
#include <errno.h>
#include <time.h>
#include <stdio.h>
// Logging DEBUG ERROR or INFO as type, errno as value if ERROR otherwise int64_t value to be logged,
// and error Message as message
static inline char *gettime();
static inline char *format_message();
static inline char *format_error();
#define LOG(type, value, message)                                                                                              \
    (type == DEBUG) ? (void)printf("%s | DEBUG | %s:%d | %s\n", gettime(), __FILE__, __LINE__, format_message(message, value)) \
                    : (type == ERROR ? (errno = value, perror(format_error(message, __FILE__, __LINE__)), exit(1))             \
                                     : (void)printf("LOG | %s\n", message))

enum LOG_TYPES
{
    DEBUG = 1,
    ERROR = 2,
    INFO = 3
};

static inline char *gettime()
{
    static char buf[64];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    snprintf(buf, sizeof(buf), "%d-%.2d-%.2d %d:%.2d:%.2d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return buf;
}

static inline char *format_message(char *message, int64_t value)
{
    static char buf[256];
    snprintf(buf, sizeof(buf), "%s %ld", message, value);
    return buf;
}

static inline char *format_error(char *message, char *file, int line)
{
    static char buf[256];
    snprintf(buf, sizeof(buf), "ERROR | %s:%d | %.229s", file, line, message);
    return buf;
}

#endif