#pragma once

namespace scenes {

class scene {
public:
    virtual ~scene() = default;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};

};
