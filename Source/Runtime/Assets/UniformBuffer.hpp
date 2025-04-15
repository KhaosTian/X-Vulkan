#pragma once

#include <memory>

namespace Vulkan {
class Device;
class Buffer;
class DeviceMemory;

class UniformBuffer final {
public:
    UniformBuffer(UniformBuffer&&) noexcept;
    UniformBuffer(const Device&);
    ~UniformBuffer();

    const Buffer& handle() const { return *m_handle; }
    void          SetValue(const void* data, size_t size);

private:
    const Device&                 m_device;
    std::unique_ptr<Buffer>       m_handle;
    std::unique_ptr<DeviceMemory> m_memory;
};
} // namespace Vulkan
