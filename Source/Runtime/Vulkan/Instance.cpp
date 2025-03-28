#include "Instance.hpp"
#include "Exception.hpp"
#include "Vulkan.hpp"
#include "Window.hpp"
#include "Enumerate.hpp"

namespace Vulkan {
Instance::Instance(const Window& window, const std::vector<const char*>& validation_layers): m_window(window) {
    // 初始化实例扩展
    m_instance_extensions = window.GetRequiredInstanceExtensions();
    m_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    // 检查验证层支持
    CheckValidationLayersSupport(validation_layers);

    // 应用信息
    VkApplicationInfo app_info  = {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "NOVA";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "NOVA";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = m_vk_api_version;

    // 实例创建信息
    VkInstanceCreateInfo create_info    = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = static_cast<uint32_t>(m_instance_extensions.size());
    create_info.ppEnabledExtensionNames = m_instance_extensions.data();
    create_info.enabledLayerCount       = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames     = validation_layers.data();

    // 创建实例
    VK_CHECK(vkCreateInstance(&create_info, nullptr, &m_vk_instance), "create vulkan instance");

    // 枚举物理设备
    m_vk_physical_devices = GetEnumerateVector(m_vk_instance, vkEnumeratePhysicalDevices);
}

void Instance::CheckValidationLayersSupport(const std::vector<const char*>& validation_layers) const {
    // 枚举可用的验证层
    const auto available_layers = GetEnumerateVector(vkEnumerateInstanceLayerProperties);

    for (const auto& layer_name: validation_layers) {
        bool found = false;

        // 检查是否支持所有需要的验证层
        for (const auto& layer: available_layers) {
            if (std::strcmp(layer.layerName, layer_name) == 0) {
                found = true;
                break;
            }
        }

        // 不支持所需的验证层
        if (!found) {
            Throw(std::runtime_error("Validation layer not supported."));
        }
    }
}

Instance::~Instance() {
    if (m_vk_instance == nullptr) return;

    vkDestroyInstance(m_vk_instance, nullptr);
    m_vk_instance = nullptr;
}
} // namespace Vulkan