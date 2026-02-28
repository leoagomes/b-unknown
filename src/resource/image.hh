#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resource {

struct image {
    std::string path;
    Image img;

    image(const std::string &path) : path(path) {
        img = LoadImage(path.c_str());
    }
};

struct image_loader {
    using result_type = std::shared_ptr<image>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<image>(path);
    }
};

typedef entt::resource_cache<image, image_loader> image_cache;

};

