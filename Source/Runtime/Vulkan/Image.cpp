
#include "Vulkan/CommandPool.hpp"
#include "Vulkan/DepthBuffer.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/Buffer.hpp"
#include "Vulkan/DeviceMemory.hpp"
#include "Vulkan/CommandBuffer.hpp"
#include <stdexcept>
#include "Vulkan/Image.hpp"
#include "Vulkan/Exception.hpp"

namespace Vulkan {
Image::Image(const Device& device, VkExtent2D extent, uint32_t mip_level, VkFormat format):
    Image(
        device,
        extent,
        mip_level,
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    ) {
}
Image::Image(
    const Device&     device,
    VkExtent2D        extent,
    uint32_t          mip_level,
    VkFormat          format,
    VkImageTiling     tiling,
    VkImageUsageFlags usage
):
    m_device(device),
    m_extent(extent),
    m_format(format),
    m_image_layout(VK_IMAGE_LAYOUT_UNDEFINED) {
    VkImageCreateInfo create_info = {};
    create_info.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.imageType         = VK_IMAGE_TYPE_2D;
    create_info.extent.width      = extent.width;
    create_info.extent.height     = extent.height;
    create_info.extent.depth      = 1;
    create_info.mipLevels         = mip_level;
    create_info.arrayLayers       = 1;
    create_info.format            = format;
    create_info.tiling            = tiling;
    create_info.initialLayout     = m_image_layout;
    create_info.usage             = usage;
    create_info.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    create_info.samples           = VK_SAMPLE_COUNT_1_BIT;
    create_info.flags             = 0;

    VK_CHECK(vkCreateImage(m_device.handle(), &create_info, nullptr, &m_handle), "create image");
}

Image::~Image() {
    if (m_handle == nullptr) return;
    vkDestroyImage(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}

DeviceMemory Image::AllocateMemory(VkMemoryPropertyFlags property) const {
    const auto   requirements = GetMemoryRequirements();
    DeviceMemory memory(m_device, requirements.size, requirements.memoryTypeBits, 0, property);
    VK_CHECK(vkBindImageMemory(m_device.handle(), m_handle, memory.handle(), 0), "bind image memory");

    return memory;
}

VkMemoryRequirements Image::GetMemoryRequirements() const {
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(m_device.handle(), m_handle, &requirements);
    return requirements;
}

void Image::CopyFrom(CommandPool& cmd_pool, const Buffer& buffer) {
    CommandBuffer::Submit(cmd_pool, [&](VkCommandBuffer cmd) {
        VkBufferImageCopy copy_region               = {};
        copy_region.bufferOffset                    = 0;
        copy_region.bufferRowLength                 = 0;
        copy_region.bufferImageHeight               = 0;
        copy_region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_region.imageSubresource.mipLevel       = 0;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount     = 1;
        copy_region.imageOffset                     = {
            0,
            0,
            0,
        };
        copy_region.imageExtent = { m_extent.width, m_extent.height, 1 };

        vkCmdCopyBufferToImage(cmd, buffer.handle(), m_handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    });
}

void Image::TransitionImageLayout(CommandPool& cmd_pool, VkImageLayout new_layout) {
    // 提交一次性命令缓冲区执行布局转换（适用于初始化/加载时操作）
    CommandBuffer::Submit(cmd_pool, [&](VkCommandBuffer cmd) {
        VkImageMemoryBarrier barrier            = {};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout                       = m_image_layout; // 当前图像布局
        barrier.newLayout                       = new_layout; // 目标图像布局
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED; // 不需要队列族所有权转移
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = m_handle; // 目标VkImage对象
        barrier.subresourceRange.baseMipLevel   = 0; // 基础mipmap级别
        barrier.subresourceRange.levelCount     = 1; // 单个mipmap级别
        barrier.subresourceRange.baseArrayLayer = 0; // 基础数组层
        barrier.subresourceRange.layerCount     = 1; // 单个数组层

        // 根据目标布局设置图像aspect
        if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            // 包含stencil aspect的格式需要特殊处理（如D24_UNORM_S8_UINT）
            if (HasStencilComponent(m_format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        } else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // 默认颜色aspect
        }

        // 定义管线阶段和访问权限掩码
        VkPipelineStageFlags src_stage;
        VkPipelineStageFlags dst_stage;

        /* 处理不同布局转换场景的同步策略：
         * 1. 初始化传输：从undefined状态准备接收像素数据
         * 2. 传输到着色器读取：完成传输后转为可采样状态
         * 3. 深度附件初始化：准备深度测试所需状态 */
        if (m_image_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            // 不需要等待先前操作，直接允许传输写入
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            src_stage             = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; // 最早可能的阶段
            dst_stage             = VK_PIPELINE_STAGE_TRANSFER_BIT; // 传输操作阶段
        } else if (m_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            // 确保传输完成后再允许着色器读取
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            src_stage             = VK_PIPELINE_STAGE_TRANSFER_BIT; // 等待传输完成
            dst_stage             = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; // 片段着色器使用阶段
        } else if (m_image_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
                   new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            // 深度附件需要读写权限，同步到early-z片段测试阶段
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; // 最早可能的阶段
            dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // early-z深度测试阶段
        } else {
            Throw(std::invalid_argument("unsupported layout transition"));
        }

        // 提交内存屏障命令（隐含命令缓冲区记录）
        vkCmdPipelineBarrier(cmd, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    });

    // 更新当前布局状态
    m_image_layout = new_layout;
}

bool Image::HasStencilComponent(const VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

} // namespace Vulkan