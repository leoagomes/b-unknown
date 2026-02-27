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

    ~texture() {
        UnloadTexture(tex2d);
    }
};

struct sprite {
    entt::resource<texture> tex;
    Rectangle source;

    sprite(entt::resource<texture> tex, const Rectangle &source)
        : tex(tex), source(source) {
    }

    ~sprite() {}
};

struct font {
    Font fnt;
    std::string path;

    font(const std::string &path) : path(path) {
        fnt = LoadFontEx(path.c_str(), 32, nullptr, 0);
    }

    ~font() {
        UnloadFont(fnt);
    }
};

struct texture_loader {
    using result_type = std::shared_ptr<texture>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<texture>(path);
    }
};

struct sprite_loader {
    using result_type = std::shared_ptr<sprite>;

    result_type operator()(entt::resource<texture> tex, const Rectangle &source) const {
        return std::make_shared<sprite>(tex, source);
    }
};

struct font_loader {
    using result_type = std::shared_ptr<font>;

    result_type operator()(const std::string &path) const {
        return std::make_shared<font>(path);
    }
};

};
