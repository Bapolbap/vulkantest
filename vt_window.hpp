#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace vt {

    class VtWindow {
        public:
            VtWindow(int width, int height, std::string name);
            ~VtWindow();

            VtWindow(const VtWindow &) = delete;
            VtWindow &operator=(const VtWindow &) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); }

            VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

            bool wasWindowResized() { return frameBufferResized; }
            void resetWindowResizeFlag() { frameBufferResized = false; }
            GLFWwindow *getGLFWWindow() const { return window; }

            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        private:
            static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
            void initWindow();

            int width;
            int height;
            bool frameBufferResized = false;

            std::string windowName;
            GLFWwindow *window;
    };
}