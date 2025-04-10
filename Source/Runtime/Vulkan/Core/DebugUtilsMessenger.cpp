#include "Vulkan/Core/Instance.hpp"
#include "Vulkan/Core/Vulkan.hpp"
#include "Vulkan/Core/DebugUtilsMessenger.hpp"

namespace Vulkan {

inline VkResult DebugUtilsMessenger::CreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks*              pAllocator,
    VkDebugUtilsMessengerEXT*                 pDebugMessenger
) {
    const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func == nullptr) return VK_ERROR_EXTENSION_NOT_PRESENT;
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

inline void DebugUtilsMessenger::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
    const auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func == nullptr) return;
    func(instance, callback, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    std::cerr << "validation layer" << callback_data->pMessage << std::endl;
    return VK_FALSE;
}

DebugUtilsMessenger::DebugUtilsMessenger(const Instance& instance): m_instance(instance) {
    VkDebugUtilsMessengerCreateInfoEXT create_info = {};
    create_info.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    //指定定回调函数处理的消息级别
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    //指向回调函数的指针
    create_info.pfnUserCallback = DebugCallback;
    create_info.pUserData       = nullptr;

    VK_CHECK(CreateDebugUtilsMessengerEXT(instance.handle(), &create_info, nullptr, &m_handle), "create debug utils meesenger");
}
DebugUtilsMessenger::~DebugUtilsMessenger() {
}
} // namespace Vulkan