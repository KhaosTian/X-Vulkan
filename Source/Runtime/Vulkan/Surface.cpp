#include "Surface.hpp"
#include "Instance.hpp"
#include "Window.hpp"
#include <stdexcept>

namespace Vulkan {
Surface::Surface(const Instance& instance): m_instance(instance) {
    if (glfwCreateWindowSurface(instance.handle(), instance.window().Handle(), nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface.");
    }
}

Surface::~Surface() {
    if (m_surface == nullptr) {
        return;
    }

    vkDestroySurfaceKHR(m_instance.handle(), m_surface, nullptr);
    m_surface = nullptr;
}
} // namespace Vulkan
