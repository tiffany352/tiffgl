#ifndef TGL_FBO_H
#define TGL_FBO_H

#include <stdbool.h>

typedef struct tgl_fbo_target {
    GLenum target, fmt, ifmt, type, attach;
    unsigned awidth, aheight, samples;
    bool multisample, fixedsamplelocs;
    float rwidth, rheight;
} tgl_fbo_target;

typedef struct tgl_fbo {
    GLuint fbo;
    GLuint *textures;
    tgl_fbo_target *targets;
    unsigned num;
    bool complete, have_textures;
} tgl_fbo;

enum tgl_fbo_type {
    TGL_FBO_READ    = 1,
    TGL_FBO_WRITE   = 2,
    TGL_FBO_RW      = TGL_FBO_READ|TGL_FBO_WRITE,
};

void tgl_fbo_init(tgl_fbo *fbo);
void tgl_fbo_free(tgl_fbo *fbo);

void tgl_fbo_numTargets(tgl_fbo *self, unsigned num);
void tgl_fbo_size_abs(tgl_fbo *self, unsigned target, unsigned w, unsigned h);
void tgl_fbo_size_rel(tgl_fbo *self, unsigned target, float w, float h);
void tgl_fbo_texture(tgl_fbo *self, unsigned target, GLenum target_type, GLenum ifmt, GLenum fmt, GLenum attachment, GLenum type);
void tgl_fbo_multisample(tgl_fbo *self, unsigned target, unsigned samples, bool fixed);
GLuint tgl_fbo_getTex(tgl_fbo *self, unsigned target);
void tgl_fbo_bindTex(tgl_fbo *self, unsigned target);
void tgl_fbo_bind(tgl_fbo *self, enum tgl_fbo_type type);
void tgl_fbo_swap(tgl_fbo *self, unsigned target1, unsigned target2);
void tgl_fbo_bind_with(tgl_fbo *self, enum tgl_fbo_type type, unsigned n, const unsigned *order);

bool /*success*/ tgl_fbo_build(tgl_fbo *self, unsigned w, unsigned h);

#endif
