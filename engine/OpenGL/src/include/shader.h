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

    static GLShader LoadPrimitiveDefault();
    static GLShader LoadTextureDefault();
};
} // namespace nvogl
