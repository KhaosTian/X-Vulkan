#include "Vulkan/DebugUtilsMessenger.hpp"
#include "Vulkan/Instance.hpp"

namespace Vulkan {
const char* ObjectTypeToString(const VkObjectType obj_type) {
    switch (obj_type) {
#define STR(e) \
    case VK_OBJECT_TYPE_##e: \
        return #e
        STR(UNKNOWN);
        STR(INSTANCE);
        STR(PHYSICAL_DEVICE);
        STR(DEVICE);
        STR(QUEUE);
        STR(SEMAPHORE);
        STR(COMMAND_BUFFER);
        STR(FENCE);
        STR(DEVICE_MEMORY);
        STR(BUFFER);
        STR(IMAGE);
        STR(EVENT);
        STR(QUERY_POOL);
        STR(BUFFER_VIEW);
        STR(IMAGE_VIEW);
        STR(SHADER_MODULE);
        STR(PIPELINE_CACHE);
        STR(PIPELINE_LAYOUT);
        STR(RENDER_PASS);
        STR(PIPELINE);
        STR(DESCRIPTOR_SET_LAYOUT);
        STR(SAMPLER);
        STR(DESCRIPTOR_POOL);
        STR(DESCRIPTOR_SET);
        STR(FRAMEBUFFER);
        STR(COMMAND_POOL);
        STR(SAMPLER_YCBCR_CONVERSION);
        STR(DESCRIPTOR_UPDATE_TEMPLATE);
        STR(SURFACE_KHR);
        STR(SWAPCHAIN_KHR);
        STR(DISPLAY_KHR);
        STR(DISPLAY_MODE_KHR);
        STR(DEBUG_REPORT_CALLBACK_EXT);
        STR(DEBUG_UTILS_MESSENGER_EXT);
        STR(ACCELERATION_STRUCTURE_KHR);
        STR(VALIDATION_CACHE_EXT);
        STR(PERFORMANCE_CONFIGURATION_INTEL);
        STR(DEFERRED_OPERATION_KHR);
        STR(INDIRECT_COMMANDS_LAYOUT_NV);
#undef STR

        default:
            return "unknown";
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    const VkDebugUtilsMessageSeverityFlagBitsEXT      message_serverity,
    const VkDebugUtilsMessageTypeFlagsEXT             meesage_type,
    const VkDebugUtilsMessengerCallbackDataEXT* const callback_data,
    void* const                                       user_data
) {
    (void)user_data;
    const auto attributes = C
}
} // namespace Vulkan