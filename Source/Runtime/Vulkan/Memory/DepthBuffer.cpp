#include "Vulkan/Memory/DepthBuffer.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Exception.hpp"
#include "Vulkan/Command/CommandPool.hpp"
#include "Vulkan/Memory/DeviceMemory.hpp"
#include "Vulkan/Memory/Image.hpp"
#include "Vulkan/Memory/ImageView.hpp"

namespace Vulkan {
DepthBuffer::DepthBuffer(CommandPool& cmd_pool, VkExtent2D extent):
    // 查找设备支持的深度格式
    m_format(FindDepthFormat(cmd_pool.device())),

    // 创建深度图像对象
    // 使用最优平铺方式 (Optimal Tiling) 提升GPU访问效率
    // 指定用途为深度/模板附件
    m_image(std::make_unique<Image>(
        cmd_pool.device(),
        extent,
        1, // Mip层级
        m_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    )),

    // 为深度图像分配设备本地内存
    // 必须在创建 ImageView 前完成内存分配，所以要注意变量的定义顺序
    m_image_memory(std::make_unique<DeviceMemory>(m_image->AllocateMemory(
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT // 优先使用GPU本地内存
    ))),

    // 创建深度图像的视图
    // 指定只关注深度部分，不含模板，即使格式可能包含模板
    m_image_view(std::make_unique<ImageView>(
        cmd_pool.device(),
        m_image->handle(), // 关联的Image对象
        m_format,
        VK_IMAGE_ASPECT_DEPTH_BIT // 仅深度部分
    )) {
    // 将图像布局转换为深度/模板附件最优布局
    // 确保图像在渲染管线中正确用作深度附件
    // 此操作通过命令缓冲区提交物理设备操作
    m_image->TransitionImageLayout(
        cmd_pool,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL // 目标布局
    );
}

DepthBuffer::~DepthBuffer() {
    m_image_view.reset();
    m_image.reset();
    m_image_memory.reset();
}

// 查找设备支持的深度格式（优先级：D32_SFLOAT_S8 > D24_S8 > D32）
VkFormat DepthBuffer::FindDepthFormat(const Device& device) {
    return FindSupportFormat(
        device,
        // 候选格式优先级（按设备支持广泛性排序）：
        {
            VK_FORMAT_D32_SFLOAT_S8_UINT, // 32位深度 + 8位模板（部分移动设备可能不支持）
            VK_FORMAT_D24_UNORM_S8_UINT, // 24位深度 + 8位模板（广泛支持）
            VK_FORMAT_D32_SFLOAT // 仅32位深度（无模板）
        },
        VK_IMAGE_TILING_OPTIMAL, // 必须为最优平铺
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT // 必须支持作为深度附件
    );
}
// 通用格式选择函数：遍历候选格式，返回第一个满足条件的格式
VkFormat DepthBuffer::FindSupportFormat(
    const Device&                device,
    const std::vector<VkFormat>& candidates, // 按优先级排序的候选列表
    const VkImageTiling          tiling, // 平铺方式需求
    const VkFormatFeatureFlags   feature_flags // 必须支持的特性标志
) {
    for (auto format: candidates) {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(device.physical_device(), format, &properties);
        // 检查线性平铺支持
        if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & feature_flags) == feature_flags) {
            return format;
        }
        // 检查最优平铺支持（深度缓冲通常需要此模式）
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & feature_flags) == feature_flags) {
            return format;
        }
    }
    // 若无候选格式被支持，抛出异常（通常意味着设备不满足需求）
    Throw(std::runtime_error("Failed to find supported format. Check if device supports any of the candidate formats.")
    );
}
} // namespace Vulkan
