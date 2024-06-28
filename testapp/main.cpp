#include "renderer.h"
#include "texture.h"
#include "window.h"

// TODO: this should be removed. it's bad to have this here.
using namespace Velocity;

int main() {
    GLWindowConfig config = GLWindowConfig(800, 600, "Testing");
    config.Resizable = false;

    GLWindow w = GLWindow(config);
    GLRenderer rend = GLRenderer(w.GetWidth(), w.GetHeight());

    // GLImage img = GLImage::FromColor(100, 100, Color::Orange());
    GLImage img = GLImage::LoadFromFile("../resources/images/logo-256.png");
    GLTexture tex = GLTexture(img);
    GLImage::Unload(img);

    while (!w.WindowShouldClose()) {
        rend.Clear(Color(30, 30, 30, 255));
        rend.DrawTexture(tex, 100, 100, tex.GetWidth(), tex.GetHeight());
        rend.DrawTexture(tex, 500, 100, tex.GetWidth(), tex.GetHeight());

        rend.Present(w);
    }

    return 0;
}
