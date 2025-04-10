#pragma once

#include "Vulkan/Core/Vulkan.hpp"
#include <cstddef>
#include <cstdint>

namespace Vulkan {
class DescriptorBinding;
class Device;
class DescriptorPool final {
    VkDescriptorPool m_handle;

public:
    VkDescriptorPool handle() const { return m_handle; }

private:
    const Device& m_device;

public:
    DescriptorPool(DescriptorPool&&) = delete;
    DescriptorPool(const Device& device, const std::vector<DescriptorBinding>& descriptor_bindings, size_t max_sets);
    ~DescriptorPool();

    const Device& device() const { return m_device; }
};
} // namespace Vulkan

