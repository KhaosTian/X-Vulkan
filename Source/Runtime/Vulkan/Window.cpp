#include "Window.hpp"
#include "Types.hpp"

namespace Vulkan {
Window::Window(const WindowConfig& config): m_config(config) {
    // glfw 初始化
    if (!glfwInit()) {
        throw std::runtime_error("failed to init glfw");
    }

    //  检查glfw对vulkan的持
    if (!glfwVulkanSupported()) {
        throw std::runtime_error("glfw vulkan don't supported");
    }

    // 初始化window hint
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, config.enable_resize ? GLFW_TRUE : GLFW_FALSE);

    // 获取显示器
    const GLFWmonitor* monitor = config.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    // 创建window
    m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("failed to create glfw window.");
    }

    // 显示模式
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode) {
        glfwSetWindowPos(m_window, (mode->width - config.width) / 2, (mode->height - config.height) / 2);
    }
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

VkExtent2D Window::GetWindowSize() const {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return VkExtent2D { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

std::vector<const char*> Window::GetRequiredInstanceExtensions() const {
    uint32_t                 extension_count     = 0;
    const char**             required_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    std::vector<const char*> extensions(required_extensions, required_extensions + extension_count);
    return extensions;
}

void Window::PollEvents() const {
    glfwPollEvents();
}
void Window::WaitEvents() const {
    glfwWaitEvents();
}
bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}
} // namespace Vulkan
