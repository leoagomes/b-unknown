#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resources {

struct shader {
    std::string path;
    Shader shdr;

    shader(const std::string &path) : path(path) {
        // currently, fragment shader only
        shdr = LoadShader(0, path.c_str());
    }

    ~shader() {
        UnloadShader(shdr);
    }
};

struct shader_loader {
    using result_type = std::shared_ptr<shader>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<shader>(path);
    }
};

typedef entt::resource_cache<shader, shader_loader> shader_cache;

};
