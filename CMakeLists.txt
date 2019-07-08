cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH "thirdparty/vulkan/lib")

project(vulkan_image_viewer C CXX)

# guard against in-source builds and bad build-type strings
include(cmake/safeguards.cmake)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# get git submoduls
include(cmake/git_submodels.cmake)

# project version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

add_executable(vulkan_image_viewer  "") # MACOSX_BUNDLE

add_subdirectory(src)
add_subdirectory(thirdparty)

file(GLOB LIBRARIES "thirdparty/vulkan/lib/*.dylib")

target_link_libraries(vulkan_image_viewer PUBLIC vulkan HLSL vulkan.1 vulkan.1.1.108 MoltenVK glfw)



include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )


# enable testing
enable_testing()


# define tests
add_subdirectory(tests)
