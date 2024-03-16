export module vkutil:allocator;

import std;
export import vulkan_hpp;
import :details;

export namespace vkutil{
    class DeviceMemoryAllocator {
    public:
        explicit DeviceMemoryAllocator(
            vk::PhysicalDevice physicalDevice)
            : availableMemoryTypes { physicalDevice.getMemoryProperties().memoryTypes } { }
        DeviceMemoryAllocator(const DeviceMemoryAllocator&) = delete;
        DeviceMemoryAllocator(DeviceMemoryAllocator &&) noexcept = default;
        auto operator=(const DeviceMemoryAllocator&) -> DeviceMemoryAllocator& = delete;
        auto operator=(DeviceMemoryAllocator&&) noexcept -> DeviceMemoryAllocator& = default;

        [[nodiscard]] auto allocate(
            const vk::raii::Device &device,
            const vk::MemoryRequirements &memoryRequirements,
            vk::MemoryPropertyFlags memoryPropertyFlags) const
        -> vk::raii::DeviceMemory {
            const std::uint32_t memoryTypeIndex = [&] {
                for (auto [idx, memoryType] : availableMemoryTypes | ranges::views::enumerate<std::uint32_t>) {
                    if ((memoryRequirements.memoryTypeBits & (1 << idx)) && ((memoryType.propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)) {
                        return idx;
                    }
                }
                throw std::runtime_error { "Failed to find suitable memory type." };
            }();

            return { device, vk::MemoryAllocateInfo {
                memoryRequirements.size,
                memoryTypeIndex,
            } };
        }

    private:
        vk::ArrayWrapper1D<vk::MemoryType, 32> availableMemoryTypes;
    };
};