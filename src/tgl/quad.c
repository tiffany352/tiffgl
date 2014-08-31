#include "tgl/gl.h"

#include "quad.h"

void tgl_quad_init(tgl_quad *q, unsigned attrib)
{
    static const float verts[] = {
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
    };
    glGenBuffers(1, &q->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, q->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(attrib);
}

void tgl_quad_free(tgl_quad *q)
{
    glDeleteBuffers(1, &q->vbo);
}

void tgl_quad_draw(tgl_quad *q, unsigned num)
{
    (void)q;
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, num);
}

void tgl_quad_draw_once(tgl_quad *q)
{
    (void)q;
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
