#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
struct WindowConfig {
    uint32_t    width;
    uint32_t    height;
    std::string title;
    bool        is_fullscreen;
    bool        enable_resize;
};

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

} // namespace Vulkan
