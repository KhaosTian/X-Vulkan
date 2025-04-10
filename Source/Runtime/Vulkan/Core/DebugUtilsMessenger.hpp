#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Instance;

class DebugUtilsMessenger final {
private:
    VkDebugUtilsMessengerEXT m_handle;

public:
    VkDebugUtilsMessengerEXT handle() const { return m_handle; }

private:
    const Instance& m_instance;

public:
    DebugUtilsMessenger(DebugUtilsMessenger&&) = delete;
    explicit DebugUtilsMessenger(const Instance& instance);
    ~DebugUtilsMessenger();

    const Instance& instance() const { return m_instance; }

    // 调试回调函数
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);

private:
    inline VkResult
    CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
};

} // namespace Vulkan