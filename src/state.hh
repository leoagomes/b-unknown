#pragma once

#include <memory>

#include "entt/entt.hpp"

#include "resource/image.hh"
#include "resource/texture.hh"
#include "resource/sprite.hh"
#include "resource/font.hh"
#include "resource/shader.hh"

#include "shaders/crt.hh"

namespace state {

struct shared_resources {
    resource::image_cache image_cache;
    resource::texture_cache tex_cache;
    resource::sprite_cache sprite_cache;
    resource::font_cache font_cache;
    resource::shader_cache shader_cache;

    std::shared_ptr<shaders::crt> crt_shader = nullptr;
};

};
