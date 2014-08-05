#ifndef TGL_GLUTIL_H
#define TGL_GLUTIL_H

typedef void (*tgl_log_fn)(const char *file, int line, const char *func, const char *msg, void *user);

const char * tgl_strerror(GLenum err);
void tgl_log_(const char *file, int line, const char *func, const char *fmt, ...);
void tgl_set_log_func(tgl_log_fn fn, void *user);
void tgl_check_(const char *file, int line, const char *func, const char* fmt, ...);

#ifdef TGL_GETERROR
#define tgl_check(...) tgl_check_(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define tgl_check(...)
#endif

#define tgl_log(...) tgl_log_(__FILE__, __LINE__, __func__, __VA_ARGS__)

#endif
