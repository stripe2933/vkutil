export module vkutil:descriptor;

import std;
export import vulkan_hpp;
import :details.ranges;

export namespace vkutil{
    template <std::size_t DescriptorSetLayoutCount>
    class DescriptorSetLayouts : public std::array<vk::DescriptorSetLayout, DescriptorSetLayoutCount> {
    public:
        static constexpr std::size_t layoutCount = DescriptorSetLayoutCount;

        explicit DescriptorSetLayouts(
            std::array<vk::raii::DescriptorSetLayout, DescriptorSetLayoutCount> descriptorSetLayouts)
            : backDescriptorSetLayouts { std::move(descriptorSetLayouts) } {
            std::apply([this](auto &...layouts) {
                static_cast<std::array<vk::DescriptorSetLayout, DescriptorSetLayoutCount>&>(*this) = { *layouts... };
            }, backDescriptorSetLayouts);
        }

    private:
        std::array<vk::raii::DescriptorSetLayout, DescriptorSetLayoutCount> backDescriptorSetLayouts;
    };

    template <typename DescriptorSetLayouts>
    struct DescriptorSets : std::array<vk::DescriptorSet, DescriptorSetLayouts::layoutCount> {
        DescriptorSets(
            vk::Device device,
            vk::DescriptorPool descriptorPool,
            const DescriptorSetLayouts &descriptorSetLayouts)
                : std::array<vk::DescriptorSet, DescriptorSetLayouts::layoutCount> {
                    device.allocateDescriptorSets(vk::DescriptorSetAllocateInfo {
                        descriptorPool,
                        descriptorSetLayouts,
                    })
                    | ranges::to_array<vk::DescriptorSet, DescriptorSetLayouts::layoutCount>
                } { }
    };
}