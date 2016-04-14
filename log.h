#ifndef XS_LOG_H
#define XS_LOG_H

#include <stdio.h>

#define WARN(...) fprintf(stderr, __VA_ARGS__)
#define ERROR(...) {WARN(__VA_ARGS__); exit(1);}

#ifdef DEBUG
#define LOG(...) {\
	fprintf(stderr, "%s:%d ", __FILE__, __LINE__);\
	WARN(__VA_ARGS__);\
}
#else//!DEBUG
#define LOG(...)
#endif//DEBUG

#endif//!XS_LOG_H
