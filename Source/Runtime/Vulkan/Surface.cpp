#include "Surface.hpp"
#include "Instance.hpp"
#include "Vulkan.hpp"
#include "Window.hpp"

namespace Vulkan {
Surface::Surface(const Instance& instance): m_instance(instance) {
    VK_CHECK(
        glfwCreateWindowSurface(instance.handle(), instance.window().handle(), nullptr, &m_surface),
        "create window surface"
    );
}

Surface::~Surface() {
    if (m_surface == nullptr) return;

    vkDestroySurfaceKHR(m_instance.handle(), m_surface, nullptr);
    m_surface = nullptr;
}
} // namespace Vulkan
