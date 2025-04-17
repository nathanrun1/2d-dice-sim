#pragma once

#include "dicesim_device.hpp"
#include "dicesim_swap_chain.hpp"
#include "dicesim_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace dice {
    class DiceSimRenderer {
    public:
        DiceSimRenderer(DiceSimWindow& window, DiceSimDevice& device);
        ~DiceSimRenderer();

        DiceSimRenderer(const DiceSimRenderer&) = delete;
        DiceSimRenderer& operator=(const DiceSimRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return diceSimSwapChain->getRenderPass(); }
        float getAspectRatio() const { return diceSimSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        DiceSimWindow& diceSimWindow;
        DiceSimDevice& diceSimDevice;
        std::unique_ptr<DiceSimSwapChain> diceSimSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{ 0 };
        bool isFrameStarted{ false };
    };
}  // namespace dice