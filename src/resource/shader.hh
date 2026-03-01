#pragma once

#include <memory>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "defs.hh"

namespace resource {

using shader = raylib::Shader;

struct shader_loader {
    using result_type = std::shared_ptr<shader>;

    result_type operator()(const std::string& name) const {
        const auto path = "data/shaders/glsl-" + std::to_string(GLSL_VERSION) + "/" + name + ".glsl";
        return std::make_shared<shader>("", path);
    }
};

typedef entt::resource_cache<shader, shader_loader> shader_cache;

namespace shaders {

constexpr auto crt = entt::hashed_string{"crt"};

};

};
