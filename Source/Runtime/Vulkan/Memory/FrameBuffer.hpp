#pragma once

#include "Vulkan/Memory/ImageView.hpp"
#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class CommandPool;
class Device;
class Buffer;
class DeviceMemory;

class FrameBuffer final {
private:
    const Device& m_device;
    FrameBuffer   m_handle;

public:
    FrameBuffer(FrameBuffer&&) noexcept;
    FrameBuffer(const Device& device, VkExtent2D extent, uint32_t mip_level, VkFormat format);
    FrameBuffer(const ImageView& image_view, const RenderPass& render_pass);
    ~FrameBuffer();

    FrameBuffer   handle() const { return m_handle; }
    const Device& device() const { return m_device; }

    DeviceMemory         AllocateMemory(VkMemoryPropertyFlags property_flags) const;
    VkMemoryRequirements GetMemoryRequirements() const;

    void CopyFrom(CommandPool& cmd_pool, const Buffer& buffer);
    void TransitionImageLayout(CommandPool& cmd_pool, VkImageLayout layout);
};

} // namespace Vulkan