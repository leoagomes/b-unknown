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

    void draw_cube(Vector3 position, float rotation);
    void draw_loading();
    void draw_loaded();

public:
    boot(std::shared_ptr<state::shared_resources> shared_resources);
    ~boot();

    void init() override;
    void update(float dt) override;
    void draw() override;
};

};
