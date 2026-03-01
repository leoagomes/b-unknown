#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "entt/entt.hpp"
#include "raylib.h"

#include "global.hh"
#include "scene/manager.hh"
#include "scene/boot.hh"

global::shared_resources global::resources;
scene::manager global::scene;

void update_draw_frame();
void load_crt_shader();

int main() {
    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "BUnknown");
    InitAudioDevice();
    SetTargetFPS(60);

    load_crt_shader();

    auto boot = std::make_shared<scene::boot>();
    global::scene.push(boot);

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
    global::scene.update_and_draw(dt);
}

void load_crt_shader() {
    auto crt_shader = std::make_shared<shaders::crt>();
    crt_shader->set_resolution(GetScreenWidth(), GetScreenHeight());
    crt_shader->set_curvature(0.05f);
    global::resources.crt_shader = crt_shader;
}
