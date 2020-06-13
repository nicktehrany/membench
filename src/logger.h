#ifndef LOGGER_H
#include <errno.h>
// LOG ERROR DEBUG or FATAL as type, errno value as error_val (0 for DEBUG and ERROR), 
// and error Message as message
#define LOG(type, error_val, message) \
    (type == DEBUG) ? (void)printf("DEBUG: %s\n", message) : ((type == FATAL) ? \
    (errno = error_val, perror(message), exit(1)) : (void)printf("Error: %s\n", message))

enum LOG_TYPES
{
    FATAL = 1,
    ERROR = 2,
    DEBUG = 4
};

#endif