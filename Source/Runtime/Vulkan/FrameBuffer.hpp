#pragma once

#include "Vulkan/Vulkan.hpp"

namespace Vulkan {
class ImageView;
class RenderPass;
class Device;

class Framebuffer final {
private:
    const Device& m_device;
    VkFramebuffer m_handle;

public:
    Framebuffer(Framebuffer&&) noexcept;
    Framebuffer(const ImageView& image_view, const RenderPass& render_pass, bool with_ds);
    Framebuffer(
        const ImageView&  image_view,
        const ImageView&  image_view1,
        const ImageView&  image_view2,
        const RenderPass& render_pass
    );
    ~Framebuffer();

    VkFramebuffer handle() const { return m_handle; }
    const Device& device() const { return m_device; }
};

} // namespace Vulkan