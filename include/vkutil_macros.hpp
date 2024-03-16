#pragma once

#define VKUTIL_NAMED_DESCRIPTOR_INFO(DescriptorSetCount, ...) \
    struct DescriptorSetLayouts : vkutil::DescriptorSetLayouts<DescriptorSetCount> { \
        vk::DescriptorSetLayout __VA_ARGS__; \
        \
        explicit DescriptorSetLayouts( \
            std::array<vk::raii::DescriptorSetLayout, DescriptorSetCount> descriptorSetLayouts) \
            : vkutil::DescriptorSetLayouts<DescriptorSetCount> { std::move(descriptorSetLayouts) } { \
            std::tie(__VA_ARGS__) = *this; \
        } \
    }; \
    \
    struct DescriptorSets : vkutil::DescriptorSets<DescriptorSetLayouts> { \
        vk::DescriptorSet __VA_ARGS__; \
        \
        DescriptorSets( \
            vk::Device device, \
            vk::DescriptorPool descriptorPool, \
            const DescriptorSetLayouts &descriptorSetLayouts) \
                : vkutil::DescriptorSets<DescriptorSetLayouts> { device, descriptorPool, descriptorSetLayouts } { \
            std::tie(__VA_ARGS__) = *this; \
        } \
    };
