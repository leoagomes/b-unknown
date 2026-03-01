#pragma once

#include <entt/entt.hpp>
#include <raylib.h>

namespace resource {

struct sound {
    std::string name;
    Sound snd;

    sound(const std::string &name) : name(name) {
        const auto path = TextFormat("data/sounds/%s.wav", name.c_str());
        snd = LoadSound(path);
    }

    ~sound() {
        UnloadSound(snd);
    }
};

struct sound_loader {
    using result_type = std::shared_ptr<sound>;

    result_type operator()(const std::string &name) const {
        return std::make_shared<sound>(name);
    }
};

typedef entt::resource_cache<sound, sound_loader> sound_cache;

namespace sounds {

constexpr auto boot_chime = entt::hashed_string{"boot_chime"};

};

};
