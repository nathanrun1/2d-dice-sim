#pragma once

#include "dicesim_camera.hpp"
#include "dicesim_device.hpp"
#include "dicesim_game_object.hpp"
#include "dicesim_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace dice {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(DiceSimDevice& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(
            VkCommandBuffer commandBuffer,
            std::vector<std::shared_ptr<DiceSimGameObject>>& gameObjects,
            const DiceSimCamera& camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        DiceSimDevice& diceSimDevice;

        std::unique_ptr<DiceSimPipeline> diceSimPipeline;
        VkPipelineLayout pipelineLayout;
    };
}  // namespace dice