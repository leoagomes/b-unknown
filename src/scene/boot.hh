#pragma once

#include <atomic>
#include <thread>
#include <string>

#include <entt/entt.hpp>
#include <raylib.h>
#include <rlgl.h>

#include "colors.hh"
#include "global.hh"
#include "resource/image.hh"
#include "scene/base.hh"

namespace scene {

/* # Boot scene
 *
 * The boot scene's purpose is to preload all required resources for menu and
 * game scenes -- while displaying something for the user, obviously.
 *
 * It uses threading keep the UI responsive as much as possible, so it has to be
 * carefully crafted.
 *
 * When first initialized, it loads only required resources for displaying the
 * scene itself, sequentially, blocking the main thread. **It is important to
 * keep the boot scene's required resources to a minimum.**
 *
 * After loading the bare minimum resources, it spawns a thread to load all
 * other resources that _can_ be loaded in a separate thread. Because OpenGL
 * requires textures are loaded from the main thread, we'll only load images and
 * sound. While the auxiliary thread is running, the boot scene **must not**
 * touch any resource caches. EnTT resource caches are **not** thread-safe.
 * Resource loading progress is reported through an atomic variable.
 *
 * Once all CPU resources are pre-loaded, the boot scene joins the auxiliary
 * thread and proceeds to load GPU-related resources -- Textures, mostly. At
 * which point, it once again accesses resource caches.
 *
 * After all of that is done, the final boot sequence animation happens, the
 * boot chime is played, and we switch to the menu scene.
 */
class boot : public base {
private:
    // A simple wrapper for rendering the spinning wireframe cube.
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
                    DrawCubeWires(
                        Vector3{0, 0, 0},
                        1, 1, 1,
                        colors::white
                    );
                rlPopMatrix();
            EndMode3D();
        }
    };

    // A wrapper for the parallel loading logic.
    struct parallel_loader {
        // names of the assets to load
        static constexpr const char* boot_sound_name = "boot";
        static constexpr const char* one_bit_input_pack_name = "1-bit-input-pack";
        static constexpr const char* one_bit_pack_name = "1-bit-pack";
        static constexpr const char* dungeon_mode_name = "dungeon-mode";
        static constexpr const char* dungeon_437_name = "dungeon-437";
        static constexpr const char* monogram_name = "monogram";

        // total number of assets to load
        static constexpr int total_assets = 9;
        // how many assets have been loaded
        std::atomic<int> progress;
        // whether the loading is done
        std::atomic<bool> done;

        // the underlying thread for loading the assets
        std::thread thread;

        // The parallel loader just loads this data into RAM, the main thread
        // uses it to populate the structures.
        unsigned char* dungeon_mode_font_data = nullptr;
        int dungeon_mode_font_data_size = 0;
        unsigned char* monogram_extended_font_data = nullptr;
        int monogram_extended_font_data_size = 0;
        unsigned char* monogram_extended_italic_font_data = nullptr;
        int monogram_extended_italic_font_data_size = 0;

        parallel_loader()
            : progress(0),
              done(false) {}

        ~parallel_loader() {
            if (dungeon_mode_font_data)
                UnloadFileData(dungeon_mode_font_data);
            if (monogram_extended_font_data)
                UnloadFileData(monogram_extended_font_data);
            if (monogram_extended_italic_font_data)
                UnloadFileData(monogram_extended_italic_font_data);
        }

        void load_cpu_assets() {
            done.store(false);
            progress.store(0);

            // to simplify setting the progress, we use a local variable
            int current_progress = 0;

            // load the boot chime
            global::resources.sound_cache.load(
                resource::sounds::boot_chime,
                boot_sound_name
            );
            progress.store(++current_progress);

            // load the sprite atlases
            auto& image_cache = global::resources.image_cache;
            image_cache.load(
                resource::images::one_bit_input_pack,
                one_bit_input_pack_name
            );
            progress.store(++current_progress);
            image_cache.load(resource::images::one_bit_pack, one_bit_pack_name);
            progress.store(++current_progress);
            image_cache.load(resource::images::dungeon_mode, dungeon_mode_name);
            progress.store(++current_progress);
            image_cache.load(resource::images::dungeon_437, dungeon_437_name);
            progress.store(++current_progress);
            image_cache.load(resource::images::monogram, monogram_name);
            progress.store(++current_progress);

            // GPU textures need to be loaded from the main thread, however, we can
            // expedite the loading process by reading the data from disk into RAM.
            // Then, we can load the textures from RAM, instead of from disk, saving
            // time on the blocking loads.
            // This is partily what we're doing with the sprite atlases, they just
            // have special representations for images. The data below this comment,
            // however, need to be represented as a blob of bytes.
            monogram_extended_font_data = LoadFileData(
                "data/fonts/monogram-extended.ttf",
                &monogram_extended_font_data_size
            );
            progress.store(++current_progress);
            monogram_extended_italic_font_data = LoadFileData(
                "data/fonts/monogram-extended-italic.ttf",
                &monogram_extended_italic_font_data_size
            );
            progress.store(++current_progress);
            dungeon_mode_font_data = LoadFileData(
                "data/fonts/dungeon-mode.ttf",
                &dungeon_mode_font_data_size
            );
            progress.store(++current_progress);

            done.store(true);
        }

        void load_gpu_assets() {
            auto& font_cache = global::resources.font_cache;
            font_cache.load(
                resource::fonts::monogram_extended,
                "ttf", monogram_extended_font_data,
                monogram_extended_font_data_size,
                32
            );
            font_cache.load(
                resource::fonts::monogram_extended_italic,
                "ttf",
                monogram_extended_italic_font_data,
                monogram_extended_italic_font_data_size,
                32
            );
            font_cache.load(
                resource::fonts::dungeon_mode,
                "ttf",
                dungeon_mode_font_data,
                dungeon_mode_font_data_size,
                32
            );
        }

        void load_cpu_assets_in_background() {
#if defined(PLATFORM_WEB)
            // On Web, avoid blocking joins on the main loop thread.
            load_cpu_assets();
#else
            if (thread.joinable())
                thread.join();
            thread = std::thread([this]() { load_cpu_assets(); });
#endif
        }
    };

    enum class boot_state {
        instantiate,      // the boot scene instance has just been instantiated
        load_core_assets, // loading core assets, required for boot scene display
        load_cpu_assets,  // loading assets (in parallel)
        load_gpu_assets,  // loading GPU assets from the main thread
        loaded,           // done
    };

    cube_renderer cube;
    boot_state state;
    float total_delta;

    parallel_loader loader;

    RenderTexture render_texture;
    bool render_texture_loaded = false;
    entt::resource<raylib::Sound> boot_chime;
    bool played_boot_chime = false;

    entt::resource<raylib::Font> font;
    static constexpr int font_size = 32;
    static constexpr int font_spacing = 0;

    int screen_width = 0;
    int screen_height = 0;

    void load_core_assets() {
        state = boot_state::load_core_assets;

        // load the core boot font
        auto ret =
            global::resources.font_cache.load(
                resource::fonts::monogram,
                "monogram",
                32
            );
        if (!ret.second)
            throw std::runtime_error("Failed to load font: monogram");
        // store it so we don't need to access the cache
        font = ret.first->second;
    }

public:
    boot()
        : cube(),
          state(boot_state::instantiate),
          total_delta(0.0f),
          loader() {}

    ~boot() override {
#if !defined(PLATFORM_WEB)
        if (loader.thread.joinable())
            loader.thread.join();
#endif
        if (render_texture_loaded)
            UnloadRenderTexture(render_texture);
    }

    void initialize() override {
        total_delta = 0.0f;
        played_boot_chime = false;
        load_core_assets();

        render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        render_texture_loaded = true;
        state = boot_state::load_cpu_assets;
        loader.load_cpu_assets_in_background();
    }

    void update(float dt) override {
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        total_delta += dt;
        if (state == boot_state::load_cpu_assets) {
            if (loader.done.load()) {
#if !defined(PLATFORM_WEB)
                if (loader.thread.joinable())
                    loader.thread.join();
#endif
                state = boot_state::load_gpu_assets;
            }
        }

        if (state == boot_state::load_gpu_assets) {
            loader.load_gpu_assets();
            boot_chime = global::resources.sound_cache[resource::sounds::boot_chime];
            state = boot_state::loaded;
            total_delta = 0.0f;
        }

        if (state == boot_state::loaded && !played_boot_chime) {
            if (IsAudioDeviceReady() && boot_chime) {
                boot_chime->Play();
            }
            played_boot_chime = true;
        }

        if (state == boot_state::loaded) {
            cube.position.z += dt * 20.0f;
        }
    }

    void draw() override {
        auto width = GetScreenWidth();
        auto height = GetScreenHeight();

        BeginTextureMode(render_texture);
            ClearBackground(colors::black);
            float scale = state == boot_state::loaded ? 10.0f : 1.0f;
            auto rotation = total_delta * 10.0f;
            cube.draw(rotation);
            // draw the heading
            const char* heading_text = "OS II";
            auto heading_length = this->font->MeasureText(
                heading_text,
                static_cast<float>(this->font->baseSize),
                font_spacing
            );
            auto heading_position = Vector2{
                (screen_width - heading_length.x) / 2,
                20
            };
            this->font->DrawText(
                heading_text,
                heading_position,
                font_size,
                font_spacing,
                colors::white
            );
            switch (state) {
                case boot_state::load_cpu_assets: {
                    this->font->DrawText(
                        "Loading system...",
                        Vector2{20, (float)height - 32 - 50},
                        font_size,
                        font_spacing,
                        colors::white);
                    int progress = loader.progress.load();
                    if (progress > parallel_loader::total_assets)
                        progress = parallel_loader::total_assets;
                    const std::string progress_text = "Progress: "
                        + std::to_string(progress)
                        + "/"
                        + std::to_string(parallel_loader::total_assets);
                    this->font->DrawText(
                        progress_text.c_str(),
                        Vector2{20, (float)height - 32 - 16},
                        font_size,
                        font_spacing,
                        colors::white);
                    break;
                }
                case boot_state::load_gpu_assets: {
                    this->font->DrawText(
                        "Uploading GPU assets...",
                        Vector2{20, (float)height - 32 - 50},
                        font_size,
                        font_spacing,
                        colors::white);
                    break;
                }
                case boot_state::loaded: {
                    this->font->DrawText(
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
};

};
