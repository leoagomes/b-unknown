#pragma once

#include <memory>

#include "entt/entt.hpp"
#include <raylib-cpp.hpp>

namespace resource {

struct image_loader {
    using result_type = std::shared_ptr<raylib::Image>;

    result_type operator()(const std::string& name) const {
        const auto path = std::string("data/images/") + name + ".png";
        return std::make_shared<raylib::Image>(path);
    }

    result_type operator()(const std::string& file_type, const unsigned char* file_data, int data_size) const {
        return std::make_shared<raylib::Image>(file_type, file_data, data_size);
    }
};

typedef entt::resource_cache<raylib::Image, image_loader> image_cache;

namespace images {

constexpr auto one_bit_pack = entt::hashed_string{"one_bit_pack"};
constexpr auto one_bit_input_pack = entt::hashed_string{"one_bit_input_pack"};
constexpr auto dungeon_mode = entt::hashed_string{"dungeon_mode"};
constexpr auto dungeon_437 = entt::hashed_string{"dungeon_437"};
constexpr auto monogram = entt::hashed_string{"monogram"};

};

};

