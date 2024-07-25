#pragma once

#include "gl_objects.h"

namespace nvogl {
struct GLShader {
    GLuint m_ProgramId;

    GLShader();
    ~GLShader();

    bool Load(const char *vertex_path, const char *fragment_path);
    void Bind() const;
    int GetProgramId() const;

    static void LoadPrimitiveDefault(GLShader& shader);
    static void LoadTextureDefault(GLShader& shader);
};
} // namespace nvogl
