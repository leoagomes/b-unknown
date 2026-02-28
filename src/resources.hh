#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "raylib.h"

namespace resources {

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

struct sprite {
    entt::resource<texture> tex;
    Rectangle source;

    sprite(entt::resource<texture> tex, const Rectangle &source)
        : tex(tex), source(source) {
    }

    ~sprite() {}
};

struct sprite_loader {
    using result_type = std::shared_ptr<sprite>;

    result_type operator()(entt::resource<texture> tex, const Rectangle &source) const {
        return std::make_shared<sprite>(tex, source);
    }
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

    void draw_text(const std::string &text, const Vector2 &position, float font_size, float spacing, const Color &color) const {
        DrawTextEx(fnt, text.c_str(), position, font_size, spacing, color);
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

    result_type operator()(const std::string &path) const {
        return std::make_shared<font>(path);
    }
};

struct shader {
    std::string path;
    Shader shdr;

    shader(const std::string &path) : path(path) {
        shdr = LoadShader(path.c_str(), path.c_str());
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

};
