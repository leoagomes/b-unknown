#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resources {

struct texture {
    Texture2D tex2d;
    std::string path;

    texture(const std::string &path) : path(path) {
        tex2d = LoadTexture(path.c_str());
    }

    texture(const Image &img) {
        tex2d = LoadTextureFromImage(img);
    }

    ~texture() {
        UnloadTexture(tex2d);
    }
};

struct texture_loader {
    using result_type = std::shared_ptr<texture>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<texture>(path);
    }
};

typedef entt::resource_cache<texture, texture_loader> texture_cache;

};
