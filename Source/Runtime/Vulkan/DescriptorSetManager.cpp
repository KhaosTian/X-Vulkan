#include "Vulkan/DescriptorSetManager.hpp"
#include "Vulkan/DescriptorPool.hpp"
#include "Vulkan/DescriptorSetLayout.hpp"
#include "Vulkan/DescriptorBinding.hpp"
#include "Vulkan/DescriptorSets.hpp"
#include "Vulkan/Exception.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
DescriptorManager::DescriptorManager(const Device& device, const std::vector<DescriptorBinding>& bindings, size_t max_sets) {
    // 根据传入的bindings构建binding和type的映射
    std::map<uint32_t, VkDescriptorType> binding_types;
    for (const auto& binding: bindings) {
        const auto& result = binding_types.insert(std::make_pair(binding.binding, binding.type));
        if (!result.second) {
            Throw(std::invalid_argument("binding collosion"));
        }
    }

    // 初始化描述符池、描述符堆布局和描述符堆
    m_pool       = std::make_unique<DescriptorPool>(device, bindings, max_sets);
    m_set_layout = std::make_unique<DescriptorSetLayout>(device, bindings);
    m_sets       = std::make_unique<DescriptorSets>(*m_pool, *m_set_layout, binding_types, max_sets);
}

DescriptorManager::~DescriptorManager() {
    m_sets.reset();
    m_set_layout.reset();
    m_pool.reset();
}

} // namespace Vulkan