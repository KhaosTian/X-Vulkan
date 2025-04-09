#include "Vulkan/Vulkan.hpp"
#include "Vulkan/CommandPool.hpp"
#include "Vulkan/Device.hpp"

namespace Vulkan {
CommandPool::CommandPool(const Device& device, uint32_t queue_family_index, uint32_t queue, bool allow_reset):
    m_device(device) {
    VkCommandPoolCreateInfo create_info = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex        = queue_family_index;
    create_info.flags                   = allow_reset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;

    m_queue = device.graphics_queue();

    VK_CHECK(vkCreateCommandPool(device.handle(), &create_info, nullptr, &m_handle), "create command pool");
}

CommandPool::~CommandPool() {
    if (m_handle == nullptr) return;
    vkDestroyCommandPool(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

} // namespace Vulkan