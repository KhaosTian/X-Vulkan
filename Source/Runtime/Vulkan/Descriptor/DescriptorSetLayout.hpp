#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Device;
class DescriptorBinding;

class DescriptorSetLayout final {
public:
    DescriptorSetLayout(DescriptorSetLayout&&) = delete;
    DescriptorSetLayout(const Device& device, const std::vector<DescriptorBinding>& descriptor_bindings);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout handle() const { return m_handle; }
    const Device&         device() const { return m_device; }

private:
    VkDescriptorSetLayout m_handle;
    const Device&         m_device;
};
} // namespace Vulkan
