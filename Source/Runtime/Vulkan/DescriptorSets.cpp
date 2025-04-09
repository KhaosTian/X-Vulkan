#include "Vulkan/DescriptorSets.hpp"
#include "Vulkan/DescriptorPool.hpp"
#include "Vulkan/DescriptorSetLayout.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/Exception.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace Vulkan {
DescriptorSets::DescriptorSets(const DescriptorPool& pool, const DescriptorSetLayout& layout, std::map<uint32_t, VkDescriptorType> binding_types, size_t size):
    m_descriptor_pool(pool),
    m_binding_types(std::move(binding_types)) {
    std::vector<VkDescriptorSetLayout> layouts(size, layout.handle());

    VkDescriptorSetAllocateInfo allocate_info = {};
    allocate_info.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocate_info.descriptorPool              = pool.handle();
    allocate_info.descriptorSetCount          = static_cast<uint32_t>(size);
    allocate_info.pSetLayouts                 = layouts.data();

    m_descriptor_sets.resize(size);

    VK_CHECK(vkAllocateDescriptorSets(m_descriptor_pool.device().handle(), nullptr, m_descriptor_sets.data()), "create descriptor sets");
}

VkWriteDescriptorSet DescriptorSets::Bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& buffer_info, uint32_t count) const {
    VkWriteDescriptorSet descriptor_write = {};
    descriptor_write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet               = m_descriptor_sets[index];
    descriptor_write.dstBinding           = binding;
    descriptor_write.dstArrayElement      = 0;
    descriptor_write.descriptorType       = GetBindingType(binding);
    descriptor_write.descriptorCount      = count;
    descriptor_write.pBufferInfo          = &buffer_info;

    return descriptor_write;
}

VkWriteDescriptorSet DescriptorSets::Bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& buffer_info, uint32_t count) const {
    VkWriteDescriptorSet descriptor_write = {};
    descriptor_write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet               = m_descriptor_sets[index];
    descriptor_write.dstBinding           = binding;
    descriptor_write.dstArrayElement      = 0;
    descriptor_write.descriptorType       = GetBindingType(binding);
    descriptor_write.descriptorCount      = count;
    descriptor_write.pImageInfo           = &buffer_info;

    return descriptor_write;
}

void DescriptorSets::UpdateDescriptors(uint32_t index, const std::vector<VkWriteDescriptorSet>& descriptor_writes) {
    vkUpdateDescriptorSets(m_descriptor_pool.device().handle(), static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
}

VkDescriptorType DescriptorSets::GetBindingType(uint32_t binding) const {
    const auto it = m_binding_types.find(binding);
    if (it != m_binding_types.end()) return it->second;
    Throw(std::invalid_argument("binding not found"));
}

} // namespace Vulkan