#include "logger.h"
#include "texture.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Velocity {

GLImage::GLImage(Color *data, int width, int height)
    : m_Pixels(data), m_Width(width), m_Height(height) {
    LogInfo("Successfully created image with dimensions (%d x %d)", m_Width,
            m_Height);
}

GLImage GLImage::LoadFromFile(const char *path) {
    int width, height;
    stbi_uc *data = stbi_load(path, &width, &height, nullptr, STBI_rgb_alpha);
    if (data == nullptr) {
        LogError("Couldn't load image -> '%s'", path);
        return GLImage(nullptr, 0, 0);
    }
    Color *pixels = new Color[width * height];
    for (int i = 0; i < width * height; i++) {
        pixels[i] = Color(data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2],
                          data[i * 4 + 3]);
    }
    LogInfo("Loaded image from '%s'", path);

    stbi_image_free(data);

    return GLImage(pixels, width, height);
}

GLImage GLImage::FromColor(int width, int height, const Color color) {
    Color *data = new Color[width * height * 4];
    for (int i = 0; i < width * height; i++) {
        data[i] = color;
    }
    LogInfo("Initialized image with color -> (%d, %d, %d, %d)", color.R,
            color.G, color.B, color.A);

    return GLImage(data, width, height);
}

int GLImage::GetWidth() const { return m_Width; }

int GLImage::GetHeight() const { return m_Height; }

Color GLImage::GetColor(int x, int y) const {
    int i = y * m_Height + x;
    return m_Pixels[i];
}

Color *GLImage::GetData() const {
    return m_Pixels;
}

void GLImage::Unload(GLImage& img) { delete[] img.GetData(); }

GLTexture::GLTexture(const char *path) {
    GLImage img = GLImage::LoadFromFile(path);
    Load(img);
    GLImage::Unload(img);
}

GLTexture::GLTexture(GLImage img) {
    Load(img);
}

void GLTexture::Load(GLImage img) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetData());
    // glGenerateMipmap(GL_TEXTURE_2D);

    m_Id = textureID;
    m_Width = img.GetWidth();
    m_Height = img.GetHeight();
}

GLTexture::~GLTexture() { glDeleteTextures(1, &m_Id); }

int GLTexture::GetId() const { return m_Id; }

int GLTexture::GetWidth() const { return m_Width; }

int GLTexture::GetHeight() const { return m_Height; }

} // namespace Velocity
