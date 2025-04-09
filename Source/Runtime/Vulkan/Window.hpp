#pragma once

#include <GLFW/glfw3.h>
#include "Vulkan/Vulkan.hpp"
#include <functional>

namespace Vulkan {
struct WindowConfig {
    uint32_t    width;
    uint32_t    height;
    std::string title;
    bool        is_fullscreen;
    bool        enable_resize;
};

class Window final {
private:
    WindowConfig m_config;
    GLFWwindow*  m_handle {};

public:
    Window(Window&&) = delete;
    explicit Window(const WindowConfig& config);
    ~Window();

public:
    GLFWwindow*         handle() const { return m_handle; }
    const WindowConfig& config() const { return m_config; }

    VkExtent2D               GetWindowSize() const;
    std::vector<const char*> GetRequiredInstanceExtensions() const;

    void PollEvents() const;
    void WaitEvents() const;
    bool ShouldClose() const;
};

} // namespace Vulkan
