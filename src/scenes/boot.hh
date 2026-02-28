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
private:
    std::shared_ptr<state::shared_resources> shared_resources;
    RenderTexture render_texture;
    Sound boot_sound = {};
    bool boot_sound_loaded = false;

    enum class boot_state {
        initializing,
        loading,
        loaded,
        finished,
    };

    boot_state boot_state = boot_state::initializing;

    float total_delta = 0.0f;
    Camera camera;

    Vector3 cube_position = Vector3{0, 0, 0};

    int resolution_location;
    int curvature_location;

    void draw_cube(Vector3 position, float rotation) {
        BeginMode3D(camera);
            rlPushMatrix();
                rlTranslatef(position.x, position.y, position.z);
                rlRotatef(rotation * 60.0f, 0.0f, 1.0f, 0.0f);
                rlRotatef(rotation * 30.0f, 1.0f, 0.0f, 0.0f);
                DrawCubeWires(Vector3{0, 0, 0}, 1, 1, 1, colors::white);
            rlPopMatrix();
        EndMode3D();
    }

public:
    boot(std::shared_ptr<state::shared_resources> shared_resources)
        : shared_resources(shared_resources) {
    }

    ~boot() {
        if (boot_sound_loaded) {
            UnloadSound(boot_sound);
            boot_sound_loaded = false;
        }
    }

    void init() override {
        boot_state = boot_state::initializing;
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
        boot_state = boot_state::loading;
    }

    void update(float dt) override {
        total_delta += dt;
        if (this->boot_state == boot_state::loading) {
            if (total_delta > 1.0f) {
                this->boot_state = boot_state::loaded;
                if (IsAudioDeviceReady()) {
                    boot_sound = LoadSound("data/sounds/boot.wav");
                    boot_sound_loaded = (boot_sound.frameCount > 0);
                    if (boot_sound_loaded) {
                        PlaySound(boot_sound);
                    }
                }
            }
        }

        if (this->boot_state == boot_state::loaded) {
            cube_position.z += dt * 20.0f;
        }
    }

    void draw() override {
        switch (this->boot_state) {
            case boot_state::loading:
                draw_loading();
                break;
            case boot_state::loaded:
                draw_loaded();
                break;
            default:
                break;
        }

        auto crt_shader = shared_resources->shader_cache[resources::shaders::crt];
        BeginDrawing();
            ClearBackground(colors::black);
            BeginShaderMode(crt_shader->shdr);
                DrawTextureRec(render_texture.texture, Rectangle{0, 0, (float)render_texture.texture.width, (float)-render_texture.texture.height}, Vector2{0, 0}, colors::white);
            EndShaderMode();
        EndDrawing();
    }

private:

    void draw_loading() {
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
            draw_cube(cube_position, total_delta);
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
    }

    void draw_loaded() {
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
            draw_cube(cube_position, total_delta * 10.0f);
            font->draw_text(
                heading_text,
                heading_position,
                font_size,
                font_spacing,
                colors::white);
            font->draw_text(
                "Loaded",
                Vector2{20, height - 82 + heading_length.y},
                font_size,
                font_spacing,
                colors::white);
        EndTextureMode();
    }
};

};
