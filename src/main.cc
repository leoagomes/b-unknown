#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "entt/entt.hpp"
#include "raylib.h"

#include "state.hh"
#include "scene/base.hh"
#include "scene/manager.hh"
#include "scene/boot.hh"

std::shared_ptr<state::shared_resources> shared_resources;

scene::manager scene_manager;

void update_draw_frame();

int main() {
    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "BUnknown");
    InitAudioDevice();
    SetTargetFPS(60);

    shared_resources = std::make_shared<state::shared_resources>();
    auto boot = std::make_shared<scene::boot>(shared_resources);
    scene_manager.push(boot);

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
    float dt = GetFrameTime();
    scene_manager.update_and_draw(dt);
}
