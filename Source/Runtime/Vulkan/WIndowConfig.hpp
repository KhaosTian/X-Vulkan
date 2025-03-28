#pragma once

#include <cstdint>
#include <string>

namespace Vulkan {
struct WindowConfig {
    uint32_t    width;
    uint32_t    height;
    std::string title;
    bool        is_fullscreen;
    bool        enable_resize;
};
} // namespace Vulkan