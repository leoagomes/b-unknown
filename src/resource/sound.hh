#pragma once

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

namespace resource {

struct sound_loader {
    using result_type = std::shared_ptr<raylib::Sound>;

    result_type operator()(const std::string& name) const {
        const auto path = std::string("data/sounds/") + name + ".wav";
        return std::make_shared<raylib::Sound>(path);
    }

    result_type operator()(const std::string& file_type, const unsigned char* file_data, int data_size) const {
        raylib::Wave wave(file_type, file_data, data_size);
        return std::make_shared<raylib::Sound>(wave);
    }
};

typedef entt::resource_cache<raylib::Sound, sound_loader> sound_cache;

namespace sounds {

constexpr auto boot_chime = entt::hashed_string{"boot_chime"};

};

};
