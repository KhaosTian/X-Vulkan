#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class DepthBuffer;
class Swapchain;

class RenderPass final {
private:
    const Swapchain&   m_swapchain;
    const DepthBuffer& m_depth_buffer;
    VkRenderPass       m_handle;

public:
    RenderPass(RenderPass&&) = delete;

    RenderPass(const Swapchain& swapchain, const DepthBuffer& depth_buffer, VkAttachmentLoadOp color_buffer_load_op);
    RenderPass(
        const Swapchain&   swapchain,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    RenderPass(
        const Swapchain&   swapchain,
        VkFormat           format,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    RenderPass(
        const Swapchain&   swapchain,
        VkFormat           format,
        VkFormat           format1,
        VkFormat           format2,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    ~RenderPass();

    VkRenderPass       handle() const { return m_handle; }
    const Swapchain&   swapchain() const { return m_swapchain; }
    const DepthBuffer& depth_buffer() const { return m_depth_buffer; }
};

} // namespace Vulkan
