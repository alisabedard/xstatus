#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#ifdef DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__);
#else
#define LOG(...)
#endif
#endif//LOG_H
