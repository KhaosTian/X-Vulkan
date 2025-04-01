#pragma once

#include <Vulkan/Device.hpp>
#include <Vulkan/Vulkan.hpp>

namespace Vulkan {
class Device;

class Fence final {
private:
    VkFence       m_handle {};
    const Device& m_device;

public:
    explicit Fence(const Device& device, bool signaled = false);
    Fence(Fence&& ohter) noexcept;
    ~Fence();

public:
    const VkFence& handle() const { return m_handle; }

    void Wait(const uint64_t timeout) const;
    void Reset() const;
    void QueryStatus() const;
};
} // namespace Vulkan