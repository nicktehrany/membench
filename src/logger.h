#ifndef LOGGER_H
#include <errno.h>
#include <time.h>
#include <stdio.h>
// LOG ERROR DEBUG or FATAL as type, errno value as error_val (0 for DEBUG and ERROR), 
// and error Message as message
static inline char *gettime();
#define LOG(type, error_val, message) \
    (type == DEBUG) ? (void)printf("DEBUG: %s %s\n", gettime(), message) : ((type == FATAL) ? \
    (errno = error_val, perror(message), exit(1)) : (void)printf("Error: %s %s\n", __TIME__, message))

enum LOG_TYPES
{
    FATAL = 1,
    ERROR = 2,
    DEBUG = 4
};

static inline char *gettime() 
{
    static char buf[64];
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    snprintf(buf, sizeof(buf), "%d-%d-%d %d:%d:%d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, \
     timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return buf;
}

#endif