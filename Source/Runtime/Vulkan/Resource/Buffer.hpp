#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class CommandPool;
class Device;
class DeviceMemory;

class Buffer final {
public:
    Buffer(Buffer&&) = delete;
    Buffer(const Device& device, size_t size, VkBufferUsageFlags usage);
    ~Buffer();

    VkBuffer      handle() const { return m_handle; }
    const Device& device() const { return m_device; }

    DeviceMemory AllocateMemory(VkMemoryPropertyFlags property);
    DeviceMemory AllocateMemory(VkMemoryAllocateFlags allocate, VkMemoryPropertyFlags property);
    void         CopyFrom(CommandPool& cmd_pool, const Buffer& src, VkDeviceSize size);
    void         CopyTo(CommandPool& cmd_pool, const Buffer& dst, VkDeviceSize size);

private:
    const Device& m_device;
    VkBuffer      m_handle;

    VkMemoryRequirements GetMemoryRequirements() const;
    VkDeviceAddress      GetDeviceAddress() const;
};

} // namespace Vulkan
