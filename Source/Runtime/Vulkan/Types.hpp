#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
struct QueueFamilyIndices {
    uint32_t graphics = UINT32_MAX;
    uint32_t present  = UINT32_MAX;
    uint32_t compute  = UINT32_MAX;

    bool IsComplete() const { return (graphics != UINT32_MAX && present != UINT32_MAX); }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

struct SwapchainConfig {
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR   present_mode;
    VkExtent2D         extent;
    uint32_t           image_count;
};
} // namespace Vulkan
