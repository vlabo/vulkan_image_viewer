#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
    
public:
    Window();
    ~Window();

    void            init( const char* name, int width, int height );
    VkSurfaceKHR    createSurface( VkInstance instance );
    void            destroy();
    std::vector<const char*>    getRequiredExtensions();

    void    processEvents();
    bool    isOpen();

private:
    GLFWwindow* m_window;
};