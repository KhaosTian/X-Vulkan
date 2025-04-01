#pragma once

#include <Vulkan/Surface.hpp>
#include <Vulkan/Vulkan.hpp>
#include <Vulkan/Types.hpp>

namespace Vulkan {
class Surface;

class Device final {
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

public:
    Device(Device&&) = delete;
    Device(
        VkPhysicalDevice                physical_device,
        const Surface&                  surface,
        const std::vector<const char*>& required_extensions,
        const VkPhysicalDeviceFeatures& device_features
    );
    ~Device();

public:
    VkDevice           handle() const { return m_handle; }
    VkPhysicalDevice   physical_device() const { return m_physical_device; }
    const Surface&     surface() const { return m_surface; }
    QueueFamilyIndices queue_family_indices() const { return m_queue_indices; }

    VkQueue graphics_queue() const { return m_queue_graphics; }
    VkQueue present_queue() const { return m_queue_present; }
    VkQueue compute_queue() const { return m_queue_compute; }

    void WaitIdle() const;

private:
    void               CheckRequiredExtensionsSupport(const std::vector<const char*>& required_extensions) const;
    QueueFamilyIndices FindQueueFamilyIndices(const std::vector<VkQueueFamilyProperties> queue_families) const;
};
} // namespace Vulkan