#ifndef TGL_GL_H
#define TGL_GL_H

#ifdef TGL_USE_GLEW
#include <GL/glew.h>
#define TGL_EXTENSION(n) GLEW_##n
#elif defined(TGL_USE_EPOXY)
#include <epoxy/gl.h>
#define TGL_EXTENSION(n) epoxy_has_gl_extension("GL_" #n)
#else
#error You must provide a GL function loader. Currently, GLEW and libepoxy are supported. Use \
    -DTGL_USE_GLEW or -DTGL_USE_EPOXY
#endif

#endif
