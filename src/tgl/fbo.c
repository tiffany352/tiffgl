#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "tgl/gl.h"

#include "glutil.h"
#include "fbo.h"

void tgl_fbo_init(tgl_fbo *fbo)
{
    memset(fbo, 0, sizeof(tgl_fbo));
}

void tgl_fbo_free(tgl_fbo *self)
{
    if (self->textures) {
        free(self->textures);
    }
    if (self->targets) {
        free(self->targets);
    }
}

void tgl_fbo_numTargets(tgl_fbo *self, unsigned num)
{
    self->num = num;
    if (self->have_textures) {
        glDeleteTextures(num, self->textures);
    }
    self->textures = realloc(self->textures, num * sizeof(GLuint));
    self->targets = realloc(self->targets, num * sizeof(tgl_fbo_target));
    memset(self->targets, 0, num * sizeof(tgl_fbo_target));
    for (unsigned i = 0; i < num; i++) {
        self->targets[i].rwidth = 1.0;
        self->targets[i].rheight = 1.0;
    }
    glGenTextures(num, self->textures);
    self->have_textures = true;
}

void tgl_fbo_size_abs(tgl_fbo *self, unsigned target, unsigned w, unsigned h)
{
    assert(target < self->num);
    self->targets[target].awidth = w;
    self->targets[target].aheight = h;
}

void tgl_fbo_size_rel(tgl_fbo *self, unsigned target, float w, float h)
{
    assert(target < self->num);
    self->targets[target].rwidth = w;
    self->targets[target].rheight = h;
}

void tgl_fbo_texture(tgl_fbo *self, unsigned id, GLenum target, GLenum ifmt, GLenum fmt, GLenum attachment, GLenum type)
{
    assert(id < self->num);
    tgl_fbo_target *t = &self->targets[id];
    t->target = target;
    t->ifmt = ifmt;
    t->fmt = fmt;
    t->type = type;
    t->attach = attachment;
}

void tgl_fbo_multisample(tgl_fbo *self, unsigned target, unsigned samples, bool fixed)
{
    assert(target < self->num);
    tgl_fbo_target *t = &self->targets[target];
    t->multisample = true;
    t->samples = samples;
    t->fixedsamplelocs = fixed;
}

GLuint tgl_fbo_getTex(tgl_fbo *self, unsigned target)
{
    assert(target < self->num);
    return self->textures[target];
}

void tgl_fbo_bindTex(tgl_fbo *self, unsigned target)
{
    assert(target < self->num);
    glBindTexture(self->targets[target].target, self->textures[target]);
}

void tgl_fbo_bind(tgl_fbo *self, enum tgl_fbo_type type)
{
    GLenum drawbufs[self->num], target;
    unsigned i;
    for (i = 0; i < self->num; i++) {
        drawbufs[i] = self->targets[i].attach;
    }
    if (type == (TGL_FBO_READ|TGL_FBO_WRITE)) {
        target = GL_FRAMEBUFFER;
    } else if (type == TGL_FBO_READ) {
        target = GL_READ_FRAMEBUFFER;
    } else if (type == TGL_FBO_WRITE) {
        target = GL_DRAW_FRAMEBUFFER;
    } else {
        fprintf(stderr, "Invalid enum value");
        abort();
    }
    glBindFramebuffer(target, self->fbo);
    if (type == TGL_FBO_READ) {
        glReadBuffer(drawbufs[0]);
    } else {
        glDrawBuffers(self->num, drawbufs);
    }
}

void tgl_fbo_bind_with(tgl_fbo *self, enum tgl_fbo_type type,  unsigned n, const unsigned *order)
{
    GLenum drawbufs[n], target;
    unsigned i;
    for (i = 0; i < n; i++) {
        unsigned id = order[i];
        assert(id < self->num);
        drawbufs[i] = self->targets[id].attach;
    }
    if (type == (TGL_FBO_READ|TGL_FBO_WRITE)) {
        target = GL_FRAMEBUFFER;
    } else if (type == TGL_FBO_READ) {
        target = GL_READ_FRAMEBUFFER;
    } else if (type == TGL_FBO_WRITE) {
        target = GL_DRAW_FRAMEBUFFER;
    } else {
        fprintf(stderr, "Invalid enum value");
        abort();
    }
    glBindFramebuffer(target, self->fbo);
    if (type == TGL_FBO_READ) {
        glReadBuffer(drawbufs[0]);
    } else {
        glDrawBuffers(n, drawbufs);
    }

}

void tgl_fbo_swap(tgl_fbo *self, unsigned target1, unsigned target2)
{
    assert(target1 < self->num && target2 < self->num);
    tgl_fbo_target temp = self->targets[target1];
    GLuint tex = self->textures[target1];
    self->targets[target1] = self->targets[target2];
    self->textures[target1] = self->textures[target2];
    self->targets[target2] = temp;
    self->textures[target2] = tex;
}

bool tgl_fbo_build(tgl_fbo *self, unsigned w, unsigned h)
{
    unsigned i;

    if (!glIsFramebuffer(self->fbo)) {
        glGenFramebuffers(1, &self->fbo);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
    for (i = 0; i < self->num; i++) {
        tgl_fbo_target *t = self->targets + i;
        glBindTexture(t->target, self->textures[i]);
        unsigned tw = t->awidth + t->rwidth * w;
        unsigned th = t->aheight + t->rheight * h;
        if (t->multisample) {
            glTexImage2DMultisample(t->target, t->samples, t->fmt,
                                    tw, th, t->fixedsamplelocs);
        } else {
            glTexImage2D(t->target, 0, t->ifmt,
                         tw, th, 0, t->fmt,
                         t->type, NULL);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, t->attach, t->target, self->textures[i], 0);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char *status_str;
        switch (status) {
#define err(n) case n: status_str = #n; break
            err(GL_FRAMEBUFFER_UNDEFINED);
            err(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
            err(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
            err(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
            err(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
            err(GL_FRAMEBUFFER_UNSUPPORTED);
            err(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
            err(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
        default:
                status_str = "Unknown error";
                break;
        }
        tgl_log("Unable to create framebuffer: %s", status_str);
        return false;
    }
    return true;
}
