#include "Vulkan/DescriptorPool.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/Device.hpp"
#include <cstdint>
#include <vector>

namespace Vulkan {
DescriptorPool::DescriptorPool(
    const Device&                         device,
    const std::vector<DescriptorBinding>& descriptor_bindings,
    size_t                                max_sets
):
    m_device(device) {
    std::vector<VkDescriptorPoolSize> pool_sizes;

    for (const auto& binding: descriptor_bindings) {
        VkDescriptorPoolSize size = {
            .type            = binding.type,
            .descriptorCount = static_cast<uint32_t>(binding.descriptor_count * max_sets)
        };
        pool_sizes.emplace_back(size);
    }

    VkDescriptorPoolCreateInfo create_info = {};
    create_info.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    create_info.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    create_info.poolSizeCount              = static_cast<uint32_t>(pool_sizes.size());
    create_info.maxSets                    = static_cast<uint32_t>(max_sets);
    create_info.pPoolSizes                 = pool_sizes.data();

    VK_CHECK(vkCreateDescriptorPool(m_device.handle(), &create_info, nullptr, &m_handle), "create descriptor pool");

} // namespace Vulkan
} // namespace Vulkan