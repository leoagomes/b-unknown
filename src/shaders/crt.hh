#pragma once

#include "raylib.h"

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
    Shader shader;

    crt() {
        const auto path = TextFormat("data/shaders/glsl-%d/crt.glsl", GLSL_VERSION);
        shader = LoadShader(0, path);
        load_locations();
    }

    ~crt() {
        UnloadShader(shader);
    }

    crt& set_resolution(float width, float height) {
        float resolution_values[2] = {width, height};
        SetShaderValue(
            shader,
            resolution_location,
            resolution_values,
            SHADER_UNIFORM_VEC2
        );
        return *this;
    }

    crt& set_curvature(float curvature) {
        SetShaderValue(
            shader,
            curvature_location,
            &curvature,
            SHADER_UNIFORM_FLOAT
        );
        return *this;
    }
};

};
