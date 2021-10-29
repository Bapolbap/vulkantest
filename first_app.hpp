#pragma once

#include "vt_window.hpp"
#include "vt_pipeline.hpp"
#include "vt_device.hpp"

namespace vt {
    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 800;

            void run();
        private:
            VtWindow vtWindow{WIDTH, HEIGHT, "vulkan"};
            VtDevice vtDevice{vtWindow};
            VtPipeline vtPipeline{
                "shaders/simple_shader.vert.spv",
                "shaders/simple_shader.frag.spv",
                vtDevice,
                VtPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}