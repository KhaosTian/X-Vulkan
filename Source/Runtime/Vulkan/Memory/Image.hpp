#pragma once

#include <Vulkan/Vulkan.hpp>
#include <cstdint>

namespace Vulkan {
class CommandPool;
class Device;
class Buffer;
class DeviceMemory;

class Image final {
private:
    const Device&    m_device;
    VkImage          m_handle;
    const VkExtent2D m_extent;
    const VkFormat   m_format;
    VkImageLayout    m_image_layout;

public:
    Image(Image&&) noexcept;
    Image(const Device& device, VkExtent2D extent, uint32_t mip_level, VkFormat format);
    Image(
        const Device&     device,
        VkExtent2D        extent,
        uint32_t          mip_level,
        VkFormat          format,
        VkImageTiling     tiling,
        VkImageUsageFlags usage_flags
    );
    ~Image();

    VkImage       handle() const { return m_handle; }
    const Device& device() const { return m_device; }
    VkExtent2D    extent() const { return m_extent; }
    VkFormat      format() const { return m_format; }

    DeviceMemory         AllocateMemory(VkMemoryPropertyFlags property_flags) const;
    VkMemoryRequirements GetMemoryRequirements() const;

    void CopyFrom(CommandPool& cmd_pool, const Buffer& src);
    void TransitionImageLayout(CommandPool& cmd_pool, VkImageLayout layout);
};

} // namespace Vulkan