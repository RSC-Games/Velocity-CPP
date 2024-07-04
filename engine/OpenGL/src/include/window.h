#pragma once

#include <string>

#if WIN32
#include "glfw-linux/glfw3.h"
#else
#include "glfw-linux/glfw3.h"
#endif
#include "util.h"

namespace Velocity {

struct GLWindowConfig {
    int Width;
    int Height;
    const char *Title;
    bool Resizable;
    // TODO: what does fullscreen mean?
    // Does it mean fullscreen when created or always fullscreen
    // bool Fullscreen;

    bool Visible;

    GLWindowConfig() = delete;
    GLWindowConfig(int width, int height, const char *title);
};

class GLWindow {
public:
    GLWindow() = delete;
    GLWindow(GLWindowConfig config);
    ~GLWindow();

    const char *GetTitle() const;
    Vec2i GetDimensions() const;
    int GetWidth() const;
    void SetWidth(int newWidth);
    int GetHeight() const;
    void SetHeight(int newHeight);
    Vec2 GetMousePosition() const;
    GLFWwindow *GetWindowHandle() const;
    double GetGLFWTime() const;

    bool WindowShouldClose() const;

private:
    int m_Width;
    int m_Height;
    const char *m_Title;
    GLFWwindow *m_Window;
};

} // namespace Velocity
