#include "Vulkan/Render/GraphicsPipeline.hpp"

#include "Vulkan/Descriptor/DescriptorBinding.hpp"
#include "Vulkan/Descriptor/DescriptorSetManager.hpp"
#include "Vulkan/Descriptor/DescriptorSetLayout.hpp"
#include "Vulkan/Descriptor/DescriptorSets.hpp"

#include "Vulkan/Resource/DepthBuffer.hpp"

#include "Vulkan/Render/PipelineLayout.hpp"

#include "Vulkan/Render/RenderPass.hpp"

#include "Vulkan/Resource/SwapChain.hpp"

#include "Vulkan/Render/ShaderModule.hpp"

#include "Vulkan/Core/Device.hpp"
#include <vector>

namespace Vulkan {

GraphicsPipeline::GraphicsPipeline(const Swapchain& swapchain, const DepthBuffer& depth_buffer): m_swapchain(swapchain) {
    // 初始化
    const auto& device = swapchain.device();

    // 顶点输入状态配置
    auto vertex_input_state                            = VkPipelineVertexInputStateCreateInfo {};
    vertex_input_state.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.vertexBindingDescriptionCount   = 0; //顶点绑定
    vertex_input_state.pVertexBindingDescriptions      = nullptr;
    vertex_input_state.vertexAttributeDescriptionCount = 0; //顶点属性
    vertex_input_state.pVertexAttributeDescriptions    = nullptr;

    // input assembly state create info，输入装配
    auto input_assembly_state                   = VkPipelineInputAssemblyStateCreateInfo {};
    input_assembly_state.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //图元类型
    input_assembly_state.primitiveRestartEnable = VK_FALSE; //禁用图元重启（用于带索引的绘制）

    // viewport，视口
    auto viewport     = VkViewport {};
    viewport.x        = 0.0f; // 视口左上角坐标
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(swapchain.extent().width); //  视口大小
    viewport.height   = static_cast<float>(swapchain.extent().height);
    viewport.minDepth = 0.0f; // 深度缓冲区深度
    viewport.maxDepth = 1.0f;

    // scissor 裁剪
    auto scissor   = VkRect2D {};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain.extent();

    // viewport state create info
    auto viewport_state          = VkPipelineViewportStateCreateInfo {};
    viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports    = &viewport;
    viewport_state.scissorCount  = 1;
    viewport_state.pScissors     = &scissor;

    // rasterization state create info
    auto rasterization_state = VkPipelineRasterizationStateCreateInfo {};

    rasterization_state.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state.depthClampEnable        = VK_FALSE; // 深度截取
    rasterization_state.rasterizerDiscardEnable = VK_FALSE; // 丢弃片元
    rasterization_state.lineWidth               = 1.0f; // 线宽
    rasterization_state.cullMode                = VK_CULL_MODE_BACK_BIT; // 背面剔除
    rasterization_state.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE; // 逆时针正面
    rasterization_state.depthBiasEnable         = VK_FALSE; // 深度偏移
    rasterization_state.depthBiasConstantFactor = 0.0f;
    rasterization_state.depthBiasClamp          = 0.0f;
    rasterization_state.depthBiasSlopeFactor    = 0.0f;

    // multisample state create info
    auto multisample_state                  = VkPipelineMultisampleStateCreateInfo {};
    multisample_state.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state.sampleShadingEnable   = VK_FALSE; // 采样着色
    multisample_state.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT; // 采样次数
    multisample_state.minSampleShading      = 1.0f; // 最小采样着色比例
    multisample_state.pSampleMask           = nullptr; // 采样掩码指针
    multisample_state.alphaToCoverageEnable = VK_FALSE; // 透明度到覆盖率？
    multisample_state.alphaToOneEnable      = VK_FALSE; // 透明度到1？

    // depth stencil state create info
    auto depth_stencil_state                  = VkPipelineDepthStencilStateCreateInfo {};
    depth_stencil_state.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_state.depthTestEnable       = VK_TRUE;
    depth_stencil_state.depthWriteEnable      = VK_TRUE;
    depth_stencil_state.depthCompareOp        = VK_COMPARE_OP_LESS;
    depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
    depth_stencil_state.stencilTestEnable     = VK_FALSE;
    depth_stencil_state.minDepthBounds        = 0.0f;
    depth_stencil_state.maxDepthBounds        = 1.0f;
    depth_stencil_state.stencilTestEnable     = VK_FALSE;
    depth_stencil_state.front                 = {};
    depth_stencil_state.back                  = {};

    // color blend attachment state
    auto color_blend_attachment_state                = VkPipelineColorBlendAttachmentState {};
    color_blend_attachment_state.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment_state.blendEnable         = VK_FALSE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.alphaBlendOp        = VK_BLEND_OP_ADD;

    // color blend state create info
    auto color_blend_state              = VkPipelineColorBlendStateCreateInfo {};
    color_blend_state.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.logicOpEnable     = VK_FALSE;
    color_blend_state.logicOp           = VK_LOGIC_OP_COPY;
    color_blend_state.attachmentCount   = 1;
    color_blend_state.pAttachments      = &color_blend_attachment_state;
    color_blend_state.blendConstants[0] = 0.0f;
    color_blend_state.blendConstants[1] = 0.0f;
    color_blend_state.blendConstants[2] = 0.0f;
    color_blend_state.blendConstants[3] = 0.0f;

    // dynamic state create info
    std::vector<VkDynamicState> dynamic_states = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    auto                        dynamic_state  = VkPipelineDynamicStateCreateInfo {};
    dynamic_state.sType                        = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount            = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state.pDynamicStates               = dynamic_states.data();

    // descriptor set
    std::vector<DescriptorBinding> descriptor_bindings = { { 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT },
                                                           { 1, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT } };

    m_descriptor_set_manager = std::make_unique<DescriptorSetManager>(device, descriptor_bindings, 1);
    // push constant ranges

    // shader stage create info
    const ShaderModule vert_shader(device, "shaders/vert.spv");
    const ShaderModule frag_shader(device, "shaders/frag.spv");

    auto shader_stages = std::vector<VkPipelineShaderStageCreateInfo> { vert_shader.CreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT), frag_shader.CreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT) };

    m_pipeline_layout = std::make_unique<PipelineLayout>(device, m_descriptor_set_manager->descriptor_set_layout(), nullptr);
    m_render_pass     = std::make_unique<RenderPass>(swapchain, depth_buffer, VK_ATTACHMENT_LOAD_OP_CLEAR);

    // graphics pipeline create info
    auto graphics_pipeline                = VkGraphicsPipelineCreateInfo {};
    graphics_pipeline.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline.stageCount          = 2;
    graphics_pipeline.pStages             = shader_stages.data();
    graphics_pipeline.pVertexInputState   = &vertex_input_state;
    graphics_pipeline.pInputAssemblyState = &input_assembly_state;
    graphics_pipeline.pViewportState      = &viewport_state;
    graphics_pipeline.pRasterizationState = &rasterization_state;
    graphics_pipeline.pMultisampleState   = &multisample_state;
    graphics_pipeline.pDepthStencilState  = &depth_stencil_state;
    graphics_pipeline.pColorBlendState    = &color_blend_state;
    graphics_pipeline.pDynamicState       = &dynamic_state;
    graphics_pipeline.layout              = m_pipeline_layout->handle();
    graphics_pipeline.renderPass          = m_render_pass->handle();
    graphics_pipeline.subpass             = 0;
    graphics_pipeline.basePipelineHandle  = nullptr;
    graphics_pipeline.basePipelineIndex   = -1;

    VK_CHECK(vkCreateGraphicsPipelines(device.handle(), nullptr, 1, &graphics_pipeline, nullptr, &m_handle), "create graphics pipeline");
}

GraphicsPipeline::~GraphicsPipeline() {
    if (m_handle == nullptr) return;

    vkDestroyPipeline(m_swapchain.device().handle(), m_handle, nullptr);
    m_handle = nullptr;

    m_swap_render_pass.reset();
    m_render_pass.reset();
    m_pipeline_layout.reset();
    m_descriptor_set_manager.reset();
}

VkDescriptorSet GraphicsPipeline::descriptor_set(uint32_t index) const {
    return m_descriptor_set_manager->descriptor_sets().handle(index);
}

} // namespace Vulkan