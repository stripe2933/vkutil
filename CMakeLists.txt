cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(vkutil
    LANGUAGES CXX
    DESCRIPTION
    "Abstracting frequently used Vulkan objects using RAII pattern."
)

# ----------------
# CMake options.
# ----------------

option(VKUTIL_USE_GLFW_WINDOW "Use GLFW window")

if (NOT DEFINED VKUTIL_VK_VERSION)
    set(VKUTIL_VK_VERSION 1000000)
endif()

# ----------------
# CMake scripts.
# ----------------

include(cmake/CompileVulkanShader.cmake)

# ----------------
# External dependencies.
# ----------------

find_package(Vulkan REQUIRED)

if (VKUTIL_USE_GLFW_WINDOW)
    find_package(GLFW3 REQUIRED)
    find_package(glm CONFIG REQUIRED)
endif()

# ----------------
# Module configurations for external dependencies.
# ----------------

file(DOWNLOAD https://raw.githubusercontent.com/stripe2933/Cpp20Module/main/Vulkan.cmake
    ${PROJECT_BINARY_DIR}/Vulkan.cmake
)
include(${PROJECT_BINARY_DIR}/Vulkan.cmake)

if (VKUTIL_USE_GLFW_WINDOW)
    file(DOWNLOAD https://raw.githubusercontent.com/stripe2933/Cpp20Module/main/glm.cmake
        ${PROJECT_BINARY_DIR}/glm.cmake
    )
    include(${PROJECT_BINARY_DIR}/glm.cmake)
endif()

# ----------------
# Project libraries.
# ----------------

add_library(vkutil)
target_include_directories(vkutil PUBLIC
    include
)
target_sources(vkutil PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS interface
    FILES
        interface/details/details.cppm
        interface/details/ranges.cppm
        interface/allocator.cppm
        interface/buffer.cppm
        interface/command.cppm
        interface/descriptor.cppm
        interface/image.cppm
        interface/pipeline.cppm
        interface/ref_holder.cppm
        interface/vkutil.cppm
)
target_link_libraries(vkutil PUBLIC
    VulkanHppModule
)

if (VKUTIL_USE_GLFW_WINDOW)
    target_sources(vkutil PUBLIC
        FILE_SET CXX_MODULES
        FILES ${PROJECT_SOURCE_DIR}/interface/window.cppm
    )
    target_link_libraries(vkutil PUBLIC
        glfw
        glm::module
    )
    target_compile_definitions(vkutil PRIVATE
        VKUTIL_USE_GLFW_WINDOW
        GLFW_INCLUDE_NONE
    )
endif()