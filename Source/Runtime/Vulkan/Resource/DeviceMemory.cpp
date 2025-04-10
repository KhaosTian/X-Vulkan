#include "Vulkan/Core/Exception.hpp"
#include "Vulkan/Core/Vulkan.hpp"
#include "Vulkan/Resource/DeviceMemory.hpp"
#include "Vulkan/Core/Device.hpp"
#include <cstdint>
#include <stdexcept>

namespace Vulkan {

DeviceMemory::DeviceMemory(
    const Device&         device,
    size_t                size,
    uint32_t              memory_type_bits,
    VkMemoryAllocateFlags allocate,
    VkMemoryPropertyFlags property
):
    m_device(device) {
    VkMemoryAllocateFlagsInfo flags_info = {};

    flags_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    flags_info.pNext = nullptr;
    flags_info.flags = allocate;

    VkMemoryAllocateInfo allocate_info = {};

    allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info.pNext           = &flags_info;
    allocate_info.allocationSize  = size;
    allocate_info.memoryTypeIndex = FindMemopryType(memory_type_bits, property);

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

uint32_t DeviceMemory::FindMemopryType(uint32_t type_filter, VkMemoryPropertyFlags property) const {
    VkPhysicalDeviceMemoryProperties properties;
    vkGetPhysicalDeviceMemoryProperties(m_device.physical_device(), &properties);
    // 分配内存时需要指定内存类型，type_filter按位标记了允许的内从类型，flags则标记了其必备的属性组合
    for (uint32_t i = 0; i != properties.memoryTypeCount; ++i) {
        //通过filter捕获符合要求的内存类型，通过flags查询内存属性是否支持所有特性
        if ((type_filter & (1 << i)) && (properties.memoryTypes[i].propertyFlags & property) == property) {
            return i;
        }
    }

    Throw(std::runtime_error("failed to find susitable memory type"));
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
