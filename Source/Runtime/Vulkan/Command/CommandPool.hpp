#pragma once

#include <Vulkan/Vulkan.hpp>

namespace Vulkan {
class Device;
class CommandPool final {
private:
    VkCommandPool m_handle;
    const Device& m_device;
    VkQueue       m_queue;

public:
    CommandPool(CommandPool&&) = delete;
    CommandPool(const Device& device, uint32_t queue_family_index, uint32_t queue, bool allow_reset);

    ~CommandPool();

public:
    VkCommandPool handle() const { return m_handle; }
    const Device& device() const { return m_device; }
    VkQueue       queue() const { return m_queue; }
};
} // namespace Vulkan