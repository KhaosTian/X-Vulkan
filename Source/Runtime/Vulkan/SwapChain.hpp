#pragma once

#include "Vulkan.hpp"
#include "Types.hpp"
#include <memory>
#include <vector>

namespace Vulkan {
class Device;
class ImageView;
class Window;

class SwapChain final {
public:
    SwapChain(const Device& device, const VkPresentModeKHR present_mode);
    ~SwapChain();
    SwapChain(SwapChain&&) = delete;

    VkSwapchainKHR GetHandle() const { return m_swapchain; }

private:
    VkSwapchainKHR                          m_swapchain { nullptr };
    const Device&                           m_device;
    const VkPhysicalDevice                  m_physical_device { nullptr };
    std::vector<VkImage>                    m_images {};
    std::vector<std::unique_ptr<ImageView>> m_image_views {};
    VkPresentModeKHR                        m_present_mode {};
    SwapchainConfig                         m_config {};

public:
private:
    SwapchainSupportDetails QuerySwapChainSupport(const VkSurfaceKHR surface) const;

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats) const;

    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& present_modes,
        const VkPresentModeKHR               desired_mode
    ) const;

    static VkExtent2D ChooseSwapExtent(const Window& window, const VkSurfaceCapabilitiesKHR& capabilities);

    static uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

    void CreateSwapchain(VkSurfaceKHR surface, const SwapchainSupportDetails& details, const SwapchainConfig& config);

    void CreateImageViews(const VkSwapchainKHR swapchain);
};
} // namespace Vulkan
