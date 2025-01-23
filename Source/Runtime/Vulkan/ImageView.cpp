#include "ImageView.hpp"
#include "Device.hpp"

namespace Vulkan {

ImageView::ImageView(
    const Device&            device,
    const VkImage            image,
    const VkFormat           format,
    const VkImageAspectFlags aspect_flags
):
    m_device(device) {
    VkImageViewCreateInfo create_info = {};
    create_info.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image                 = image;
    create_info.viewType              = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format                = format;

    // subresourceRange
    create_info.subresourceRange.aspectMask     = aspect_flags;
    create_info.subresourceRange.baseMipLevel   = 0;
    create_info.subresourceRange.levelCount     = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount     = 1;

    // components
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    if (vkCreateImageView(m_device.handle(), &create_info, nullptr, &m_image_view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

ImageView::~ImageView() {
    if (m_image_view == nullptr) return;
    vkDestroyImageView(m_device.handle(), m_image_view, nullptr);
    m_image_view = nullptr;
}
} // namespace Vulkan