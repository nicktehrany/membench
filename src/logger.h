#ifndef LOGGER_H
#include <errno.h>
#include <time.h>
#include <stdio.h>
// LOG ERROR DEBUG or ERROR as type, errno as value if ERROR otherwise int64_t to be logged as value for DEBUG,
// and error Message as message
static inline char *gettime();
static inline char *getmessage();
#define LOG(type, value, message) \
    (type == DEBUG) ? (void)printf("DEBUG: %s %s\n", gettime(), getmessage(message, value)) : \
    (errno = value, perror(message), exit(1))

enum LOG_TYPES
{
    DEBUG = 1,
    ERROR = 2,
};

static inline char *gettime()
{
    static char buf[64];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    snprintf(buf, sizeof(buf), "%d-%d-%d %d:%d:%d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return buf;
}

static inline char *getmessage(char *message, int64_t value)
{
    static char buf[256];
    snprintf(buf, sizeof(buf), "%s %ld", message, value);
    return buf;
}

#endif