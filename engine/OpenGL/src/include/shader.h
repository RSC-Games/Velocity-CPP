#pragma once

#if WIN32
#include "glfw-win/glfw3.h"
#else
#include "glfw-linux/glfw3.h"
#endif


namespace Velocity {
struct GLShader {
    int m_ProgramId;

    GLShader();
    ~GLShader();

    void LoadDefaults();
    void Bind();
};
} // namespace Velocity
