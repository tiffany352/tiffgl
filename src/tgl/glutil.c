#include <GL/glew.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "glutil.h"

static tgl_log_fn tgl_log_cb;
static void *tgl_log_user;

const char * tgl_strerror(GLenum err)
{
    char * res;
    switch(err) {
    case GL_NO_ERROR:
        res = "GL_NO_ERROR";
        break;
    case GL_INVALID_ENUM:
        res = "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        res = "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        res = "GL_INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        res = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        res = "GL_OUT_OF_MEMORY";
        break;
    case GL_STACK_OVERFLOW:
        res = "GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW:
        res = "GL_STACK_UNDERFLOW";
        break;
    default:
        res = "UNKNOWN ERROR";
        break;
    }
    return res;
}

void tgl_log_(const char *file, int line, const char *func, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char msg[4096];
    vsnprintf(msg, 4096, fmt, ap);
    va_end(ap);
    if (tgl_log_cb) {
        tgl_log_cb(file, line, func, msg, tgl_log_user);
    } else {
        fprintf(stderr, "[%s:%i] %s: %s\n", file, line, func, msg);
    }
}

void tgl_set_log_Func(tgl_log_fn fn, void *user)
{
    tgl_log_cb = fn;
    tgl_log_user = user;
}

void tgl_check_(const char *file, int line, const char *func, const char* fmt, ...)
{
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        va_list ap;
        va_start(ap, fmt);
        char msg[4096];
        size_t off = vsnprintf(msg, 4096, fmt, ap);
        va_end(ap);
        snprintf(msg + off, 4096-off, ": %s (%i)", tgl_strerror(err), err);
        tgl_log_(file, line, func, "%s", msg);
    }
}
