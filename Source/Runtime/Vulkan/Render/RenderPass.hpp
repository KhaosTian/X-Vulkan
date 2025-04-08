#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class DepthBuffer;
class SwapChain;

class RenderPass final {
private:
    const SwapChain&   m_swap_chain;
    const DepthBuffer& m_depth_buffer;
    VkRenderPass       m_handle;

public:
    RenderPass(RenderPass&&) = delete;

    RenderPass(const SwapChain& swap_chain, const DepthBuffer& depth_buffer, VkAttachmentLoadOp color_buffer_load_op);
    RenderPass(
        const SwapChain&   swap_chain,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    RenderPass(
        const SwapChain&   swap_chain,
        VkFormat           format,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    RenderPass(
        const SwapChain&   swap_chain,
        VkFormat           format,
        VkFormat           format1,
        VkFormat           format2,
        const DepthBuffer& depth_buffer,
        VkAttachmentLoadOp color_buffer_load_op,
        VkAttachmentLoadOp depth_buffer_load_op
    );
    ~RenderPass();

    VkRenderPass       handle() const { return m_handle; }
    const SwapChain&   swap_chain() const { return m_swap_chain; }
    const DepthBuffer& depth_buffer() const { return m_depth_buffer; }
};

} // namespace Vulkan