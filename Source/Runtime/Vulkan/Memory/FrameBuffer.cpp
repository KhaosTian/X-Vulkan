#include "Vulkan/Memory/Framebuffer.hpp"
#include "Vulkan/Memory/DepthBuffer.hpp"
#include "Vulkan/Memory/ImageView.hpp"
#include "Vulkan/Render/RenderPass.hpp"
#include "Vulkan/Swapchain.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Vulkan.hpp"
#include <cstdint>
#include <vector>

namespace Vulkan {
Framebuffer::Framebuffer(const ImageView& image_view, const RenderPass& render_pass, bool with_ds):
    m_device(image_view.device()) {
    std::vector<VkImageView> attachments = { image_view.handle() };

    if (with_ds) {
        attachments.push_back(render_pass.depth_buffer().image_view().handle());
    }

    VkFramebufferCreateInfo create_info = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_info.renderPass              = render_pass.handle();
    create_info.attachmentCount         = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments            = attachments.data();
    create_info.width                   = render_pass.swapchain().extent().width;
    create_info.height                  = render_pass.swapchain().extent().height;
    create_info.layers                  = 1;

    VK_CHECK(vkCreateFramebuffer(image_view.device().handle(), &create_info, nullptr, &m_handle), "create framebuffer");
}

Framebuffer::Framebuffer(
    const ImageView&  image_view,
    const ImageView&  image_view1,
    const ImageView&  image_view2,
    const RenderPass& render_pass
):
    m_device(image_view.device()) {
    std::vector<VkImageView> attachments = { image_view.handle(),
                                             image_view1.handle(),
                                             image_view2.handle(),
                                             render_pass.depth_buffer().image_view().handle() };

    VkFramebufferCreateInfo create_info = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_info.renderPass              = render_pass.handle();
    create_info.attachmentCount         = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments            = attachments.data();
    create_info.width                   = render_pass.swapchain().extent().width;
    create_info.height                  = render_pass.swapchain().extent().height;
    create_info.layers                  = 1;

    VK_CHECK(vkCreateFramebuffer(image_view.device().handle(), &create_info, nullptr, &m_handle), "create framebuffer");
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept: m_device(other.m_device), m_handle(other.m_handle) {
    other.m_handle = nullptr;
}

Framebuffer::~Framebuffer() {
    if (m_handle == nullptr) return;
    vkDestroyFramebuffer(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

} // namespace Vulkan