#pragma once

#include "vt_device.hpp"
#include "vt_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace vt {
    class VtModel {
        public:

            struct Vertex {
                glm::vec3 position{};
                glm::vec3 color{};
                glm::vec3 normal{};
                glm::vec2 uv{};

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

                bool operator==(const Vertex &other) const {
                    return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
                }
            };

            struct Builder {
                std::vector<Vertex> vertices{};
                std::vector<uint32_t> indices{};

                void loadModels(const std::string &filepath);
            };

            VtModel(VtDevice &device, const VtModel::Builder &builder);
            ~VtModel();

            static std::unique_ptr<VtModel> createModelFromFile(VtDevice &device, const std::string &filepath);

            VtModel(const VtModel &) = delete;
            VtModel &operator=(const VtModel &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
            void createIndexBuffers(const std::vector<uint32_t> &indices);

            VtDevice& vtDevice;

            std::unique_ptr<VtBuffer> vertexBuffer;
            uint32_t vertexCount;

            bool hasIndexBuffer = false;
            std::unique_ptr<VtBuffer> indexBuffer;
            uint32_t indexCount;

    };
}