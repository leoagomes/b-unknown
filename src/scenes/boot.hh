#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"
#include "rlgl.h"

#include "scenes/scene.hh"
#include "state.hh"
#include "colors.hh"

namespace scenes {

class boot : public scene {
public:
    std::shared_ptr<state::shared_resources> shared_resources;

    float total_delta = 0.0f;
    Camera camera;

    boot(std::shared_ptr<state::shared_resources> shared_resources)
        : shared_resources(shared_resources) {
    }

    ~boot() {
    }

    void init() override {
        total_delta = 0.0f;
        camera = Camera{
            Vector3{0.0f, 0.0f, 10.0f},
            Vector3{0.0f, 0.0f, 0.0f},
            Vector3{0.0f, 1.0f, 0.0f},
            45.0f,
            CAMERA_PERSPECTIVE
        };
        // load the boot font
        shared_resources->font_cache.load(
            entt::hashed_string{"dungeon-mode"},
            "data/fonts/dungeon-mode.ttf");
        shared_resources->font_cache.load(
            entt::hashed_string{"monogram"},
            "data/fonts/monogram.ttf");
        // load the crt shader
    }

    void update(float dt) override {
        total_delta += dt;
    }

    void draw() override {
        auto font =
            shared_resources->font_cache[entt::hashed_string{"monogram"}];

        auto width = GetScreenWidth();
        auto height = GetScreenHeight();

        const float font_size = 32;
        const float font_spacing = 0;

        const char* heading_text = "OS II";
        auto heading_length = font->measure_text(heading_text);
        auto heading_position = Vector2{
            (width - heading_length.x) / 2,
            20
        };

        BeginDrawing();
            ClearBackground(colors::black);
            draw_cube();
            font->draw_text(
                heading_text,
                heading_position,
                font_size,
                font_spacing,
                colors::white);
            font->draw_text(
                "Loading system...",
                Vector2{20, height - 82 + heading_length.y},
                font_size,
                font_spacing,
                colors::white);
        EndDrawing();
    }

    void draw_cube() {
        BeginMode3D(camera);
            rlPushMatrix();
                rlRotatef(total_delta * 60.0f, 0.0f, 1.0f, 0.0f);
                rlRotatef(total_delta * 30.0f, 1.0f, 0.0f, 0.0f);
                DrawCubeWires(Vector3{0, 0, 0}, 1, 1, 1, colors::white);
            rlPopMatrix();
        EndMode3D();
    }
};

};
