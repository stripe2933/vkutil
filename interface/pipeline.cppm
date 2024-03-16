export module vkutil:pipeline;

import std;
export import vulkan_hpp;
import :ref_holder;

export namespace vkutil{
    [[nodiscard]] auto getShaderModuleCreateInfo(
        const std::filesystem::path &path)
    -> RefHolder<vk::ShaderModuleCreateInfo, std::vector<std::byte>> {
        std::ifstream file { path, std::ios::ate | std::ios::binary };
        if (!file.is_open()) {
            throw std::runtime_error { "Failed to open file." };
        }

        const std::size_t fileSize = file.tellg();
        std::vector<std::byte> buffer(fileSize);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

        return { [](std::span<const std::byte> code) {
            return vk::ShaderModuleCreateInfo {
                 {},
                 code.size_bytes(),
                 reinterpret_cast<const std::uint32_t*>(code.data()),
             };
        }, std::move(buffer) };
    }
}