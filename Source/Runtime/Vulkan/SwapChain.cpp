
#include "SwapChain.hpp"
#include "Device.hpp"
#include "Surface.hpp"
#include "Instance.hpp"
#include "Window.hpp"
#include "ImageView.hpp"
#include <cstdint>

namespace Vulkan {

SwapChain::SwapChain(const Device& device, const VkPresentModeKHR desired_mode):
    m_device(device),
    m_physical_device(device.physical_device()) {
    const auto& surface = m_device.surface();
    const auto& window  = surface.instance().window();

    const auto details      = QuerySwapChainSupport(surface.handle());
    m_config.surface_format = ChooseSwapSurfaceFormat(details.formats);
    m_config.present_mode   = ChooseSwapPresentMode(details.present_modes, desired_mode);
    m_config.extent         = ChooseSwapExtent(window, details.capabilities);
    m_config.image_count    = ChooseImageCount(details.capabilities);

    CreateSwapchain(surface.handle(), details, m_config);
    CreateImageViews(m_swapchain);
}

SwapChain::~SwapChain() {
}

SwapchainSupportDetails SwapChain::QuerySwapChainSupport(const VkSurfaceKHR surface) const {
    SwapchainSupportDetails details = {};

    // 枚举设备表面能力
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, surface, &details.capabilities);
    // 枚举设备表面格式
    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, nullptr);
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, details.formats.data());
    // 枚举设备表面呈现模式
    uint32_t mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &mode_count, nullptr);
    details.present_modes.resize(mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &mode_count, details.present_modes.data());

    return details;
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats) const {
    // 优先选择B8G8R8A8_SRGB 和 SRGB_NONLINEAR_KHR
    for (const auto& surface_format: surface_formats) {
        if (surface_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return surface_format;
        }
    }
    // 如果找不到，则选择第一个
    return surface_formats.front();
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& present_modes,
    const VkPresentModeKHR               desired_mode
) const {
    // 优先选择用户期望的模式
    for (const auto& present_mode: present_modes) {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return present_mode;
        }
    }
    // 其次选择MAILBOX模式
    for (const auto& present_mode: present_modes) {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }
    // 如果找不到，则选择第一个
    return present_modes.front();
}

VkExtent2D SwapChain::ChooseSwapExtent(const Window& window, const VkSurfaceCapabilitiesKHR& capabilities) {
    // 只有当前宽高为UINT32_MAX时，才表示宽高可以由我们自己决定
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    // 选择窗口大小
    VkExtent2D actual_extent = window.FramebufferSize();
    // 裁剪到表面支持的图像最小和最大尺寸之间
    actual_extent.width =
        std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
    actual_extent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actual_extent.height)
    );

    return actual_extent;
}

uint32_t SwapChain::ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities) {
    // 选择图像数量
    // 至少为1，最大为表面支持的最大图像数量
    uint32_t image_count = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
        image_count = capabilities.maxImageCount;
    }
    return image_count;
}

void SwapChain::CreateSwapchain(
    VkSurfaceKHR                   surface,
    const SwapchainSupportDetails& details,
    const SwapchainConfig&         config
) {
    // 创建交换链创建信息
    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface                  = surface;
    create_info.presentMode              = config.present_mode;
    create_info.minImageCount            = config.image_count;
    create_info.imageFormat              = config.surface_format.format;
    create_info.imageColorSpace          = config.surface_format.colorSpace;

    create_info.imageArrayLayers = 1;
    create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    create_info.imageExtent      = config.extent;

    create_info.preTransform   = details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    create_info.clipped      = VK_TRUE;
    create_info.oldSwapchain = nullptr;

    // 获取队列族索引
    const auto& queue_indices           = m_device.queue_family_indices();
    uint32_t    queue_family_indices[3] = { queue_indices.graphics, queue_indices.present, queue_indices.compute };

    // 如果图形队列和呈现队列不同，则使用并发模式
    if (queue_indices.graphics != queue_indices.present) {
        create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices   = queue_family_indices;
    }
    // 否则使用独占模式
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // 创建交换链
    if (vkCreateSwapchainKHR(m_device.handle(), &create_info, nullptr, &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
}

void SwapChain::CreateImageViews(const VkSwapchainKHR swapchain) {
    // 枚举交换链图像
    uint32_t image_count = 0;
    vkGetSwapchainImagesKHR(m_device.handle(), swapchain, &image_count, nullptr);
    m_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_device.handle(), swapchain, &image_count, m_images.data());

    // 创建交换链图像视图
    m_image_views.reserve(image_count);
    for (const auto& image: m_images) {
        m_image_views.emplace_back(
            std::make_unique<ImageView>(m_device, image, m_config.surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT)
        );
    }
}

} // namespace Vulkan
