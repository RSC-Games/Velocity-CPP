#pragma once

#include "util.h"

namespace Velocity {

class GLImage {
  public:
    GLImage(const char *path);
    ~GLImage();

    int GetWidth() const;
    int GetHeight() const;
    Color GetColor(int x, int y) const;
    Color *GetData() const;

    static GLImage FromColor(int width, int height, Color color);

  private:
    GLImage(Color *data, int width, int height);

    Color *m_Pixels;
    int m_Width;
    int m_Height;
};

class GLTexture {
  public:
    GLTexture() = delete;
    GLTexture(const char *path);
    GLTexture(GLImage img);
    ~GLTexture();

    int GetWidth() const;
    int GetHeight() const;

  private:
    unsigned int m_Id;
    int m_Width;
    int m_Height;
};

} // namespace Velocity
