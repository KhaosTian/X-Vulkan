#pragma once

#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Device;
class Semaphore final {
private:
    VkSemaphore   m_handle;
    const Device& m_device;

public:
    explicit Semaphore(const Device& device);
    Semaphore(Semaphore&& other) noexcept;
    ~Semaphore();

public:
    VkSemaphore handle() const { return m_handle; }
};
} // namespace Vulkan
