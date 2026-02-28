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
    RenderTexture render_texture;

    float total_delta = 0.0f;
    Camera camera;

    int resolution_location;
    int curvature_location;

    boot(std::shared_ptr<state::shared_resources> shared_resources)
        : shared_resources(shared_resources) {
    }

    ~boot() {
    }

    void init() override {
        render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
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
            resources::fonts::dungeon_mode,
            "dungeon-mode", 32);
        shared_resources->font_cache.load(
            resources::fonts::monogram,
            "monogram", 32);
        // load the crt shader
        shared_resources->shader_cache.load(
            resources::shaders::crt,
            "crt");
        auto crt_shader = shared_resources->shader_cache[resources::shaders::crt];
        resolution_location = GetShaderLocation(crt_shader->shdr, "resolution");
        curvature_location = GetShaderLocation(crt_shader->shdr, "curvature");
        float resolution[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        SetShaderValue(crt_shader->shdr, resolution_location, resolution, SHADER_UNIFORM_VEC2);
        float curvature = 0.05f;
        SetShaderValue(crt_shader->shdr, curvature_location, &curvature, SHADER_UNIFORM_FLOAT);
    }

    void update(float dt) override {
        total_delta += dt;
    }

    void draw() override {
        auto font =
            shared_resources->font_cache[resources::fonts::monogram];

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

        BeginTextureMode(render_texture);
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
        EndTextureMode();

        auto crt_shader = shared_resources->shader_cache[resources::shaders::crt];
        BeginDrawing();
            ClearBackground(colors::black);
            BeginShaderMode(crt_shader->shdr);
                DrawTextureRec(render_texture.texture, Rectangle{0, 0, (float)render_texture.texture.width, (float)-render_texture.texture.height}, Vector2{0, 0}, colors::white);
            EndShaderMode();
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
