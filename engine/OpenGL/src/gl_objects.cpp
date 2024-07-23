#include "gl_objects.h"
#include "logger.h"

namespace nvogl {
VAO VAO::Create()
{
    VAO v;
    glGenVertexArrays(1, &v.Handle);
    return v;
}

void VAO::Bind(VAO vao)
{
    glBindVertexArray(vao.Handle);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete(VAO &vao)
{
    glDeleteVertexArrays(1, &vao.Handle);
    vao.Handle = 0;
}

VBO VBO::Create(VBOType type)
{
    VBO v;
    v.Type = type;
    glGenBuffers(1, &v.Handle);
    return v;
}

void VBO::SetData(VBO &vbo, bool is_dynamic, void *data, unsigned int bytes_size)
{
    vbo.IsDynamic = is_dynamic;
    GLuint gl_type = vbo.Type == VBOType::ArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(gl_type, vbo.Handle);
    glBufferData(gl_type, bytes_size, data, is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    // Binding and unbinding immediately might have some unforseen consequences
    // so, this probably has to be dealt with (at some point).
    glBindBuffer(gl_type, 0);
}

void VBO::Bind(VBO &vbo)
{
    GLuint gl_type = vbo.Type == VBOType::ArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(gl_type, vbo.Handle);
}

void VBO::Unbind(VBOType type)
{
    GLuint gl_type = type == VBOType::ArrayBuffer ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(gl_type, 0);
}

void VBO::Delete(VBO &vbo)
{
    glDeleteBuffers(1, &vbo.Handle);
    vbo.Handle = 0;
}

} // namespace nvogl
