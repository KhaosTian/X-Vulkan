#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
class Window;

class Instance final {
public:
    Instance(Instance&&) = delete;
    explicit Instance(const Window& window);
    ~Instance();

private:
    uint32_t   m_vulkan_api_version { VK_API_VERSION_1_0 };
    VkInstance m_instance { nullptr };

    const Window& m_window;

    std::vector<VkPhysicalDevice>  m_physical_devices;
    std::vector<const char*>       m_instance_extensions;
    const std::vector<const char*> m_validation_layers { "VK_LAYER_KHRONOS_validation" };

public:
    VkInstance                           handle() const { return m_instance; }
    const std::vector<const char*>&      instance_extensions() const { return m_instance_extensions; }
    const std::vector<VkPhysicalDevice>& physical_devices() const { return m_physical_devices; }
    const Window&                        window() const { return m_window; }

    VkApplicationInfo             BuildApplicationInfo(const std::string& app_name) const;
    std::vector<VkPhysicalDevice> EnumeratePhysicalDevices() const;
    void                          CreateInstance(const VkApplicationInfo& app_info);

private:
    void CheckValidationLayersSupport() const;
};
} // namespace Vulkan