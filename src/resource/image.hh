#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resource {

struct image {
    std::string name;
    Image img;

    image(const std::string &name) : name(name) {
        const auto path = TextFormat("data/images/%s.png", name.c_str());
        img = LoadImage(path);
    }
};

struct image_loader {
    using result_type = std::shared_ptr<image>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<image>(path);
    }
};

typedef entt::resource_cache<image, image_loader> image_cache;

namespace images {

constexpr auto one_bit_pack = entt::hashed_string{"one_bit_pack"};
constexpr auto one_bit_input_pack = entt::hashed_string{"one_bit_input_pack"};
constexpr auto dungeon_mode = entt::hashed_string{"dungeon_mode"};
constexpr auto dungeon_437 = entt::hashed_string{"dungeon_437"};
constexpr auto monogram = entt::hashed_string{"monogram"};

};

};

