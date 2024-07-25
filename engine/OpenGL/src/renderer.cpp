#include "gl_objects.h"
#include "glad/glad.h"
#include "glfw-linux/glfw3.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "logger.h"
#include "renderer.h"
#include "texture.h"
#include "util.h"

namespace nvogl {
const int MAX_QUAD_COUNT = 200;
const int MAXIMUM_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
const int MAXIMUM_INDEX_COUNT = MAX_QUAD_COUNT * 6;

// Index 0 points to the first white or blank texture
const int NO_TEXTURE = 0;
const int MAX_TEXTURE_UNIT_COUNT = 32;

GLRenderer::GLRenderer(int width, int height) : m_Width(width), m_Height(height)
{
    // m_PrimitiveShader = GLShader::LoadPrimitiveDefault();
    m_TextureShader = GLShader::LoadTextureDefault();
    textureSetup();
}

GLRenderer::~GLRenderer()
{
    delete[] rd.verts;
    delete[] rd.textureSlots;
}

glm::vec3 GLRenderer::vec3ToNDC(glm::vec3 v)
{
    return glm::vec3(v.x / (float)m_Width * 2 - 1, -(v.y / (float)m_Height * 2 - 1), v.z);
}

void GLRenderer::DrawTexture(GLTexture tex, int x, int y, int w, int h)
{
    if (rd.indexCount >= MAXIMUM_INDEX_COUNT || rd.textureNextSlot > MAX_TEXTURE_UNIT_COUNT - 1) {
        batchEnd();
        batchFlush();
        batchBegin();
    }

    float texture_index = 0.0f;
    for (size_t i = 1; i < rd.textureNextSlot; i++) {
        if (rd.textureSlots[i] == (int)tex.GetId()) {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f) {
        texture_index = (float)rd.textureNextSlot;
        rd.textureSlots[rd.textureNextSlot] = tex.GetId();
        // rd.textureSlots[rd.textureNextSlot] = rd.textureNextSlot;
        rd.textureNextSlot++;
    }

    Color c = Color::White();
    glm::vec4 norm = glm::vec4(c.R / 255.0f, c.G / 255.0f, c.B / 255.0f, c.A / 255.0f);
    size_t *size = &rd.vertCount;

    glm::vec3 topLeft = vec3ToNDC({x, y, 0.0f});
    glm::vec3 botLeft = vec3ToNDC({x, y + h, 0.0f});
    glm::vec3 topRight = vec3ToNDC({x + w, y, 0.0f});
    glm::vec3 botRight = vec3ToNDC({x + w, y + h, 0.0f});

    LogDebug("Rect:");
    LogDebug("\t Top left: (%.1f, %.1f)", topLeft.x, topLeft.y);
    LogDebug("\t Bot left: (%.1f, %.1f)", botLeft.x, botLeft.y);
    LogDebug("\tTop right: (%.1f, %.1f)", topRight.x, topRight.y);
    LogDebug("\tBot right: (%.1f, %.1f)", botRight.x, botRight.y);

    // Top left corner
    rd.verts[(*size)++] = VertexInfo(topLeft, norm, {0.0f, 1.0f}, texture_index);
    // Bottom left corner
    rd.verts[(*size)++] = VertexInfo(botLeft, norm, {0.0f, 0.0f}, texture_index);
    // Bottom right corner
    rd.verts[(*size)++] = VertexInfo(botRight, norm, {1.0f, 0.0f}, texture_index);
    // Top right corner
    rd.verts[(*size)++] = VertexInfo(topRight, norm, {1.0f, 1.0f}, texture_index);
    rd.indexCount += 6;
    rd.stat.quadCounter++;
}

void GLRenderer::Clear(Color color)
{
    glClearColor((float)color.R / 255.0f, (float)color.G / 255.0f, (float)color.B / 255.0f,
                 (float)color.A / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::Present(GLWindow& window)
{
    if (rd.indexCount > 0) {
        batchEnd();
        batchFlush();
        batchBegin();
    }

    glfwSwapBuffers(window.GetWindowHandle());
    glfwPollEvents();
}

void GLRenderer::textureSetup()
{
    rd = RenderData();
    rd.quadVao = VAO::Create();
    VAO::Bind(rd.quadVao);

    rd.quadVbo = VBO::Create(VBOType::ArrayBuffer);
    VBO::SetData(rd.quadVbo, true, NULL, MAXIMUM_VERTEX_COUNT * sizeof(VertexInfo));
    VBO::Bind(rd.quadVbo);

    rd.verts = new VertexInfo[MAXIMUM_VERTEX_COUNT];

    // Fill indices
    int indices[MAXIMUM_INDEX_COUNT] = {0};
    int num = 0;
    for (int i = 0; i < MAXIMUM_INDEX_COUNT; i += 6) {
        indices[i + 0] = 0 + num;
        indices[i + 1] = 1 + num;
        indices[i + 2] = 2 + num;

        indices[i + 3] = 0 + num;
        indices[i + 4] = 2 + num;
        indices[i + 5] = 3 + num;
        num += 4;
    }
    glGenBuffers(1, &rd.quadIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rd.quadIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAXIMUM_INDEX_COUNT * sizeof(int), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
                          (void *)offsetof(VertexInfo, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
                          (void *)offsetof(VertexInfo, Color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
                          (void *)offsetof(VertexInfo, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
                          (void *)offsetof(VertexInfo, TextureIndex));
    glEnableVertexAttribArray(3);

    // Textures

    // NOTE: this block queries the GPU for the maximum texture unit limit on
    // the current device
    // TODO: This should be used instead of the `MAX_TEXTURE_UNIT_COUNT`
    // constant once variable length arrays has been figured out on the fragment
    // shader side.
    // ------------------------------
    // int units;
    // glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &units);
    // LogInfo("Maximum texture limit: %d", units);

    rd.textureSlots = new int[MAX_TEXTURE_UNIT_COUNT];

    GLuint whiteTex;
    glGenTextures(1, &whiteTex);
    glBindTexture(GL_TEXTURE_2D, whiteTex);
    // set the texture wrapping parameters
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    glGenerateMipmap(GL_TEXTURE_2D);

    rd.textureSlots[NO_TEXTURE] = whiteTex;
    for (int i = 1; i < MAX_TEXTURE_UNIT_COUNT; i++)
        rd.textureSlots[i] = 0;
    rd.textureNextSlot = 1;

    m_TextureShader.Bind();
    LogInfo("Shader (id = %d) bound!", m_TextureShader.GetProgramId());

    int samplers[MAX_TEXTURE_UNIT_COUNT];
    for (int i = 0; i < MAX_TEXTURE_UNIT_COUNT; i++)
        samplers[i] = i;
    // TODO: passing uniform should be implemented into the shader class
    int loc = glGetUniformLocation(m_TextureShader.GetProgramId(), "u_Textures");
    glUniform1iv(loc, MAX_TEXTURE_UNIT_COUNT, samplers);
}

void GLRenderer::batchBegin()
{
    rd.vertCount = 0;
}

void GLRenderer::batchFlush()
{
    for (size_t i = 0; i < rd.textureNextSlot; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, rd.textureSlots[i]);
        /* Opengl 4.0+ approach */
        // glBindTextureUnit(i, s);
    }

    VAO::Bind(rd.quadVao);
    glDrawElements(GL_TRIANGLES, rd.indexCount, GL_UNSIGNED_INT, nullptr);
    // Update needed stats
    rd.stat.drawCounter++;

    // Reset after flushing
    rd.indexCount = 0;
    rd.textureNextSlot = 1;
}

void GLRenderer::batchEnd()
{
    VBO::Bind(rd.quadVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, rd.vertCount * sizeof(VertexInfo), rd.verts);
}

VertexInfo::VertexInfo(glm::vec3 pos, glm::vec4 color, glm::vec2 tex_coords, float texture_index)
    : Position(pos), Color(color), TexCoords(tex_coords), TextureIndex(texture_index)
{
}

} // namespace nvogl
