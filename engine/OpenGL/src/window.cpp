#include "glad/glad.h"
#include "glfw-linux/glfw3.h"

#include "logger.h"
#include "util.h"
#include <GL/glext.h>
#include "window.h"

namespace Velocity {
GLWindowConfig::GLWindowConfig(int width, int height, const std::string& title)
    : Width(width), Height(height), Title(title), Resizable(false),
      Visible(true) {}

static void frameResizeCallback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    LogDebug("RESIZE: (%d, %d)", width, height);
}

static void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}


GLWindow::GLWindow(GLWindowConfig config)
    : m_Width(config.Width), m_Height(config.Height),
      m_Title(config.Title.c_str()), m_Window(nullptr) {
    if (!glfwInit()) {
        LogFatal("Unable to initialize glfw");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, config.Resizable);
    glfwWindowHint(GLFW_VISIBLE, config.Visible);
    // TODO: handle fullscreen config

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
    if (m_Window == nullptr) {
        glfwTerminate();
        LogFatal("Unable to create window");
    }

    glfwMakeContextCurrent(m_Window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LogFatal("Failed to initialize GLAD.");
    }

    glfwSwapInterval(1);

    LogInfo("Device Information\n\tGPU: %s", glGetString(GL_VERSION));
    LogInfo("Window initialized with these parameter");
    LogInfo("    Width: . . . . . . . %d", config.Width);
    LogInfo("    Height:  . . . . . . %d", config.Height);
    LogInfo("    Title: . . . . . . . %s", config.Title.c_str());
    LogInfo("    Resizable: . . . . . %s", config.Resizable ? "Yes" : "No");
    LogInfo("    Visible: . . . . . . %s", config.Visible ? "Yes" : "No");
    // LogInfo("    Fullscreen:  . . . . %s", config.Fullscreen ? "Yes" : "No");

    glViewport(0, 0, m_Width, m_Height);

    // Resize viewport upon window resize
    glfwSetFramebufferSizeCallback(m_Window, frameResizeCallback);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

GLWindow::~GLWindow() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

const char *GLWindow::GetTitle() const { return m_Title; }

int GLWindow::GetWidth() const { return m_Width; }
void GLWindow::SetWidth(int newWidth) { m_Width = newWidth; }

int GLWindow::GetHeight() const { return m_Height; }
void GLWindow::SetHeight(int newHeight) { m_Height = newHeight; }

Vec2 GLWindow::GetMousePosition() const {
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return Vec2(x, y);
}

Vec2i GLWindow::GetDimensions() const { return Vec2i(m_Width, m_Height); }

bool GLWindow::WindowShouldClose() const {
    return static_cast<bool>(glfwWindowShouldClose(m_Window));
}

GLFWwindow *GLWindow::GetWindowHandle() const { return m_Window; }

double GLWindow::GetGLFWTime() const {
    return glfwGetTime();
}

} // namespace Velocity
