export module vkutil:image;

import std;
export import vulkan_hpp;
import :allocator;

export namespace vkutil{
    struct Image {
        vk::Image image;
        vk::Extent3D extent;
        vk::Format format;
        std::uint32_t arrayLayers;
        std::uint32_t mipLevels;

        operator vk::Image() const noexcept { return image; }
    };

    class AllocatedImage : public Image {
    public:
        AllocatedImage(
            const vk::raii::Device &device,
            const DeviceMemoryAllocator &allocator,
            const vk::ImageCreateInfo &imageCreateInfo,
            vk::MemoryPropertyFlags memoryPropertyFlags)
            : Image { nullptr, imageCreateInfo.extent, imageCreateInfo.format, imageCreateInfo.arrayLayers, imageCreateInfo.mipLevels },
              backImage { device, imageCreateInfo },
              deviceMemory { allocator.allocate(device, backImage.getMemoryRequirements(), memoryPropertyFlags) } {
            backImage.bindMemory(*deviceMemory, 0);
            image = *backImage;
        }
        AllocatedImage(const AllocatedImage&) = delete;
        AllocatedImage(AllocatedImage &&) noexcept = default;
        auto operator=(const AllocatedImage&) -> AllocatedImage& = delete;
        auto operator=(AllocatedImage&&) noexcept -> AllocatedImage& = default;

    protected:
        vk::raii::Image backImage;
        vk::raii::DeviceMemory deviceMemory;
    };
}