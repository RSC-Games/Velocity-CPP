#include "glad/glad.h"
#include "logger.h"
#include "shader.h"
#include <cstdio>
#include <cstring>
#include <cwchar>

#define INFO_LOG_BUF_LEN 512

namespace Velocity {
static const char *DEFAULT_VERT_PATH = "../resources/shader/texture.vert";
static const char *DEFAULT_FRAG_PATH = "../resources/shader/texture.frag";

GLShader::GLShader() : m_ProgramId(0) {}

GLShader::~GLShader() { glDeleteProgram(m_ProgramId); }

static char *readEntireFile(const char *path);
void GLShader::LoadDefaults() {
    bool compiled = true, linked = true;

    GLint success;
    char info_log_buffer[INFO_LOG_BUF_LEN] = {0};
    char *vertex_source = readEntireFile(DEFAULT_VERT_PATH);
    int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    if (vertex_id == 0) {
        LogFatal("Unable to create vertex shader id");
    }
    // TODO: handle 'null' cases of the shader id
    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);

    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_id, INFO_LOG_BUF_LEN, NULL, info_log_buffer);
        LogError("Vertex shader error: %s", info_log_buffer);
        compiled = false;
    } else {
        LogDebug("Successfully compiled vertex shaders");
    }

    memset(info_log_buffer, 0, INFO_LOG_BUF_LEN);
    char *fragment_source = readEntireFile(DEFAULT_FRAG_PATH);
    int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragment_id == 0) {
        LogFatal("Unable to create fragment shader id");
    }
    // TODO: handle 'null' cases of the shader id
    glShaderSource(fragment_id, 1, &fragment_source, NULL);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_id, INFO_LOG_BUF_LEN, NULL,
                           info_log_buffer);
        LogError("Fragment shader error: %s", info_log_buffer);
        compiled = false;
    } else {
        LogDebug("Successfully compiled fragment shaders");
    }

    int shader_program_id = glCreateProgram();
    if (shader_program_id == 0) {
        LogFatal("Unable to create shader program");
    }
    glAttachShader(shader_program_id, vertex_id);
    glAttachShader(shader_program_id, fragment_id);
    glLinkProgram(shader_program_id);

    memset(info_log_buffer, 0, INFO_LOG_BUF_LEN);
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program_id, INFO_LOG_BUF_LEN, NULL,
                            info_log_buffer);
        LogError("Shader link error: %s", info_log_buffer);
        linked = false;
    } else {
        LogDebug("Successfully linked shaders");
    }

    // Shader have to be deleted before sources' memory are freed
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    delete[] vertex_source;
    delete[] fragment_source;

    if (compiled && linked) {
        LogInfo("Intialized default shader");
        m_ProgramId = shader_program_id;
    } else {
        LogFatal("Couldn't load default shaders");
    }
}

void GLShader::Bind() const { glUseProgram(m_ProgramId); }

int GLShader::GetProgramId() const { return m_ProgramId;}

// WARNING: This function allocates memory
static char *readEntireFile(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        LogFatal("Unable to locate '%s'\n", path);
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);

    // Create a buffer.
    char *result = new char[size + 1];

    // Read the whole file into the buffer.
    fread(result, size, 1, f);
    result[size] = '\0';
    fclose(f);

    return result;
}
} // namespace Velocity
