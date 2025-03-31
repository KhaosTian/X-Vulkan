#pragma once

#include "Device.hpp"
#include "Vulkan.hpp"

namespace Vulkan {
class Fence final {
private:
    VkFence       m_vk_fence {};
    const Device& m_device;

public:
    explicit Fence(const Device& device, bool signaled = false);
    Fence(Fence&& ohter) noexcept;
    Fence& operator=(Fence&& other)      = delete;
    Fence(const Fence& other)            = delete;
    Fence& operator=(const Fence& other) = delete;
    ~Fence();

public:
    const VkFence& handle() const { return m_vk_fence; }

    void Wait() const;
    void Reset() const;
    void QueryStatus() const;
};
} // namespace Vulkan