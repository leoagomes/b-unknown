#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "entt/entt.hpp"
#include "raylib.h"

#include "state.hh"
#include "scene/base.hh"
#include "scene/boot.hh"

std::shared_ptr<state::shared_resources> shared_resources;
std::shared_ptr<scene::base> scene;

void update_draw_frame();

int main() {
    shared_resources = std::make_shared<state::shared_resources>();
    scene = std::make_shared<scene::boot>(shared_resources);

    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "BUnknown");
    InitAudioDevice();
    SetTargetFPS(60);

    scene->init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_draw_frame, 60, 1);
#else
    while (!WindowShouldClose()) {
        update_draw_frame();
    }
#endif

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void update_draw_frame() {
    scene->update(GetFrameTime());
    scene->draw();
}
