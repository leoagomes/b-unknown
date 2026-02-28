#pragma once

namespace scene {

class base {
private:
    bool initialized = false;

public:
    virtual ~base() = default;
    virtual void initialize() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;

    virtual void on_enter() {}
    virtual void on_leave() {}
    virtual void on_resume() {}

    bool is_initialized() const { return initialized; }
};

};
