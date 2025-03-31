#pragma once

#include "GLFW/glfw3.h"
#include "Types.hpp"
#include "Vulkan.hpp"

#include <functional>

namespace Vulkan {
class WindowConfig;

class Window final {
    VULKAN_HANDLE(GLFWwindow*, m_window)
private:
    WindowConfig m_config;

public:
    NON_COPY(Window)
    explicit Window(const WindowConfig& config);
    ~Window();

public:
    const WindowConfig& config() const { return m_config; }

    VkExtent2D               GetWindowSize() const;
    std::vector<const char*> GetRequiredInstanceExtensions() const;

    void PollEvents() const;
    void WaitEvents() const;
    bool ShouldClose() const;
};

} // namespace Vulkan
