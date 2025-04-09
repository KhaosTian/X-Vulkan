#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class Device;
class DescriptorBinding;

class DescriptorSetLayout final {
    VkDescriptorSetLayout m_handle;

public:
    VkDescriptorSetLayout handle() const { return m_handle; }

private:
    const Device& m_device;

public:
    DescriptorSetLayout(DescriptorSetLayout&&) = delete;
    DescriptorSetLayout(const Device& device, const std::vector<DescriptorBinding>& descriptor_bindings);
    ~DescriptorSetLayout();

    const Device& device() const { return m_device; }
};
} // namespace Vulkan
