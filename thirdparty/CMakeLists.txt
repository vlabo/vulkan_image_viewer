# GLFW
add_subdirectory(glfw)
target_include_directories(vulkan_image_viewer PRIVATE "glfw/include")

# Vulkan
target_include_directories(vulkan_image_viewer PRIVATE "vulkan/include")
if(APPLE)
target_link_directories(vulkan_image_viewer PRIVATE "/usr/local/lib")
elseif(CMAKE_CL_64)
target_link_directories(vulkan_image_viewer PRIVATE "vulkan/lib/windows")
endif()
