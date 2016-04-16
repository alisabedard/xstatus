#ifndef XS_CLASS_H
#define XS_CLASS_H

#define $(obj, method, ...) (obj)->method((obj), ##__VA_ARGS__)

#endif//!XS_CLASS_H
