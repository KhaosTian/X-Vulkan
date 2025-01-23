#include "Instance.hpp"
#include "Window.hpp"

namespace Vulkan {
Instance::Instance(const Window& window): m_window(window) {
    // 初始化实例扩展
    m_instance_extensions = window.QueryRequiredInstanceExtensions();
    m_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    // 检查验证层支持
    CheckValidationLayersSupport();
    // 创建实例
    const VkApplicationInfo app_info = BuildApplicationInfo("Nova");
    CreateInstance(app_info);
    // 枚举物理设备
    m_physical_devices = EnumeratePhysicalDevices();
}

Instance::~Instance() {
    if (m_instance == nullptr) return;

    vkDestroyInstance(m_instance, nullptr);
    m_instance = nullptr;
}

std::vector<VkPhysicalDevice> Instance::EnumeratePhysicalDevices() const {
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());
    return physical_devices;
}

void Instance::CreateInstance(const VkApplicationInfo& app_info) {
    // 实例创建信息
    VkInstanceCreateInfo create_info    = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = static_cast<uint32_t>(m_instance_extensions.size());
    create_info.ppEnabledExtensionNames = m_instance_extensions.data();
    create_info.enabledLayerCount       = static_cast<uint32_t>(m_validation_layers.size());
    create_info.ppEnabledLayerNames     = m_validation_layers.data();

    // 创建实例
    if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}

VkApplicationInfo Instance::BuildApplicationInfo(const std::string& app_name) const {
    VkApplicationInfo app_info  = {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = app_name.c_str();
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = m_vulkan_api_version;
    return app_info;
}

void Instance::CheckValidationLayersSupport() const {
    // 枚举可用的验证层
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // 检查是否支持所有需要的验证层
    for (const auto& layer_name: m_validation_layers) {
        bool found = false;
        for (const auto& layer: available_layers) {
            if (std::strcmp(layer.layerName, layer_name) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("Validation layer not supported.");
        }
    }
}
} // namespace Vulkan