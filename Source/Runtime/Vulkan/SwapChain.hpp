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
    VULKAN_HANDLE(VkSwapchainKHR, m_vk_swapchain)
private:
    const Device&                           m_device;
    const VkPhysicalDevice                  m_vk_physical_device { nullptr };
    std::vector<VkImage>                    m_images {};
    std::vector<std::unique_ptr<ImageView>> m_image_views {};
    VkPresentModeKHR                        m_present_mode {};
    SwapchainConfig                         m_config {};

public:
    NON_COPY(SwapChain)
    SwapChain(const Device& device, const VkPresentModeKHR present_mode);
    ~SwapChain();

public:
    const Device&          device() const { return m_device; }
    const VkPhysicalDevice vk_physical_device() const { return m_vk_physical_device; }

private:
    static SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface);
    static VkSurfaceFormatKHR      ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats);
    static VkPresentModeKHR
    ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& present_modes, const VkPresentModeKHR desired_mode);
    static VkExtent2D ChooseSwapExtent(const Window& window, const VkSurfaceCapabilitiesKHR& capabilities);
    static uint32_t   ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
};
} // namespace Vulkan
