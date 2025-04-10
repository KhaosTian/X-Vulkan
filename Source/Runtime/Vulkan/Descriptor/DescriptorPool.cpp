#include "Vulkan/Descriptor/DescriptorPool.hpp"
#include "Vulkan/Descriptor/DescriptorBinding.hpp"
#include "Vulkan/Core/Vulkan.hpp"
#include "Vulkan/Core/Device.hpp"

namespace Vulkan {
DescriptorPool::DescriptorPool(
    const Device&                         device,
    const std::vector<DescriptorBinding>& descriptor_bindings,
    size_t                                max_sets
):
    m_device(device) {
    std::vector<VkDescriptorPoolSize> pool_sizes;

    for (const auto& binding: descriptor_bindings) {
        VkDescriptorPoolSize size = {};
        size.type                 = binding.type;
        size.descriptorCount      = static_cast<uint32_t>(binding.descriptor_count * max_sets);

        pool_sizes.push_back(size);
    }

    VkDescriptorPoolCreateInfo create_info = {};
    create_info.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    create_info.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    create_info.poolSizeCount              = static_cast<uint32_t>(pool_sizes.size());
    create_info.maxSets                    = static_cast<uint32_t>(max_sets);
    create_info.pPoolSizes                 = pool_sizes.data();

    VK_CHECK(vkCreateDescriptorPool(m_device.handle(), &create_info, nullptr, &m_handle), "create descriptor pool");
}

DescriptorPool::~DescriptorPool() {
    if (m_handle == nullptr) return;
    vkDestroyDescriptorPool(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}
} // namespace Vulkan
