#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class CommandPool;
class Device;
class Image;
class ImageView;
class DeviceMemory;

class DepthBuffer final {
private:
    const VkFormat                m_format;
    std::unique_ptr<Image>        m_image;
    std::unique_ptr<DeviceMemory> m_image_memory;
    std::unique_ptr<ImageView>    m_image_view;

public:
    DepthBuffer(DepthBuffer&&) = delete;
    DepthBuffer(CommandPool& cmd_pool, VkExtent2D extent);
    ~DepthBuffer();

    VkFormat         format() const { return m_format; }
    const ImageView& image_view() const { return *m_image_view; }

private:
    VkFormat FindDepthFormat(const Device& device);
    VkFormat FindSupportFormat(
        const Device&                device,
        const std::vector<VkFormat>& candidates,
        const VkImageTiling          tiling,
        const VkFormatFeatureFlags   feature_flags
    );
};

} // namespace Vulkan