#pragma once

#include "Vulkan.hpp"

namespace Vulkan {

class Instance;
class Window;

class Surface final {
public:
    Surface(Surface&&) = delete;
    explicit Surface(const Instance& instance);
    ~Surface();

    VkSurfaceKHR    handle() const { return m_surface; }
    const Instance& instance() const { return m_instance; }

private:
    const Instance& m_instance;
    VkSurfaceKHR    m_surface;
};

} // namespace Vulkan
