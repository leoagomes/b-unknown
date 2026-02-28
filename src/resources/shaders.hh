#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_WEB, PLATFORM_ANDROID
#define GLSL_VERSION 100
#endif

namespace resources {

struct shader {
    std::string name;
    Shader shdr;

    shader(const std::string &name) : name(name) {
        // currently, fragment shader only
        const auto path = TextFormat("data/shaders/glsl-%d/%s.glsl", GLSL_VERSION, name.c_str());
        shdr = LoadShader(0, path);
    }

    ~shader() {
        UnloadShader(shdr);
    }
};

struct shader_loader {
    using result_type = std::shared_ptr<shader>;

    result_type operator()(const std::string &name) const {
        return std::make_shared<shader>(name);
    }
};

typedef entt::resource_cache<shader, shader_loader> shader_cache;

namespace shaders {

constexpr auto crt = entt::hashed_string{"crt"};

};

};
