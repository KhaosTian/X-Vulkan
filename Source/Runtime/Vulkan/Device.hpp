#pragma once

#include "Surface.hpp"
#include "Vulkan.hpp"
#include "Types.hpp"

namespace Vulkan {
class Surface;

class Device final {
public:
    Device(Device&&) = delete;
    Device(
        VkPhysicalDevice                physical_device,
        const Surface&                  surface,
        const std::vector<const char*>& required_extensions,
        const VkPhysicalDeviceFeatures& device_features
    );
    ~Device();

private:
    const VkPhysicalDevice m_physical_device;
    const Surface&         m_surface;

    VkDevice                   m_device { nullptr };
    VkPhysicalDeviceFeatures   m_device_features {};
    VkPhysicalDeviceProperties m_device_properties {};

    std::vector<VkExtensionProperties> m_available_extensions;

    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices;

public:
    VkDevice           handle() const { return m_device; }
    VkPhysicalDevice   physical_device() const { return m_physical_device; }
    const Surface&     surface() const { return m_surface; }
    QueueFamilyIndices queue_family_indices() const { return m_queue_indices; }

    static std::vector<VkDeviceQueueCreateInfo> BuildQueueCreateInfos(const QueueFamilyIndices& queue_indices);

private:
    void CheckRequiredExtensionsSupport(const std::vector<const char*>& required_extensions) const;

    QueueFamilyIndices FindQueueFamilyIndices() const;

    void CreateDevice(
        const std::vector<const char*>&             required_extensions,
        const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos
    );
};
} // namespace Vulkan