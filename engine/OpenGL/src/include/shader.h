#pragma once

namespace nvogl {
struct GLShader {
    int m_ProgramId;

    GLShader();
    ~GLShader();

    bool Load(const char *vertex_path, const char *fragment_path);
    void Bind() const;
    int GetProgramId() const;

    static GLShader LoadPrimitiveDefault();
    static GLShader LoadTextureDefault();
};
} // namespace nvogl
