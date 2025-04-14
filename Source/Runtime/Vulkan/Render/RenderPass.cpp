#include "Vulkan/Render/RenderPass.hpp"
#include "RenderPass.hpp"
#include "Vulkan/Resource/SwapChain.hpp"
#include "Vulkan/Resource/DepthBuffer.hpp"
#include "Vulkan/Core/Device.hpp"
#include "Vulkan/Core/Vulkan.hpp"
#include <cstdint>
#include <vector>

namespace Vulkan {
RenderPass::RenderPass(const Swapchain& swapchain, const DepthBuffer& depth_buffer, VkAttachmentLoadOp color_buffer_load_op): m_swapchain(swapchain), m_depth_buffer((depth_buffer)) {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format                  = swapchain.format();
    color_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                  = color_buffer_load_op;
    color_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment            = 0;
    color_attachment_ref.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments    = &color_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = {
        color_attachment,
    };

    VkRenderPassCreateInfo create_info = {};
    create_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount        = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments           = attachments.data();
    create_info.subpassCount           = 1;
    create_info.pSubpasses             = &subpass;
    create_info.dependencyCount        = 1;
    create_info.pDependencies          = &dependency;

    VK_CHECK(vkCreateRenderPass(swapchain.device().handle(), &create_info, nullptr, &m_handle), "create render pass");
}
RenderPass::RenderPass(const Swapchain& swapchain, const DepthBuffer& depth_buffer, VkAttachmentLoadOp color_buffer_load_op, VkAttachmentLoadOp depth_buffer_load_op):
    m_swapchain(swapchain),
    m_depth_buffer((depth_buffer)) {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format                  = swapchain.format();
    color_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                  = color_buffer_load_op;
    color_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depth_attachment = {};
    depth_attachment.format                  = depth_buffer.format();
    depth_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp                  = depth_buffer_load_op;
    depth_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depth_attachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment            = 0;
    color_attachment_ref.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment            = 1;
    depth_attachment_ref.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass    = {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = {
        color_attachment,
        depth_attachment,
    };

    VkRenderPassCreateInfo create_info = {};
    create_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount        = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments           = attachments.data();
    create_info.subpassCount           = 1;
    create_info.pSubpasses             = &subpass;
    create_info.dependencyCount        = 1;
    create_info.pDependencies          = &dependency;

    VK_CHECK(vkCreateRenderPass(swapchain.device().handle(), &create_info, nullptr, &m_handle), "create render pass");
}
RenderPass::RenderPass(const Swapchain& swapchain, VkFormat format, const DepthBuffer& depth_buffer, VkAttachmentLoadOp color_buffer_load_op, VkAttachmentLoadOp depth_buffer_load_op):
    m_swapchain(swapchain),
    m_depth_buffer((depth_buffer)) {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format                  = format;
    color_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                  = color_buffer_load_op;
    color_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depth_attachment = {};
    depth_attachment.format                  = depth_buffer.format();
    depth_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp                  = depth_buffer_load_op;
    depth_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depth_attachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment            = 0;
    color_attachment_ref.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment            = 1;
    depth_attachment_ref.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass    = {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = {
        color_attachment,
        depth_attachment,
    };

    VkRenderPassCreateInfo create_info = {};
    create_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount        = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments           = attachments.data();
    create_info.subpassCount           = 1;
    create_info.pSubpasses             = &subpass;
    create_info.dependencyCount        = 1;
    create_info.pDependencies          = &dependency;

    VK_CHECK(vkCreateRenderPass(swapchain.device().handle(), &create_info, nullptr, &m_handle), "create render pass");
}
RenderPass::RenderPass(
    const Swapchain&   swapchain,
    VkFormat           format,
    VkFormat           format1,
    VkFormat           format2,
    const DepthBuffer& depth_buffer,
    VkAttachmentLoadOp color_buffer_load_op,
    VkAttachmentLoadOp depth_buffer_load_op
):
    m_swapchain(swapchain),
    m_depth_buffer((depth_buffer)) {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format                  = format;
    color_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                  = color_buffer_load_op;
    color_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription color_attachment1 = {};
    color_attachment.format                   = format1;
    color_attachment.samples                  = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                   = color_buffer_load_op;
    color_attachment.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout            = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout              = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription color_attachment2 = {};
    color_attachment.format                   = format2;
    color_attachment.samples                  = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                   = color_buffer_load_op;
    color_attachment.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout            = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.finalLayout              = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depth_attachment = {};
    depth_attachment.format                  = depth_buffer.format();
    depth_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp                  = depth_buffer_load_op;
    depth_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout           = color_buffer_load_op == VK_ATTACHMENT_LOAD_OP_CLEAR ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depth_attachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::vector<VkAttachmentReference> color_attachment_refs = { { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },
                                                                 { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },
                                                                 { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } };

    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment            = 3;
    depth_attachment_ref.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass    = {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = color_attachment_refs.size();
    subpass.pColorAttachments       = color_attachment_refs.data();
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = {
        color_attachment,
        color_attachment1,
        color_attachment2,
        depth_attachment,
    };

    VkRenderPassCreateInfo create_info = {};
    create_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount        = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments           = attachments.data();
    create_info.subpassCount           = 1;
    create_info.pSubpasses             = &subpass;
    create_info.dependencyCount        = 1;
    create_info.pDependencies          = &dependency;

    VK_CHECK(vkCreateRenderPass(swapchain.device().handle(), &create_info, nullptr, &m_handle), "create render pass");
}

RenderPass::~RenderPass() {
    if (m_handle == nullptr) return;
    vkDestroyRenderPass(m_swapchain.device().handle(), m_handle, nullptr);
    m_handle = nullptr;
}
} // namespace Vulkan
