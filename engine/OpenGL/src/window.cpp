#include "glad/glad.h"
#include "glfw-linux/glfw3.h"

#include "logger.h"
#include "util.h"
#include "window.h"

#include <GL/glext.h>
#include <vector>

namespace nvogl {
static void _debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *user_param);

GLWindowConfig::GLWindowConfig(int width, int height, const char *title)
    : Width(width), Height(height), Title(title), Resizable(false), Visible(true)
{
}

static void frameResizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
    LogDebug("RESIZE: (%d, %d)", width, height);
}

static std::vector<KeyActionInfo> keyActions{};
// TODO: handle modifiers like caps lock
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)window;
    (void)mods;
    (void)scancode;

    KeyActionInfo ka;
    switch (action) {
        case GLFW_PRESS:
            ka.action = Action::Press;
            break;
        case GLFW_RELEASE:
            ka.action = Action::Release;
            break;
    }
    ka.key = (Key)key;
    keyActions.push_back(ka);
}

GLWindow::GLWindow(GLWindowConfig config)
    : m_Width(config.Width), m_Height(config.Height), m_Title(config.Title), m_Window(nullptr)
{
    if (glfwInit() != GLFW_TRUE) {
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

    LogInfo("Device Information");
    LogInfo("\tGPU: %s", glGetString(GL_RENDERER));
    LogInfo("\tOpenGL Version: %s", glGetString(GL_VERSION));
    LogInfo("Window initialized with these parameter");
    LogInfo("    Width: . . . . . . . %d", config.Width);
    LogInfo("    Height:  . . . . . . %d", config.Height);
    LogInfo("    Title: . . . . . . . %s", config.Title);
    LogInfo("    Resizable: . . . . . %s", config.Resizable ? "Yes" : "No");
    LogInfo("    Visible: . . . . . . %s", config.Visible ? "Yes" : "No");
    // LogInfo("    Fullscreen:  . . . . %s", config.Fullscreen ? "Yes" : "No");

    glViewport(0, 0, m_Width, m_Height);

    // Resize viewport upon window resize
    glfwSetFramebufferSizeCallback(m_Window, frameResizeCallback);
    glfwSetKeyCallback(m_Window, keyCallback);

    glfwGetWindowSize(m_Window, &m_Width, &m_Height);

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glad_glDebugMessageCallback(_debug_callback, NULL);
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

const char *GLWindow::GetTitle() const
{
    return m_Title;
}

int GLWindow::GetWidth() const
{
    return m_Width;
}
void GLWindow::SetWidth(int newWidth)
{
    m_Width = newWidth;
}

int GLWindow::GetHeight() const
{
    return m_Height;
}
void GLWindow::SetHeight(int newHeight)
{
    m_Height = newHeight;
}

Vec2 GLWindow::GetMousePosition() const
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return Vec2(x, y);
}

Vec2i GLWindow::GetDimensions() const
{
    return Vec2i(m_Width, m_Height);
}

bool GLWindow::WindowShouldClose() const
{
    return static_cast<bool>(glfwWindowShouldClose(m_Window));
}

GLFWwindow *GLWindow::GetWindowHandle() const
{
    return m_Window;
}

double GLWindow::GetGLFWTime() const
{
    return glfwGetTime();
}

bool GLWindow::IsKeyPressed(Key key) const
{
    for (size_t i = 0; i < keyActions.size(); i++) {
        const KeyActionInfo ka = keyActions[i];
        if (ka.key == key && ka.action == Action::Press) {
            keyActions.erase(keyActions.begin() + i);
            return true;
        }
    }
    return false;
}

static void _debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam)
{
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    const char *severity_str;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_str = "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_str = "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severity_str = "low";
            break;
        default:
            severity_str = "unknown";
            break;
    }
    LogError("[%s] GL CALLBACK: type = 0x%x, severity = %s, message = '%s'\n",
           (type == GL_DEBUG_TYPE_ERROR ? "GL ERROR" : ""), type, severity_str, message);
}

} // namespace nvogl
