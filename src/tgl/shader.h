#ifndef TGL_SHADER_H
#define TGL_SHADER_H

typedef struct tgl_shader {
    GLuint program, vertex, fragment;
} tgl_shader;

bool /*success*/ tgl_make_shader(GLuint *shader, GLenum type, const char *source, size_t len, char **error);
bool /*success*/ tgl_link_program(GLuint program, char **error);

#endif
