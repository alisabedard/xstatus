#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#define WARN(...) fprintf(stderr, __VA_ARGS__)
#define ERROR(...) {WARN(__VA_ARGS__); exit(1);}
#ifdef DEBUG
#define LOG(...) WARN(__VA_ARGS__);
#else
#define LOG(...)
#endif
#endif//LOG_H
