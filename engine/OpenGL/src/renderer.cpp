#include "renderer.h"
#include <GL/gl.h>

namespace Velocity {
GLRenderer::GLRenderer(int width, int height)
    : m_Width(width), m_Height(height) {
    m_Shader.LoadDefaults();
    m_Shader.Bind();
}

void GLRenderer::Clear(Color color) {
    glClearColor((float)color.R / 255.0f, (float)color.G / 255.0f,
                 (float)color.B / 255.0f, (float)color.A / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::Present(GLWindow& window) {
    glfwSwapBuffers(window.GetWindowHandle());
    glfwPollEvents();
}

} // namespace Velocity
