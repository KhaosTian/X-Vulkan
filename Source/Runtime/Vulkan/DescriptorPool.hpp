#pragma once

#include "Vulkan/Vulkan.hpp"
#include <cstddef>
#include <cstdint>

namespace Vulkan {
struct DescriptorBinding {
    uint32_t           binding;
    uint32_t           descriptor_count;
    VkDescriptorType   type;
    VkShaderStageFlags stage;
};

class Device;
class DescriptorPool final {
    HANDLE(VkDescriptorPool)
private:
    const Device& m_device;

public:
    DescriptorPool(DescriptorPool&&) = delete;
    DescriptorPool(const Device& device, const std::vector<DescriptorBinding>& descriptor_bindings, size_t max_sets);
    ~DescriptorPool();

    const Device& device() const { return m_device; }
};
} // namespace Vulkan
