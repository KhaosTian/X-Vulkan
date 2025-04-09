#pragma once

#include "Vulkan/DescriptorBinding.hpp"
#include "Vulkan/Vulkan.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace Vulkan {
class Device;
class DescriptorSets;
class DescriptorSetLayout;
class DescriptorPool;

class DescriptorManager final {
private:
    std::unique_ptr<DescriptorPool>      m_pool;
    std::unique_ptr<DescriptorSetLayout> m_set_layout;
    std::unique_ptr<DescriptorSets>      m_sets;

public:
    DescriptorManager(DescriptorManager&&) = delete;
    DescriptorManager(const Device& device, const std::vector<DescriptorBinding>& bindings, size_t max_sets);
    ~DescriptorManager();

    const DescriptorSetLayout& descriptor_set_layout() const { return *m_set_layout; }
    DescriptorSets&            descriptor_sets() const { return *m_sets; }
};
} // namespace Vulkan
