#pragma once

#include <memory>

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

namespace resource {

struct texture_loader {
    using result_type = std::shared_ptr<raylib::Texture>;

    result_type operator()(const std::string& path) const {
        return std::make_shared<raylib::Texture>(path);
    }

    result_type operator()(const raylib::Image& image) const {
        return std::make_shared<raylib::Texture>(image);
    }

    result_type operator()(const std::string& file_type, const unsigned char* file_data, int data_size) const {
        raylib::Image image(file_type, file_data, data_size);
        return std::make_shared<raylib::Texture>(image);
    }
};

typedef entt::resource_cache<raylib::Texture, texture_loader> texture_cache;

};
