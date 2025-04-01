#pragma once

#include <Vulkan/Vulkan.hpp>
#include <Vulkan/Types.hpp>

namespace Vulkan {
class Device;
class ImageView;
class Window;

class SwapChain final {
private:
    VkSwapchainKHR                          m_handle {};
    const Device&                           m_device;
    const VkPhysicalDevice                  m_physical_device { nullptr };
    std::vector<VkImage>                    m_images {};
    std::vector<std::unique_ptr<ImageView>> m_image_views {};
    VkPresentModeKHR                        m_present_mode {};
    SwapchainConfig                         m_config {};

public:
    SwapChain(SwapChain&&) = delete;
    SwapChain(const Device& device, const VkPresentModeKHR present_mode);
    ~SwapChain();

public:
    VkSwapchainKHR         handle() const { return m_handle; }
    const Device&          device() const { return m_device; }
    const VkPhysicalDevice vk_physical_device() const { return m_physical_device; }

private:
    static SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface);
    static VkSurfaceFormatKHR      ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats);
    static VkPresentModeKHR
    ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& present_modes, const VkPresentModeKHR desired_mode);
    static VkExtent2D ChooseSwapExtent(const Window& window, const VkSurfaceCapabilitiesKHR& capabilities);
    static uint32_t   ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
};
} // namespace Vulkan
