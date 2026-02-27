#pragma once

namespace scenes {

class Scene {
public:
    virtual ~Scene() = default;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};

};
