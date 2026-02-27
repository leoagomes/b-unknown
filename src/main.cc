#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void update_draw_frame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello, World!", 10, 10, 20, BLACK);
    EndDrawing();
}

int main() {
    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "BUnknown");

    SetTargetFPS(60);

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
