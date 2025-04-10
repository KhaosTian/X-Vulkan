#include "Sampler.hpp"
#include "Device.hpp"
#include "Exception.hpp"
#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
Sampler::Sampler(const Device& device, const SamplerConfig& config):
    m_device(device) {
    VkSamplerCreateInfo create_info = {};

    create_info.sType     = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    create_info.magFilter = config.mag_filter;
    create_info.minFilter = config.min_filter;

    create_info.addressModeU = config.address_mode_u;
    create_info.addressModeV = config.address_mode_v;
    create_info.addressModeW = config.address_mode_w;

    create_info.anisotropyEnable = config.enable_anisotropy;
    create_info.maxAnisotropy    = config.max_anisotropy;

    create_info.borderColor = config.border_color;

    create_info.unnormalizedCoordinates = config.unnormalized_coordinates;

    create_info.compareEnable = config.enable_compare;
    create_info.compareOp     = config.compare_op;

    create_info.mipmapMode = config.mipmap_mode;
    create_info.mipLodBias = config.mip_load_bias;

    create_info.minLod = config.min_lod;
    create_info.maxLod = config.max_lod;

    VK_CHECK(vkCreateSampler(m_device.handle(), &create_info, nullptr, &m_handle), "create sampler");
}

Sampler::~Sampler() {
    if (m_handle == nullptr) return;
    vkDestroySampler(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}
} // namespace Vulkan
