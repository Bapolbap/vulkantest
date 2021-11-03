#include "first_app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>

namespace vt {

    struct SimplePushConstantData {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    FirstApp::FirstApp() {
        loadGameObjects();
        createPipelineLayout();
        createPipeline();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(vtDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while(!vtWindow.shouldClose()) {
            glfwPollEvents();

            if(auto commandBuffer = vtRenderer.beginFrame()) {
                vtRenderer.beginSwapChainRenderPass(commandBuffer);
                renderGameObjects(commandBuffer);
                vtRenderer.endSwapChainRenderPass(commandBuffer);
                vtRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(vtDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::vector<VtModel::Vertex> vertices {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        auto vtModel = std::make_shared<VtModel>(vtDevice, vertices);

        auto triangle = VtGameObject::createGameObject();
        triangle.model = vtModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObject.push_back(std::move(triangle));
    }

    void FirstApp::createPipelineLayout() {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(vtDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipelineLayout");
        }
    }

    void FirstApp::createPipeline() {
        assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VtPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = vtRenderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vtPipeline = std::make_unique<VtPipeline>(
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            vtDevice,
            pipelineConfig);
    }

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {

        int i = 1;
        for(auto& obj : gameObject) {
            i++;
            obj.transform2d.rotation = glm::mod<float>(obj.transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
        }

        vtPipeline->bind(commandBuffer);

        for(auto& obj : gameObject) {
            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
            );

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}