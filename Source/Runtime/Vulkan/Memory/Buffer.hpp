#pragma once

#include <Vulkan/Vulkan.hpp>

namespace Vulkan {
class CommandPool;
class Device;
class DeviceMemory;

class Buffer final {
private:
    const Device& m_device;
    VkBuffer      m_handle;

public:
    Buffer(Buffer&&) = delete;
    Buffer(const Device& device, size_t size, VkBufferUsageFlags usage);
    ~Buffer();

public:
    VkBuffer      handle() const { return m_handle; }
    const Device& device() const { return m_device; }

private:
    VkMemoryRequirements GetMemoryRequirements() const;
    VkDeviceAddress      GetDeviceAddress() const;

public:
    DeviceMemory AllocateMemory(VkMemoryPropertyFlags property_flags);
    DeviceMemory AllocateMemory(VkMemoryAllocateFlags allocate_flags, VkMemoryPropertyFlags property_flags);
    void         CopyFrom(CommandPool& cmd_pool, const Buffer& src, VkDeviceSize size);
    void         CopyTo(CommandPool& cmd_pool, const Buffer& dst, VkDeviceSize size);
};

} // namespace Vulkan