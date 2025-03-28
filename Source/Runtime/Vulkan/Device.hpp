#pragma once

#include "Surface.hpp"
#include "Vulkan.hpp"
#include "Types.hpp"

namespace Vulkan {
class Surface;

class Device final {
    VULKAN_HANDLE(VkDevice, m_vk_device)
private:
    const VkPhysicalDevice m_vk_physical_device;
    const Surface&         m_surface;

    VkPhysicalDeviceFeatures   m_device_features {};
    VkPhysicalDeviceProperties m_device_properties {};

    std::vector<VkExtensionProperties> m_available_extensions;

    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices;

public:
    NON_COPY(Device)
    Device(
        VkPhysicalDevice                physical_device,
        const Surface&                  surface,
        const std::vector<const char*>& required_extensions,
        const VkPhysicalDeviceFeatures& device_features
    );
    ~Device();

public:
    VkPhysicalDevice   vk_physical_device() const { return m_vk_physical_device; }
    const Surface&     surface() const { return m_surface; }
    QueueFamilyIndices queue_family_indices() const { return m_queue_indices; }

    void WaitIdle() const;

private:
    void               CheckRequiredExtensionsSupport(const std::vector<const char*>& required_extensions) const;
    QueueFamilyIndices FindQueueFamilyIndices(const std::vector<VkQueueFamilyProperties> queue_families) const;
};
} // namespace Vulkan