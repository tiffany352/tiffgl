#ifndef TGL_QUAD_H
#define TGL_QUAD_H

enum tgl_quad_attribs {
    TGL_QUAD_POS,
    TGL_QUAD_NUMATTRIBS
};

typedef struct tgl_quad {
    GLuint vbo;
} tgl_quad;

void tgl_quad_init(tgl_quad *q, unsigned attrib);
void tgl_quad_free(tgl_quad *q);
void tgl_quad_draw(tgl_quad *q, unsigned num);
void tgl_quad_draw_once(tgl_quad *q);

#endif
