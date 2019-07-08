target_sources(
    vulkan_image_viewer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/app.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/render/device.cpp
        ${CMAKE_CURRENT_LIST_DIR}/render/utils.cpp

        ${CMAKE_CURRENT_LIST_DIR}/render/vulkandebug.h
        ${CMAKE_CURRENT_LIST_DIR}/render/device.h
        ${CMAKE_CURRENT_LIST_DIR}/render/utils.h
        ${CMAKE_CURRENT_LIST_DIR}/window.h
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/app.h
    )

