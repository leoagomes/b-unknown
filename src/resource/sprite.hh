#pragma once

#include <memory>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

namespace resource {

struct sprite {
    entt::resource<raylib::Texture> tex;
    Rectangle source;

    sprite(entt::resource<raylib::Texture> tex, const Rectangle &source)
        : tex(tex), source(source) {
    }

    ~sprite() {}
};

struct sprite_loader {
    using result_type = std::shared_ptr<sprite>;

    result_type operator()(entt::resource<raylib::Texture> tex, const Rectangle &source) const {
        return std::make_shared<sprite>(tex, source);
    }
};

typedef entt::resource_cache<sprite, sprite_loader> sprite_cache;

};
