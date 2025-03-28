#pragma once

#include "Vulkan.hpp"

namespace Vulkan {

class Instance;
class Window;

class Surface final {
    VULKAN_HANDLE(VkSurfaceKHR, m_vk_surface)
private:
    const Instance& m_instance;
    VkSurfaceKHR    m_surface;

public:
    NON_COPY(Surface)
    explicit Surface(const Instance& instance);
    ~Surface();

public:
    const Instance& instance() const { return m_instance; }
};

} // namespace Vulkan
