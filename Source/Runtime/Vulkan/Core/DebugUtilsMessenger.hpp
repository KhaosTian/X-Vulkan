#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
class Instance;

class DebugUtilsMessenger final {
private:
    VkDebugUtilsMessengerEXT m_handle;

public:
    VkDebugUtilsMessengerEXT handle() const { return m_handle; }

private:
    const Instance&                              m_instance;
    const VkDebugUtilsMessageSeverityFlagBitsEXT m_serverity;

public:
    DebugUtilsMessenger(const Instance& instance, VkDebugUtilsMessageSeverityFlagBitsEXT message_serverity);
    ~DebugUtilsMessenger();
    VkDebugUtilsMessageSeverityFlagBitsEXT serverity() const { return m_serverity; }
};
} // namespace Vulkan