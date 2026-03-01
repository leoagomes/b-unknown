#include "scene/boot.hh"

#include "colors.hh"
#include "global.hh"

namespace scene {

boot::~boot() {
    if (boot_sound_loaded) {
        UnloadSound(boot_sound);
        boot_sound_loaded = false;
    }
}

void boot::draw_cube(Vector3 position, float rotation) {
    BeginMode3D(camera);
        rlPushMatrix();
            rlTranslatef(position.x, position.y, position.z);
            rlRotatef(rotation * 60.0f, 0.0f, 1.0f, 0.0f);
            rlRotatef(rotation * 30.0f, 1.0f, 0.0f, 0.0f);
            DrawCubeWires(Vector3{0, 0, 0}, 1, 1, 1, colors::white);
        rlPopMatrix();
    EndMode3D();
}

void boot::initialize() {
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
    global::resources.font_cache.load(resource::fonts::dungeon_mode, "dungeon-mode", 32);
    global::resources.font_cache.load(resource::fonts::monogram, "monogram", 32);
    boot_state = boot_state::loading;
}

void boot::update(float dt) {
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

void boot::draw() {
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

    auto crt_shader = global::resources.crt_shader;
    BeginDrawing();
        ClearBackground(colors::black);
        BeginShaderMode(crt_shader->shader);
            DrawTextureRec(render_texture.texture, Rectangle{0, 0, (float)render_texture.texture.width, (float)-render_texture.texture.height}, Vector2{0, 0}, colors::white);
        EndShaderMode();
    EndDrawing();
}

void boot::draw_loading() {
    auto font =
        global::resources.font_cache[resource::fonts::monogram];

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

void boot::draw_loaded() {
    auto font =
        global::resources.font_cache[resource::fonts::monogram];

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
