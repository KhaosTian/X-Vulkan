#pragma once

#include "Vulkan/Vulkan.hpp"
#include <cstdint>

namespace Vulkan {
struct DescriptorBinding {
    uint32_t           binding;
    uint32_t           descriptor_count;
    VkDescriptorType   type;
    VkShaderStageFlags stage;
}
} // namespace Vulkan
