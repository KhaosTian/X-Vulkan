#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Device;
class CommandPool final {
public:
    CommandPool(CommandPool&&) = delete;
    CommandPool(const Device& device, uint32_t queue_family_index, uint32_t queue, bool allow_reset);
    ~CommandPool();

    VkCommandPool handle() const { return m_handle; }
    const Device& device() const { return m_device; }
    VkQueue       queue() const { return m_queue; }

private:
    VkCommandPool m_handle;
    const Device& m_device;
    VkQueue       m_queue;
};
} // namespace Vulkan
