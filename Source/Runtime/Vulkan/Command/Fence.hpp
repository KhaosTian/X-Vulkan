#pragma once

#include "Vulkan/Core/Device.hpp"
#include "Vulkan/Core/Vulkan.hpp"

namespace Vulkan {
class Device;

class Fence final {
public:
    explicit Fence(const Device& device, bool signaled = false);
    Fence(Fence&& ohter) noexcept;
    ~Fence();

    const VkFence& handle() const { return m_handle; }

    void Wait(const uint64_t timeout) const;
    void Reset() const;
    void QueryStatus() const;

private:
    VkFence       m_handle {};
    const Device& m_device;
};
} // namespace Vulkan
