#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
class Window;

class Instance final {
    VULKAN_HANDLE(VkInstance, m_vk_instance)
private:
    const Window& m_window;

    std::vector<VkPhysicalDevice> m_vk_physical_devices;
    std::vector<const char*>      m_instance_extensions;
    uint32_t                      m_vk_api_version { VK_API_VERSION_1_0 };

public:
    NON_COPY(Instance)
    explicit Instance(const Window& window, const std::vector<const char*>& validationLayers);
    ~Instance();

public:
    const std::vector<const char*>&      instance_extensions() const { return m_instance_extensions; }
    const std::vector<VkPhysicalDevice>& vk_physical_devices() const { return m_vk_physical_devices; }
    const Window&                        window() const { return m_window; }

private:
    void CheckValidationLayersSupport(const std::vector<const char*>& validation_layers) const;
};
} // namespace Vulkan