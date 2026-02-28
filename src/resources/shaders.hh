#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resources {

struct shader {
    std::string name;
    Shader shdr;

    shader(const std::string &name) : name(name) {
        // currently, fragment shader only
        shdr = LoadShader(0, name.c_str());
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
