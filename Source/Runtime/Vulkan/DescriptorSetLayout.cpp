#include "Vulkan/DescriptorSetLayout.hpp"
#include "Vulkan/DescriptorBinding.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/Device.hpp"
#include <cstdint>

namespace Vulkan {
DescriptorSetLayout::DescriptorSetLayout(
    const Device&                         device,
    const std::vector<DescriptorBinding>& descriptor_bindings
):
    m_device(device) {
    std::vector<VkDescriptorSetLayoutBinding> layout_bindings;

    for (const auto& binding: descriptor_bindings) {
        VkDescriptorSetLayoutBinding layout_binding = {};
        layout_binding.binding                      = binding.binding,
        layout_binding.descriptorCount              = binding.descriptor_count;
        layout_binding.descriptorType               = binding.type;
        layout_binding.stageFlags                   = binding.stage;

        layout_bindings.push_back(layout_binding);
    }

    VkDescriptorSetLayoutCreateInfo create_info = {};
    create_info.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    create_info.bindingCount                    = static_cast<uint32_t>(layout_bindings.size());
    create_info.pBindings                       = layout_bindings.data();

    VK_CHECK(vkCreateDescriptorSetLayout(m_device.handle(), &create_info, nullptr, &m_handle), "create descriptor set layout");
}
DescriptorSetLayout::~DescriptorSetLayout() {
    if (m_handle == nullptr) return;
    vkDestroyDescriptorSetLayout(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}
} // namespace Vulkan