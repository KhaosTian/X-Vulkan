#include "Vulkan/Memory/Buffer.hpp"
#include "Vulkan/Memory/DeviceMemory.hpp"
#include "Vulkan/Command/CommandBuffer.hpp"

namespace Vulkan {

Buffer::Buffer(const Device& device, size_t size, VkBufferUsageFlags usage): m_device(device) {
    VkBufferCreateInfo create_info = {};
    create_info.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    create_info.size               = size;
    create_info.usage              = usage;
    create_info.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(device.handle(), &create_info, nullptr, &m_handle), "create buffer");
}

Buffer::~Buffer() {
    if (m_handle == nullptr) return;
    vkDestroyBuffer(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

VkMemoryRequirements Buffer::GetMemoryRequirements() const {
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(m_device.handle(), m_handle, &requirements);
    return requirements;
}

DeviceMemory Buffer::AllocateMemory(VkMemoryPropertyFlags property_flags) {
    return AllocateMemory(0, property_flags);
}

DeviceMemory Buffer::AllocateMemory(VkMemoryAllocateFlags allocate_flags, VkMemoryPropertyFlags property_flags) {
    const auto requirements = GetMemoryRequirements();

    DeviceMemory memory(m_device, requirements.size, requirements.memoryTypeBits, allocate_flags, property_flags);

    VK_CHECK(vkBindBufferMemory(m_device.handle(), m_handle, memory.handle(), 0), "bind buffer memory");

    return memory;
}

VkDeviceAddress Buffer::GetDeviceAddress() const {
    VkBufferDeviceAddressInfo address_info = {};
    address_info.sType                     = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    address_info.pNext                     = nullptr;
    address_info.buffer                    = handle();
    return vkGetBufferDeviceAddress(m_device.handle(), &address_info);
}

void Buffer::CopyFrom(CommandPool& cmd_pool, const Buffer& src, VkDeviceSize size) {
    CommandBuffer::Submit(cmd_pool, [&](VkCommandBuffer cmd) {
        VkBufferCopy copy_region = {};
        copy_region.srcOffset    = 0;
        copy_region.dstOffset    = 0;
        copy_region.size         = size;

        vkCmdCopyBuffer(cmd, src.handle(), handle(), 1, &copy_region);
    });
}
void Buffer::CopyTo(CommandPool& cmd_pool, const Buffer& dst, VkDeviceSize size) {
    CommandBuffer::Submit(cmd_pool, [&](VkCommandBuffer cmd) {
        VkBufferCopy copy_region = {};
        copy_region.srcOffset    = 0;
        copy_region.dstOffset    = 0;
        copy_region.size         = size;

        vkCmdCopyBuffer(cmd, handle(), dst.handle(), 1, &copy_region);
    });
}

} // namespace Vulkan