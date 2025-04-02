
#include "Vulkan/Vulkan.hpp"
#include <Vulkan/Memory/Buffer.hpp>
#include <Vulkan/Memory/DeviceMemory.hpp>
#include <Vulkan/Command/CommandBuffer.hpp>
#include <Vulkan/Memory/Image.hpp>

namespace Vulkan {
Image::Image(const Device& device, VkExtent2D extent, uint32_t mip_level, VkFormat format):
    Image(
        device,
        extent,
        mip_level,
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    ) {
}
Image::Image(
    const Device&     device,
    VkExtent2D        extent,
    uint32_t          mip_level,
    VkFormat          format,
    VkImageTiling     tiling,
    VkImageUsageFlags usage_flags
):
    m_device(device),
    m_extent(extent),
    m_format(format),
    m_image_layout(VK_IMAGE_LAYOUT_UNDEFINED) {
    VkImageCreateInfo create_info = {};
    create_info.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.imageType         = VK_IMAGE_TYPE_2D;
    create_info.extent.width      = extent.width;
    create_info.extent.height     = extent.height;
    create_info.extent.depth      = 1;
    create_info.mipLevels         = mip_level;
    create_info.arrayLayers       = 1;
    create_info.format            = format;
    create_info.tiling            = tiling;
    create_info.initialLayout     = m_image_layout;
    create_info.usage             = usage_flags;
    create_info.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    create_info.samples           = VK_SAMPLE_COUNT_1_BIT;
    create_info.flags             = 0;

    VK_CHECK(vkCreateImage(m_device.handle(), &create_info, nullptr, &m_handle), "create image");
}

Image::~Image() {
    if (m_handle == nullptr) return;
    vkDestroyImage(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

DeviceMemory Image::AllocateMemory(VkMemoryPropertyFlags property_flags) const {
    const auto   requirements = GetMemoryRequirements();
    DeviceMemory memory(m_device, requirements.size, requirements.memoryTypeBits, 0, property_flags);
    VK_CHECK(vkBindImageMemory(m_device.handle(), m_handle, memory.handle(), 0), "bind image memory");

    return memory;
}

VkMemoryRequirements Image::GetMemoryRequirements() const {
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(m_device.handle(), m_handle, &requirements);
    return requirements;
}

void Image::CopyFrom(CommandPool& cmd_pool, const Buffer& buffer) {
    CommandBuffer::Submit(cmd_pool, [&](VkCommandBuffer cmd) {
        VkBufferImageCopy copy_region               = {};
        copy_region.bufferOffset                    = 0;
        copy_region.bufferRowLength                 = 0;
        copy_region.bufferImageHeight               = 0;
        copy_region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_region.imageSubresource.mipLevel       = 0;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount     = 1;
        copy_region.imageOffset                     = {
            0,
            0,
            0,
        };
        copy_region.imageExtent = { m_extent.width, m_extent.height, 1 };

        vkCmdCopyBufferToImage(cmd, buffer.handle(), m_handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    });
}
} // namespace Vulkan