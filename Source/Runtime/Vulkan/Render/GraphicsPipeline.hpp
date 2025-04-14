#pragma once

#include "Vulkan/Core/Vulkan.hpp"
#include <memory>

namespace Vulkan {
class DepthBuffer;
class PipelineLayout;
class RenderPass;
class Swapchain;
class DescriptorSetManager;

class GraphicsPipeline final {
public:
    GraphicsPipeline(GraphicsPipeline&&) = delete;
    GraphicsPipeline(const Swapchain& swapchain, const DepthBuffer& depth_buffer);
    ~GraphicsPipeline();

    VkPipeline            handle() const { return m_handle; }
    VkDescriptorSet       descriptor_set(uint32_t index) const;
    const PipelineLayout& pipeline_layout() const { return *m_pipeline_layout; }
    const RenderPass&     render_pass() const { return *m_render_pass; }
    const RenderPass&     swap_render_pass() const { return *m_swap_render_pass; }

private:
    VkPipeline       m_handle;
    const Swapchain& m_swapchain;

    std::unique_ptr<DescriptorSetManager> m_descriptor_set_manager;
    std::unique_ptr<PipelineLayout>       m_pipeline_layout;
    std::unique_ptr<RenderPass>           m_render_pass;
    std::unique_ptr<RenderPass>           m_swap_render_pass;
};

} // namespace Vulkan