#include "Vulkan/Vulkan.hpp"
#include <Vulkan/Memory/DeviceMemory.hpp>
#include <Vulkan/Device.hpp>

namespace Vulkan {

DeviceMemory::DeviceMemory(
    const Device&               device,
    const size_t                size,
    const uint32_t              memory_type_bits,
    const VkMemoryAllocateFlags allocate_flags,
    const VkMemoryPropertyFlags property_flags
):
    m_device(device) {
    VkMemoryAllocateFlagsInfo flags_info = {};

    flags_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
    flags_info.pNext = nullptr;
    flags_info.flags = allocate_flags;

    VkMemoryAllocateInfo allocate_info = {};

    allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info.pNext           = &flags_info;
    allocate_info.allocationSize  = size;
    allocate_info.memoryTypeIndex = FindMemopryType(memory_type_bits, property_flags);

    VK_CHECK(vkAllocateMemory(device.handle(), &allocate_info, nullptr, &m_handle), "allocate memory");
};

DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept: m_device(other.m_device), m_handle(other.m_handle) {
    other.m_handle = nullptr;
}

DeviceMemory::~DeviceMemory() {
    if (m_handle == nullptr) return;
    vkFreeMemory(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

uint32_t DeviceMemory::FindMemopryType(uint32_t type_filter, VkMemoryPropertyFlags property_flags) const {
    VkPhysicalDeviceMemoryProperties properties;
    vkGetPhysicalDeviceMemoryProperties(m_device.physical_device(), &properties);
}
void* DeviceMemory::Map(size_t offset, size_t size) {
    void* data;
    VK_CHECK(vkMapMemory(m_device.handle(), m_handle, offset, size, 0, &data), "map memory");
    return data;
}

void DeviceMemory::Unmap() {
    vkUnmapMemory(m_device.handle(), m_handle);
}

} // namespace Vulkan