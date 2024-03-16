export module vkutil:buffer;

import std;
export import vulkan_hpp;
import :allocator;

export namespace vkutil{
    struct Buffer {
        vk::Buffer buffer;
        vk::DeviceSize size;

        operator vk::Buffer() const noexcept { return buffer; }
    };

    class AllocatedBuffer : public Buffer {
    public:
        AllocatedBuffer(
            const vk::raii::Device &device,
            const DeviceMemoryAllocator &allocator,
            const vk::BufferCreateInfo &bufferCreateInfo,
            vk::MemoryPropertyFlags memoryPropertyFlags)
            : Buffer { nullptr, bufferCreateInfo.size },
              backBuffer { device, bufferCreateInfo },
              deviceMemory { allocator.allocate(device, backBuffer.getMemoryRequirements(), memoryPropertyFlags) } {
            backBuffer.bindMemory(*deviceMemory, 0);
            buffer = *backBuffer;
        }
        AllocatedBuffer(const AllocatedBuffer&) = delete;
        AllocatedBuffer(AllocatedBuffer &&) noexcept = default;
        auto operator=(const AllocatedBuffer&) -> AllocatedBuffer& = delete;
        auto operator=(AllocatedBuffer&&) noexcept -> AllocatedBuffer& = default;

    protected:
        vk::raii::Buffer backBuffer;
        vk::raii::DeviceMemory deviceMemory;
    };

    class PersistentMappedBuffer : public AllocatedBuffer {
    public:
        std::byte *data;

        PersistentMappedBuffer(
            const vk::raii::Device &device,
            const DeviceMemoryAllocator &allocator,
            const vk::BufferCreateInfo &bufferCreateInfo,
            vk::MemoryPropertyFlags memoryPropertyFlags)
            : AllocatedBuffer { device, allocator, bufferCreateInfo, memoryPropertyFlags },
              data { static_cast<std::byte*>((*device).mapMemory(*deviceMemory, 0, bufferCreateInfo.size)) } { }
        PersistentMappedBuffer(const PersistentMappedBuffer&) = delete;
        PersistentMappedBuffer(PersistentMappedBuffer&&) noexcept = default;
        auto operator=(const PersistentMappedBuffer&) -> PersistentMappedBuffer& = delete;
        auto operator=(PersistentMappedBuffer&&) noexcept -> PersistentMappedBuffer& = default;

        ~PersistentMappedBuffer() {
            backBuffer.getDevice().unmapMemory(*deviceMemory);
        }
    };
}