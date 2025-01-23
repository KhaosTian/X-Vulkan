#pragma once

#include "Vulkan.hpp"

namespace Vulkan {

struct WindowData {
    int         width;
    int         height;
    std::string title;
    bool        is_fullscreen;
    bool        resizable;
    bool        hide_title;
};

class Window final {
public:
    Window(Window&&) = delete;
    explicit Window(const WindowData& data);
    ~Window();

    GLFWwindow*       Handle() const { return m_window; }
    const WindowData& Data() const { return m_data; }

    VkExtent2D               FramebufferSize() const;
    std::vector<const char*> QueryRequiredInstanceExtensions() const;

    void PollEvents() const;
    void WaitEvents() const;
    bool ShouldClose() const;

private:
    const WindowData m_data;
    GLFWwindow*      m_window { nullptr };
};

} // namespace Vulkan
