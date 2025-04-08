#pragma once

#include "Vulkan/Vulkan.hpp"
#include "Vulkan/Command/CommandBuffers.hpp"
#include "Vulkan/Command/CommandPool.hpp"
#include "Vulkan/Device.hpp"
#include <functional>

namespace Vulkan {
class CommandBuffer final {
public:
    static void Submit(CommandPool& cmd_pool, const std::function<void(VkCommandBuffer)>& action) {
        CommandBuffers           cmds(cmd_pool, 1);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(cmds[0], &begin_info);
        action(cmds[0]);
        vkEndCommandBuffer(cmds[0]);

        VkSubmitInfo submit_info       = {};
        submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers    = &cmds[0];

        const auto graphics_queue = cmd_pool.queue();
        vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
        vkQueueWaitIdle(graphics_queue);
    }
};
} // namespace Vulkan