#pragma once

#include "Vulkan/Core/Surface.hpp"
#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Surface;

struct QueueFamilyIndices {
    uint32_t graphics = UINT32_MAX;
    uint32_t present  = UINT32_MAX;
    uint32_t compute  = UINT32_MAX;

    bool IsComplete() const { return (graphics != UINT32_MAX && present != UINT32_MAX); }
};

class Device final {
public:
    Device(Device&&) = delete;
    Device(VkPhysicalDevice physical_device, const Surface& surface, const std::vector<const char*>& required_extensions, const VkPhysicalDeviceFeatures& device_features);
    ~Device();

    VkDevice           handle() const { return m_handle; }
    VkPhysicalDevice   physical_device() const { return m_physical_device; }
    const Surface&     surface() const { return m_surface; }
    QueueFamilyIndices queue_family_indices() const { return m_queue_indices; }

    VkQueue graphics_queue() const { return m_queue_graphics; }
    VkQueue present_queue() const { return m_queue_present; }
    VkQueue compute_queue() const { return m_queue_compute; }

    void WaitIdle() const;

private:
    VkDevice               m_handle {};
    const VkPhysicalDevice m_physical_device;
    const Surface&         m_surface;

    VkPhysicalDeviceFeatures   m_device_features {};
    VkPhysicalDeviceProperties m_device_properties {};

    std::vector<VkExtensionProperties> m_available_extensions;

    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices;

    void               CheckRequiredExtensionsSupport(const std::vector<const char*>& required_extensions) const;
    QueueFamilyIndices FindQueueFamilyIndices(const std::vector<VkQueueFamilyProperties> queue_families) const;
};
} // namespace Vulkan
