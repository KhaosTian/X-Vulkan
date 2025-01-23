#pragma once
#include "Vulkan.hpp"

namespace Vulkan {
class Device;
class ImageView final {
public:
    ImageView(const Device& device, const VkImage image, const VkFormat format, const VkImageAspectFlags aspect_flags);
    ~ImageView();
    ImageView(ImageView&&) = delete;

    VkImageView handle() const { return m_image_view; }

private:
    VkImageView   m_image_view { nullptr };
    const Device& m_device;
};

} // namespace Vulkan