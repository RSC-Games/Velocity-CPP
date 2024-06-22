#pragma once

#include "shader.h"
#include "util.h"
#include "window.h"

namespace Velocity {
class GLRenderer {
  public:
    GLRenderer(int width, int height);

    void Clear(Color color);
    void Present(GLWindow& window);

  private:
    int m_Width;
    int m_Height;
    GLShader m_Shader;
};
} // namespace Velocity
