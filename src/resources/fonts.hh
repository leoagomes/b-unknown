#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resources {

struct font {
    Font fnt;
    std::string name;

    font(const std::string &name, int font_size) : name(name) {
        const auto path = TextFormat("data/fonts/%s.ttf", name.c_str());
        fnt = LoadFontEx(path, font_size, nullptr, 0);
    }

    ~font() {
        UnloadFont(fnt);
    }

    void draw_text(const std::string &text, const Vector2 &position, float font_size, float spacing, const Color &color) const {
        DrawTextEx(fnt, text.c_str(), position, font_size, spacing, color);
    }

    void draw_text(const std::string &text, const Vector2 &position, const Color &color) const {
        draw_text(text, position, fnt.baseSize, 0, color);
    }

    Vector2 measure_text(const std::string &text, float font_size, float spacing) const {
        return MeasureTextEx(fnt, text.c_str(), font_size, spacing);
    }

    Vector2 measure_text(const std::string &text) const {
        return measure_text(text, fnt.baseSize, 0);
    }
};

struct font_loader {
    using result_type = std::shared_ptr<font>;

    result_type operator()(const std::string &name, int font_size) const {
        return std::make_shared<font>(name, font_size);
    }
};

typedef entt::resource_cache<font, font_loader> font_cache;

namespace fonts {

constexpr auto dungeon_mode = entt::hashed_string{"dungeon-mode"};
constexpr auto dungeon_mode_inverted = entt::hashed_string{"dungeon-mode-inverted"};
constexpr auto monogram = entt::hashed_string{"monogram"};
constexpr auto monogram_extended = entt::hashed_string{"monogram-extended"};
constexpr auto monogram_extended_italic = entt::hashed_string{"monogram-extended-italic"};

};

};
