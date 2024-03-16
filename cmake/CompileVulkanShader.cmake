# Compile shader files and copy them into build directory.
#
# Usage:
# compile_shader(target
#     FORMAT spv
#     SOURCES path1/shader1.vert path2/shader2.frag ...
# )
#
# Result: path1/shader1.vert.spv, path2/shader2.frag.spv, ... in build directory.

find_package(Vulkan COMPONENTS glslc REQUIRED)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

function(compile_shader target)
    # Extract 1st and 4th digits
    string(SUBSTRING ${VKUTIL_VK_VERSION} 0 1 FIRST_DIGIT)
    string(SUBSTRING ${VKUTIL_VK_VERSION} 3 1 FOURTH_DIGIT)

    # Combine them with a period delimiter
    set(TARGET_ENV_VK_VERSION "--target-env=vulkan${FIRST_DIGIT}.${FOURTH_DIGIT}")

    cmake_parse_arguments(PARSE_ARGV 1 arg "" "FORMAT" "SOURCES")
    foreach(source ${arg_SOURCES})
        add_custom_command(
            OUTPUT ${PROJECT_BINARY_DIR}/${source}.${arg_FORMAT}
            DEPENDS ${PROJECT_SOURCE_DIR}/${source}
            DEPFILE ${PROJECT_SOURCE_DIR}/${source}.d
            COMMAND ${glslc_executable} ${TARGET_ENV_VK_VERSION} ${PROJECT_SOURCE_DIR}/${source} -o ${PROJECT_BINARY_DIR}/${source}.${arg_FORMAT}
        )
        target_sources(${target} PRIVATE ${PROJECT_BINARY_DIR}/${source}.${arg_FORMAT})
    endforeach()
endfunction()