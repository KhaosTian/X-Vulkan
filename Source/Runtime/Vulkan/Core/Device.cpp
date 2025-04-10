#include "Vulkan/Core/Device.hpp"

#include "Vulkan/Core/Exception.hpp"
#include "Vulkan/Core/Surface.hpp"
#include "Vulkan/Core/Enumerate.hpp"
#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {

Device::Device(
    VkPhysicalDevice                physical_device,
    const Surface&                  surface,
    const std::vector<const char*>& required_extensions,
    const VkPhysicalDeviceFeatures& device_features
):
    m_physical_device(physical_device),
    m_surface(surface),
    m_device_features(device_features) {
    // 检查设备是否支持必要的扩展
    CheckRequiredExtensionsSupport(required_extensions);

    // 寻找队列族索引
    const auto queue_families = GetEnumerateVector(m_physical_device, vkGetPhysicalDeviceQueueFamilyProperties);
    m_queue_indices           = FindQueueFamilyIndices(queue_families);

    // 构造队列创建信息数组
    std::vector<float> queue_priorities     = { 1.0f };
    const std::set     queue_family_indices = { m_queue_indices.graphics,
                                                m_queue_indices.present,
                                                m_queue_indices.compute };

    // 有几个队列族索引就创建几个队列
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos = {};
    for (const auto& queue_index: queue_family_indices) {
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueCount              = 1;
        queue_create_info.pQueuePriorities        = queue_priorities.data();
        queue_create_info.queueFamilyIndex        = queue_index;
        queue_create_infos.emplace_back(queue_create_info);
    }

    // 设备创建信息
    VkDeviceCreateInfo device_create_info      = {};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
    device_create_info.pEnabledFeatures        = &m_device_features;
    device_create_info.ppEnabledExtensionNames = required_extensions.data();
    device_create_info.enabledExtensionCount   = static_cast<uint32_t>(required_extensions.size());
    device_create_info.enabledLayerCount       = 0;

    // 创建逻辑设备
    VK_CHECK(vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_handle), "create logical device");

    // 获取队列句柄
    vkGetDeviceQueue(m_handle, m_queue_indices.graphics, 0, &m_queue_graphics);
    vkGetDeviceQueue(m_handle, m_queue_indices.present, 0, &m_queue_present);
    vkGetDeviceQueue(m_handle, m_queue_indices.compute, 0, &m_queue_compute);

    // 获取设备属性
    vkGetPhysicalDeviceProperties(m_physical_device, &m_device_properties);
}

void Device::CheckRequiredExtensionsSupport(const std::vector<const char*>& required_extensions) const {
    const auto available_extensions =
        GetEnumerateVector(m_physical_device, static_cast<const char*>(nullptr), vkEnumerateDeviceExtensionProperties);

    // 必需扩展转换为哈希表，然后剔除所有设备支持的扩展
    std::set<std::string> required_extension_set(required_extensions.begin(), required_extensions.end());
    for (const auto& extension: available_extensions) {
        required_extension_set.erase(extension.extensionName);
    }

    // 只有当哈希表为空时，才说明设备完整支持所必需的扩展
    if (!required_extension_set.empty()) {
        Throw(std::runtime_error("missing required extension."));
    }
}

QueueFamilyIndices Device::FindQueueFamilyIndices(const std::vector<VkQueueFamilyProperties> queue_families) const {
    // 匹配支持图形、呈现和计算三种管线队列族的索引
    QueueFamilyIndices indices = {};
    for (int i = 0; i < queue_families.size(); i++) {
        const auto& queue_family = queue_families[i];

        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphics = i;
        if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) indices.compute = i;

        VkBool32 is_present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, m_surface.handle(), &is_present_support);
        if (is_present_support) indices.present = i;

        if (indices.IsComplete()) break;
    }
    return indices;
}

void Device::WaitIdle() const {
    VK_CHECK(vkDeviceWaitIdle(m_handle), "wait for device idle");
}

Device::~Device() {
    if (m_handle == nullptr) return;

    vkDestroyDevice(m_handle, nullptr);
    m_handle = nullptr;
}

} // namespace Vulkan
