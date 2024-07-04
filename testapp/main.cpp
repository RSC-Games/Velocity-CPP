#include "renderer.h"
#include "texture.h"
#include "window.h"

// TODO: this should be removed. it's bad to have this here.
using namespace nvogl;

int main() {
    GLWindowConfig config = GLWindowConfig(800, 600, "Testing");
    config.Resizable = false;

    GLWindow window = GLWindow(config);
    GLRenderer rend = GLRenderer(window.GetWidth(), window.GetHeight());

    GLImage img = GLImage::LoadFromFile("../resources/images/imageA.png");
    GLTexture tex = GLTexture(img);
    GLImage::Unload(img);

    while (!window.WindowShouldClose()) {
        rend.Clear(Color(30, 30, 30, 255));

        int width = 50, height = 50;
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 10; c++) {
                rend.DrawTexture(tex, 25 + c * (width + width / 2), 25 + r * (height + height / 2), width, height);
            }
        }

        rend.Present(window);
    }

    tex.Unload();

    return 0;
}
