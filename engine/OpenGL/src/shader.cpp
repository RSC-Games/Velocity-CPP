#include "glad/glad.h"
#include "logger.h"
#include "shader.h"
#include <cstdio>
#include <cstring>

#define INFO_LOG_BUF_LEN 512

namespace nvogl {
static const char *DEFAULT_TEXTURE_VERT_PATH = "../resources/shader/texture.vert";
static const char *DEFAULT_TEXTURE_FRAG_PATH = "../resources/shader/texture.frag";
static const char *DEFAULT_PRIMITIVE_VERT_PATH = "../resources/shader/primitive.vert";
static const char *DEFAULT_PRIMITIVE_FRAG_PATH = "../resources/shader/primitive.frag";

GLShader::GLShader() : m_ProgramId(0) {}

GLShader::~GLShader()
{
    glDeleteProgram(m_ProgramId);
}

GLShader GLShader::LoadPrimitiveDefault()
{
    GLShader shader = GLShader();
    shader.Load(DEFAULT_PRIMITIVE_VERT_PATH, DEFAULT_PRIMITIVE_FRAG_PATH);
    return shader;
}

GLShader GLShader::LoadTextureDefault()
{
    GLShader shader = GLShader();
    shader.Load(DEFAULT_TEXTURE_VERT_PATH, DEFAULT_TEXTURE_FRAG_PATH);
    return shader;
}

static char *readEntireFile(const char *path);
static bool compileShader(const char *source, bool is_vertex, GLuint *id);
static bool linkShader(GLuint vertex_id, GLuint fragment_id, GLuint *id);
bool GLShader::Load(const char *vertex_path, const char *fragment_path)
{
    bool v_compiled, f_compiled, p_linked = true;
    GLuint v_id, f_id, p_id;


    char *vertex_source = readEntireFile(vertex_path);
    v_compiled = compileShader(vertex_source, true, &v_id);

    char *fragment_source = readEntireFile(fragment_path);
    f_compiled = compileShader(fragment_source, false, &f_id);

    linkShader(v_id, f_id, &p_id);

    // Shader have to be deleted before sources' memory are freed
    glDeleteShader(v_id);
    glDeleteShader(f_id);

    delete[] vertex_source;
    delete[] fragment_source;

    if (v_compiled && f_compiled && p_linked) {
        LogInfo("Intialized default shader");
        m_ProgramId = p_id;
    } else {
        LogFatal("Couldn't load default shaders");
        return false;
    }
    return true;
}

void GLShader::Bind() const
{
    glUseProgram(m_ProgramId);
}

int GLShader::GetProgramId() const
{
    return m_ProgramId;
}

static bool compileShader(const char *source, bool is_vertex, GLuint *id)
{
    GLint success;
    char info_log_buffer[INFO_LOG_BUF_LEN] = {0};
    GLuint _id = glCreateShader(is_vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    if (_id == 0) {
        LogFatal("Unable to create %s shader id", is_vertex ? "vertex" : "fragment");
    }
    // TODO: handle 'null' cases of the shader id
    glShaderSource(_id, 1, &source, NULL);
    glCompileShader(_id);

    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_id, INFO_LOG_BUF_LEN, NULL, info_log_buffer);
        LogError("%s shader error: %s", is_vertex ? "Vertex" : "Fragment", info_log_buffer);
        return false;
    }
    LogDebug("Successfully compiled vertex shaders");
    *id = _id;
    return true;
}

static bool linkShader(GLuint vertex_id, GLuint fragment_id, GLuint *id)
{
    GLint success;
    char info_log_buffer[INFO_LOG_BUF_LEN] = {0};

    GLuint _id = glCreateProgram();
    if (_id == 0) {
        LogFatal("Unable to create shader program");
    }
    glAttachShader(_id, vertex_id);
    glAttachShader(_id, fragment_id);
    glLinkProgram(_id);

    memset(info_log_buffer, 0, INFO_LOG_BUF_LEN);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, INFO_LOG_BUF_LEN, NULL, info_log_buffer);
        LogError("Shader link error: %s", info_log_buffer);
        return false;
    }
    LogDebug("Successfully linked shaders");
    *id = _id;
    return true;
}

// WARNING: This function allocates memory
static char *readEntireFile(const char *path)
{
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

} // namespace nvogl
