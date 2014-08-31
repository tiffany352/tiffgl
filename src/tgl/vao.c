#include "tgl/gl.h"

#include "vao.h"

void tgl_vao_init(tgl_vao *v)
{
    glGenVertexArrays(1, &v->vao);
}

void tgl_vao_free(tgl_vao *v)
{
    glDeleteVertexArrays(1, &v->vao);
}

void tgl_vao_bind(tgl_vao *v)
{
    glBindVertexArray(v->vao);
}
