#pragma once

#include <memory>

#include "entt/entt.hpp"

#include "scene/manager.hh"

#include "resource/font.hh"
#include "resource/image.hh"
#include "resource/shader.hh"
#include "resource/sound.hh"
#include "resource/sprite.hh"
#include "resource/texture.hh"

#include "shaders/crt.hh"

namespace global {

struct shared_resources {
    resource::font_cache font_cache;
    resource::image_cache image_cache;
    resource::shader_cache shader_cache;
    resource::sound_cache sound_cache;
    resource::sprite_cache sprite_cache;
    resource::texture_cache tex_cache;

    std::shared_ptr<shaders::crt> crt_shader = nullptr;
};

extern shared_resources resources;
extern scene::manager scene;

};
