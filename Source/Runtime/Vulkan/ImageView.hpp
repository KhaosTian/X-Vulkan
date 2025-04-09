#pragma once
#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class Device;
class ImageView final {
public:
    ImageView(const Device& device, const VkImage image, const VkFormat format, const VkImageAspectFlags aspect);
    ~ImageView();
    ImageView(ImageView&&) = delete;

    VkImageView   handle() const { return m_handle; }
    const Device& device() const { return m_device; }

private:
    VkImageView   m_handle { nullptr };
    const Device& m_device;
};

} // namespace Vulkan