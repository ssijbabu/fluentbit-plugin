/* Common utilities for Fluent Bit plugins */

#ifndef PLUGIN_UTILS_H
#define PLUGIN_UTILS_H

#include <fluent-bit/flb_log.h>
#include <time.h>
#include <stdlib.h>

/* Log helper functions */
#define LOG_INFO(fmt, ...) \
    flb_println(0, "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    flb_println(1, "[ERROR] [%s] " fmt, __FUNCTION__, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
    flb_println(2, "[DEBUG] [%s] " fmt, __FUNCTION__, ##__VA_ARGS__)

/* String utilities */
static inline char* plugin_str_dup(const char *str)
{
    if (str == NULL) {
        return NULL;
    }
    char *dup = malloc(strlen(str) + 1);
    if (dup) {
        strcpy(dup, str);
    }
    return dup;
}

/* Memory safe free */
static inline void plugin_free(void *ptr)
{
    if (ptr) {
        free(ptr);
    }
}

#endif /* PLUGIN_UTILS_H */
