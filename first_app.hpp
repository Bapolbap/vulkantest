#pragma once

#include "vt_window.hpp"
#include "vt_pipeline.hpp"
#include "vt_device.hpp"
#include "vt_game_object.hpp"
#include "vt_renderer.hpp"

#include <memory>
#include <vector>

namespace vt {
    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 800;

            FirstApp();
            ~FirstApp();

            FirstApp(const FirstApp &) = delete;
            FirstApp &operator=(const FirstApp &) = delete;

            void run();
        private:
            void loadGameObjects();
            void createPipelineLayout();
            void createPipeline();
            void renderGameObjects(VkCommandBuffer commandBuffer);

            VtWindow vtWindow{WIDTH, HEIGHT, "vulkan"};
            VtDevice vtDevice{vtWindow};
            VtRenderer vtRenderer{vtWindow, vtDevice};
            std::unique_ptr<VtPipeline> vtPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VtGameObject> gameObject;

    };
}