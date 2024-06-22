#include "renderer.h"
#include "window.h"

// TODO: this should be removed. it's bad to have this here.
using namespace Velocity;

int main() {
    GLWindowConfig config = GLWindowConfig(800, 600, "Testing");
    config.Resizable = true;

    GLWindow w = GLWindow(config);
    GLRenderer rend = GLRenderer(w.GetWidth(), w.GetHeight());

    float r = 0.0f;
    while (!w.WindowShouldClose()) {
        r += 0.0001;
        rend.Clear(Color(r, r, 1.0f, 1.0f));

        rend.Present(w);
    }

    return 0;
}
