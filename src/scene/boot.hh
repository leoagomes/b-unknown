#pragma once

#include "entt/entt.hpp"
#include "raylib.h"
#include "rlgl.h"

#include "colors.hh"
#include "global.hh"
#include "scene/base.hh"

namespace scene {

class boot : public base {
private:
    struct cube_renderer {
        Camera camera;
        Vector3 position = Vector3{0, 0, 0};

        cube_renderer() {
            camera = Camera{
                Vector3{0.0f, 0.0f, 10.0f},
                Vector3{0.0f, 0.0f, 0.0f},
                Vector3{0.0f, 1.0f, 0.0f},
                45.0f,
                CAMERA_PERSPECTIVE
            };
        }

        void draw(float rotation) {
            BeginMode3D(camera);
                rlPushMatrix();
                    rlTranslatef(position.x, position.y, position.z);
                    rlRotatef(rotation * 60.0f, 0.0f, 1.0f, 0.0f);
                    rlRotatef(rotation * 30.0f, 1.0f, 0.0f, 0.0f);
                    DrawCubeWires(Vector3{0, 0, 0}, 1, 1, 1, colors::white);
                rlPopMatrix();
            EndMode3D();
        }
    };

    enum class boot_state {
        initializing,
        loading,
        loaded,
        finished,
    };

    RenderTexture render_texture;
    Sound boot_sound;

    cube_renderer cube;
    boot_state boot_state = boot_state::initializing;

    float total_delta = 0.0f;

    static constexpr int font_size = 32;
    static constexpr int font_spacing = 0;

    int screen_width = 0;
    int screen_height = 0;

    void draw_render_texture() {
        auto crt_shader = global::resources.crt_shader;
        BeginDrawing();
            ClearBackground(colors::black);
            BeginShaderMode(crt_shader->shader);
                DrawTextureRec(
                    render_texture.texture,
                    Rectangle{
                        0,
                        0,
                        (float)render_texture.texture.width,
                        (float)-render_texture.texture.height},
                    Vector2{0, 0},
                    colors::white);
            EndShaderMode();
        EndDrawing();
    }

    void draw_heading() {
        auto font =
            global::resources.font_cache[resource::fonts::monogram];
        const char* heading_text = "OS II";
        auto heading_length = font->measure_text(heading_text);
        auto heading_position = Vector2{
            (screen_width - heading_length.x) / 2,
            20
        };
        font->draw_text(
            heading_text,
            heading_position,
            font_size,
            font_spacing,
            colors::white
        );
    }

public:
    boot() : cube() {}

    ~boot() override {
        UnloadSound(boot_sound);
        UnloadRenderTexture(render_texture);
    }

    void initialize() override {
        boot_state = boot_state::initializing;
        render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        total_delta = 0.0f;
        // load the boot font
        global::resources.font_cache.load(resource::fonts::monogram, "monogram", 32);
        boot_state = boot_state::loading;
    }

    void update(float dt) override {
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        total_delta += dt;
        if (this->boot_state == boot_state::loading) {
            if (total_delta > 1.0f) {
                this->boot_state = boot_state::loaded;
                if (IsAudioDeviceReady()) {
                    boot_sound = LoadSound("data/sounds/boot.wav");
                    if (boot_sound.frameCount > 0) {
                        PlaySound(boot_sound);
                    }
                }
            }
        }

        if (this->boot_state == boot_state::loaded) {
            cube.position.z += dt * 20.0f;
        }
    }

    void draw() override {
        auto font =
            global::resources.font_cache[resource::fonts::monogram];

        auto width = GetScreenWidth();
        auto height = GetScreenHeight();

        BeginTextureMode(render_texture);
            ClearBackground(colors::black);
            float scale = this->boot_state == boot_state::loaded ? 10.0f : 1.0f;
            auto rotation = total_delta * 10.0f;
            cube.draw(rotation);
            draw_heading();
            switch (this->boot_state) {
                case boot_state::loading: {
                    font->draw_text(
                        "Loading system...",
                        Vector2{20, (float)height - 32 - 50},
                        font_size,
                        font_spacing,
                        colors::white);
                    break;
                }
                case boot_state::loaded: {
                    font->draw_text(
                        "Loaded",
                        Vector2{20, (float)height - 32 - 50},
                        font_size,
                        font_spacing,
                        colors::white);
                    break;
                }
                default:
                    break;
            }
        EndTextureMode();

        draw_render_texture();
    }
};

};
