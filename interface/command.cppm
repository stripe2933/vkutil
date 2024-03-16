export module vkutil:command;

import std;
export import vulkan_hpp;

#define FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

export namespace vkutil{
    template <std::invocable<vk::CommandBuffer> F>
        requires (std::is_void_v<std::invoke_result_t<F, vk::CommandBuffer>>)
    auto executeSingleCommand(
        vk::Device device,
        vk::CommandPool commandPool,
        vk::Queue queue,
        F &&f,
        vk::Fence fence = {})
    -> void {
        const vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo {
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            1,
        })[0];

        commandBuffer.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
        std::invoke(FWD(f), commandBuffer);
        commandBuffer.end();

        queue.submit(vk::SubmitInfo {
            {},
            {},
            commandBuffer,
        }, fence);
        if (fence) {
            if (vk::Result waitResult = device.waitForFences(fence, vk::True, std::numeric_limits<std::uint64_t>::max());
                waitResult != vk::Result::eSuccess) {
                throw std::runtime_error { "Fence wait error: " + to_string(waitResult) };
            }
        }
        else {
            queue.waitIdle();
        }
    }

    template <std::invocable<vk::CommandBuffer> F, typename R = std::invoke_result_t<F, vk::CommandBuffer>>
        requires (!std::is_void_v<R>)
    [[nodiscard]] auto executeSingleCommand(
        vk::Device device,
        vk::CommandPool commandPool,
        vk::Queue queue,
        F &&f,
        vk::Fence fence = {})
    -> R {
        const vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo {
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            1,
        })[0];

        commandBuffer.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
        R result = std::invoke(FWD(f), commandBuffer);
        commandBuffer.end();

        queue.submit(vk::SubmitInfo {
            {}, {},
            commandBuffer,
        }, fence);

        if (fence) {
            if (vk::Result waitResult = device.waitForFences(fence, vk::True, std::numeric_limits<std::uint64_t>::max());
                waitResult != vk::Result::eSuccess) {
                throw std::runtime_error { "Fence wait error: " + to_string(waitResult) };
            }
        }
        else {
            queue.waitIdle();
        }
        return result;
    }
}