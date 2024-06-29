#pragma once

#include "util.h"

namespace Velocity {

class GLImage {
  public:
    int GetWidth() const;
    int GetHeight() const;
    Color GetColor(int x, int y) const;
    Color *GetData() const;

    static GLImage LoadFromFile(const char *path);
    static GLImage FromColor(int width, int height, Color color);
    static void Unload(GLImage& img);

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

    unsigned int GetId() const;
    int GetWidth() const;
    int GetHeight() const;

    void Unload();

  private:

    void deleteTex() const;
    void Load(GLImage img);

    unsigned int m_Id;
    int m_Width;
    int m_Height;
};

} // namespace Velocity
