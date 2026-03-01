#pragma once

#include <memory>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

namespace resource {

struct font_loader {
    using result_type = std::shared_ptr<raylib::Font>;

    result_type operator()(const std::string& name, int font_size) const {
        const auto path = std::string("data/fonts/") + name + ".ttf";
        return std::make_shared<raylib::Font>(path, font_size);
    }

    result_type operator()(
        const std::string& file_type,
        const unsigned char* file_data,
        int data_size,
        int font_size,
        int* font_chars = nullptr,
        int chars_count = 0) const {
        return std::make_shared<raylib::Font>(
            file_type,
            file_data,
            data_size,
            font_size,
            font_chars,
            chars_count);
    }
};

typedef entt::resource_cache<raylib::Font, font_loader> font_cache;

namespace fonts {

constexpr auto dungeon_mode = entt::hashed_string{"dungeon-mode"};
constexpr auto dungeon_mode_inverted = entt::hashed_string{"dungeon-mode-inverted"};
constexpr auto monogram = entt::hashed_string{"monogram"};
constexpr auto monogram_extended = entt::hashed_string{"monogram-extended"};
constexpr auto monogram_extended_italic = entt::hashed_string{"monogram-extended-italic"};

};

};
