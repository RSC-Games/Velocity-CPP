#pragma once

#if WIN32
#include "glfw-linux/glfw3.h"
#else
#include "glfw-linux/glfw3.h"
#endif
#include "util.h"

namespace nvogl {

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

// These keys are directly from GLFW.
// Furthermore, not all keys are supported
enum class Key {
    Space = 32,

    Minus = 45,
    Period = 46,

    A = 65, B, C, D, E, F, G,
    H, I, J, K, L, M, N, O, P,
    Q, R, S, T, U, V, W, X, Y, Z,

    Escape = 256,
    Enter = 257,
};

enum class Action {
    Press,
    Release,
};

struct KeyActionInfo {
    Key key;
    Action action;
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

    bool IsKeyPressed(Key key) const;
    bool IsKeyReleased(Key key) const;

    bool WindowShouldClose() const;

private:
    int m_Width;
    int m_Height;
    const char *m_Title;
    GLFWwindow *m_Window;
};

} // namespace nvogl
