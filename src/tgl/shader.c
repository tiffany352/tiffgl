#include <GL/glew.h>
#include <stdbool.h>
#include <assert.h>

#include "glutil.h"
#include "shader.h"

bool tgl_make_shader(GLuint *shader, GLenum type, const char *source, size_t len)
{
    assert(source);
    tgl_check("Unknown");

    *shader = glCreateShader(type);
    tgl_check("glCreateShader");

    glShaderSource(*shader, 1, (const GLchar**)&source, (const GLint*)&len);
    tgl_check("glShaderSource");

    glCompileShader(*shader);
    tgl_check("glCompileShader");

    GLint status, infolen;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infolen);
    if (infolen > 1) {
        char str[infolen];
        glGetShaderInfoLog(*shader, infolen, NULL, str);
        tgl_log("%s Shader info log: %s", type == GL_VERTEX_SHADER? "Vertex" : "Fragment", str);
    }
    return status == GL_TRUE;
}

bool tgl_link_program(GLuint program)
{
    glLinkProgram(program);
    tgl_check("glLinkProgram");

    GLint status, len;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        char str[len];
        glGetProgramInfoLog(program, len, NULL, str);
        tgl_log("Program info log: %s", str);
    }
    if (status == GL_FALSE) {
        return false;
    }

    glValidateProgram(program);
    tgl_check("glValidateProgram");

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        char str[len];
        glGetProgramInfoLog(program, len, NULL, str);
        tgl_log("Program info log: %s", str);
    }
    if (status == GL_FALSE) {
        return false;
    }
    return true;
}
