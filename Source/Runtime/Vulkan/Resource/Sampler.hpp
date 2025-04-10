#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
class Device;

struct SamplerConfig final {
    VkFilter mag_filter = VK_FILTER_LINEAR;
    VkFilter min_filter = VK_FILTER_LINEAR;

    VkSamplerAddressMode address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    bool          enable_anisotropy = true;
    float         max_anisotropy    = 16;
    VkBorderColor border_color      = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    bool unnormalized_coordinates = false;

    bool        enable_compare = false;
    VkCompareOp compare_op     = VK_COMPARE_OP_ALWAYS;

    VkSamplerMipmapMode mipmap_mode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    float mip_load_bias = 0.0f;
    float min_lod       = 0.0f;
    float max_lod       = 0.0f;
};

class Sampler final {
    VkSampler m_handle;

public:
    VkSampler handle() const { return m_handle; }

private:
    const Device& m_device;

public:
    Sampler(Sampler&&) = delete;
    Sampler(const Device& device, const SamplerConfig& config);
    ~Sampler();
};

} // namespace Vulkan