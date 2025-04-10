#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Device;
class DeviceMemory final {
public:
    DeviceMemory(const Device& device, size_t size, uint32_t memory_type_bits, VkMemoryAllocateFlags allocate, VkMemoryPropertyFlags property);
    DeviceMemory(DeviceMemory&& other) noexcept;
    ~DeviceMemory();

    VkDeviceMemory handle() const { return m_handle; }

    uint32_t FindMemopryType(uint32_t type_filter, VkMemoryPropertyFlags property) const;
    void*    Map(size_t offset, size_t size);
    void     Unmap();

private:
    VkDeviceMemory m_handle;
    const Device&  m_device;
};
} // namespace Vulkan
