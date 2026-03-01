#pragma once

#include <raylib-cpp.hpp>

#include "defs.hh"

namespace shaders {

struct crt {
private:
    int resolution_location;
    int curvature_location;

    void load_locations() {
        resolution_location = GetShaderLocation(shader, "resolution");
        curvature_location = GetShaderLocation(shader, "curvature");
    }

public:
    raylib::Shader shader;

    crt() {
        const auto path = "data/shaders/glsl-" + std::to_string(GLSL_VERSION) + "/crt.glsl";
        shader = raylib::Shader("", path);
        load_locations();
    }

    crt& set_resolution(float width, float height) {
        float resolution_values[2] = {width, height};
        shader.SetValue(resolution_location, resolution_values, SHADER_UNIFORM_VEC2);
        return *this;
    }

    crt& set_curvature(float curvature) {
        shader.SetValue(curvature_location, &curvature, SHADER_UNIFORM_FLOAT);
        return *this;
    }
};

};
