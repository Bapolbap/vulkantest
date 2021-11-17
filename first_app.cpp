#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "vt_camera.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <stdexcept>
#include <array>
#include <cassert>

namespace vt {

    FirstApp::FirstApp() {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {

        SimpleRenderSystem simpleRenderSystem{vtDevice, vtRenderer.getSwapChainRenderPass()};
        VtCamera camera{};
        auto viewerObject = VtGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while(!vtWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.movePlaneXZ(vtWindow.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = vtRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if(auto commandBuffer = vtRenderer.beginFrame()) {
                vtRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                vtRenderer.endSwapChainRenderPass(commandBuffer);
                vtRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(vtDevice.device());
    }

    

    void FirstApp::loadGameObjects() {
        std::shared_ptr<VtModel> vtModel = VtModel::createModelFromFile(vtDevice, "models/colored_cube.obj");

        auto gameObj = VtGameObject::createGameObject();
        gameObj.model = vtModel;
        gameObj.transform.translation = {0.f, 0.f, 2.5f};
        gameObj.transform.scale = glm::vec3{1.f};
        gameObjects.push_back(std::move(gameObj));
    }
}