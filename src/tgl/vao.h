#ifndef TGL_VAO_H
#define TGL_VAO_H

typedef struct tgl_vao {
    GLuint vao;
} tgl_vao;

void tgl_vao_init(tgl_vao *v);
void tgl_vao_free(tgl_vao *v);
void tgl_vao_bind(tgl_vao *v);

#endif
