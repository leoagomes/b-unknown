#pragma once

#include "entt/entt.hpp"

#include "resources.hh"

namespace state {

struct shared_resources {
    entt::resource_cache<resources::image, resources::image_loader> image_cache;
    entt::resource_cache<resources::texture, resources::texture_loader> tex_cache;
    entt::resource_cache<resources::sprite, resources::sprite_loader> sprite_cache;
    entt::resource_cache<resources::font, resources::font_loader> font_cache;
};

};
