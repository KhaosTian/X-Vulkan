#pragma once

#include <Vulkan/Vulkan.hpp>

namespace Vulkan {
class Device;
class DeviceMemory final {
private:
    VkDeviceMemory m_handle;
    const Device&  m_device;

public:
    DeviceMemory(
        const Device&         device,
        size_t                size,
        uint32_t              memory_type_bits,
        VkMemoryAllocateFlags allocate_flags,
        VkMemoryPropertyFlags property_flags
    );
    DeviceMemory(DeviceMemory&& other) noexcept;
    ~DeviceMemory();

public:
    VkDeviceMemory handle() const { return m_handle; }

    uint32_t FindMemopryType(uint32_t type_filter, VkMemoryPropertyFlags property_flags) const;
    void*    Map(size_t offset, size_t size);
    void     Unmap();
};
} // namespace Vulkan