#pragma once

#include "shader.h"
#include "texture.h"
#include "window.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Velocity {

extern const int MAX_QUAD_COUNT;
extern const int MAXIMUM_VERTEX_COUNT;
extern const int MAXIMUM_INDEX_COUNT;
extern const int MAX_TEXTURE_UNIT_COUNT;

struct VertexInfo {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    // NOTE: this id is a float because GPUs prefer floats so this index is represented as a float
    // TODO: make this an integer and refactor all the places that refer to it
    float TextureIndex;

    VertexInfo() = default;
    VertexInfo(glm::vec3 pos, glm::vec4 color, glm::vec2 tex_coords, float texture_index);
};

struct RenderData {
    unsigned int quadVao, quadVbo, quadIbo;

    // Quad information
    VertexInfo *verts;
    size_t vertCount;
    size_t indexCount;

    // Texture information
    int *textureSlots;
    size_t textureNextSlot;

    struct RenderStats {
        uint32_t quadCounter;
        uint32_t drawCounter;
    } stat;
};

class GLRenderer {
  public:
    GLRenderer(int width, int height);
    ~GLRenderer();

    void DrawTexture(GLTexture tex, int x, int y, int w, int h);

    void Clear(Color color);
    void Present(GLWindow& window);

  private:
    void setup();
    void batchBegin();
    void batchEnd();
    void batchFlush();
    glm::vec3 vec3ToNDC(glm::vec3 vec);

    int m_Width;
    int m_Height;
    GLShader m_Shader;
    RenderData rd;
};
} // namespace Velocity
