#pragma once

#include "vt_game_object.hpp"
#include "vt_window.hpp"

namespace vt {
    class KeyboardMovementController {
        public:
            struct keyMapping {
                int moveLeft = GLFW_KEY_A;
                int moveRight = GLFW_KEY_D;
                int moveForward = GLFW_KEY_W;
                int moveBackward = GLFW_KEY_S;
                int moveUp = GLFW_KEY_SPACE;
                int moveDown = GLFW_KEY_LEFT_SHIFT;
                int lookLeft = GLFW_KEY_LEFT;
                int lookRight = GLFW_KEY_RIGHT;
                int lookUp = GLFW_KEY_UP;
                int lookDown = GLFW_KEY_DOWN;

            };

            void movePlaneXZ(GLFWwindow *window, float dt, VtGameObject &gameObject);

            keyMapping keys{};
            float moveSpeed{3.f};
            float lookSpeed{1.5f};
    };
}