#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class DepthBuffer;
class Swapchain;

class RenderPass final {
private:
    const Swapchain& m_swapchain;
    RenderPass       m_handle;

public:
    RenderPass(RenderPass&&) = delete;
    RenderPass(const Swapchain& swapchain, VkExtent2D extent, uint32_t mip_level, VkFormat format);
    RenderPass(const ImageView& image_view, const RenderPass& render_pass);
    ~RenderPass();

    RenderPass    handle() const { return m_handle; }
    const Device& device() const { return m_swapchain; }

    DeviceMemory         AllocateMemory(VkMemoryPropertyFlags property_flags) const;
    VkMemoryRequirements GetMemoryRequirements() const;

    void CopyFrom(CommandPool& cmd_pool, const Buffer& buffer);
    void TransitionImageLayout(CommandPool& cmd_pool, VkImageLayout layout);
};

} // namespace Vulkan