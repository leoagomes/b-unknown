#pragma once

#include <memory>
#include <vector>

#include "scene/base.hh"

namespace scene {

struct manager {
private:
    std::vector<std::shared_ptr<base>> stack;

    void initialize_and_enter(std::shared_ptr<base>& scene) {
        if (!scene->is_initialized())
            scene->initialize();
        scene->on_enter();
    }

public:
    manager() : stack() {}
    ~manager() {}

    std::shared_ptr<base> current() const {
        if (stack.empty())
            return nullptr;
        return stack.back();
    }

    void push(std::shared_ptr<base> scene) {
        if (!scene) return;
        stack.push_back(scene);
        if (!scene->is_initialized())
            scene->initialize();
        scene->on_enter();
    }

    void pop() {
        auto previous = current();
        if (previous) {
            previous->on_leave();
            stack.pop_back();
        }
        auto curr = current();
        if (curr)
            curr->on_resume();
    }

    void switch_to(std::shared_ptr<base> scene) {
        auto previous = current();
        if (previous) {
            previous->on_leave();
            stack.pop_back();
        }
        stack.push_back(scene);
        initialize_and_enter(scene);
    }

    void update_and_draw(float dt) {
        auto curr = current();
        if (!curr) return;
        curr->update(dt);
        curr->draw();
    }
};

};
