#pragma once

#include "entt/entt.hpp"

#include "resources/images.hh"
#include "resources/textures.hh"
#include "resources/sprites.hh"
#include "resources/fonts.hh"
#include "resources/shaders.hh"

namespace state {

struct shared_resources {
    resources::image_cache image_cache;
    resources::texture_cache tex_cache;
    resources::sprite_cache sprite_cache;
    resources::font_cache font_cache;
    resources::shader_cache shader_cache;
};

};
