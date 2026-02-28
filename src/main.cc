#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "entt/entt.hpp"
#include "raylib.h"

#include "state.hh"
#include "scenes/scene.hh"
#include "scenes/boot.hh"

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

// entt::registry registry;

std::shared_ptr<state::shared_resources> shared_resources;
std::shared_ptr<scenes::scene> scene;

void update_draw_frame();

int main() {
    shared_resources = std::make_shared<state::shared_resources>();
    scene = std::make_shared<scenes::boot>(shared_resources);

    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "BUnknown");
    SetTargetFPS(60);

    scene->init();

    // auto entity = registry.create();
    // registry.emplace<Position>(entity, 0.0f, 0.0f);
    // registry.emplace<Velocity>(entity, 1.0f, 1.0f);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_draw_frame, 60, 1);
#else
    while (!WindowShouldClose()) {
        update_draw_frame();
    }
#endif

    CloseWindow();
    return 0;
}

void update_draw_frame() {
    scene->update(GetFrameTime());
    scene->draw();
    // for (auto entity: registry.view<Position, Velocity>()) {
    //     auto &position = registry.get<Position>(entity);
    //     auto &velocity = registry.get<Velocity>(entity);
    //     position.x += velocity.x;
    //     position.y += velocity.y;
    // }
    // BeginDrawing();
    // ClearBackground(RAYWHITE);
    // DrawText("Hello, World!", 10, 10, 20, BLACK);
    // for (auto entity: registry.view<Position>()) {
    //     auto &position = registry.get<Position>(entity);
    //     DrawCircle(position.x, position.y, 10, RED);
    // }
    // EndDrawing();
}
