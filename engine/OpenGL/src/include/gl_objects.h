#pragma once

#include <glad/glad.h>

namespace nvogl {
struct VAO {
    GLuint Handle;

    static VAO Create();
    static void Bind(VAO vao);
    static void Unbind();
    static void Delete(VAO &vao);
};

enum VBOType
{
    ArrayBuffer,
    ElementBuffer,
};

struct VBO {
    GLuint Handle;
    enum VBOType Type;
    bool IsDynamic;

    static VBO Create(VBOType type);
    static void SetData(VBO &vbo, bool is_dynamic, void *data, unsigned int bytes_size);
    static void Bind(VBO &vbo);
    static void Unbind(VBOType type);
    static void Delete(VBO &vbo);
};

} // namespace nvogl
