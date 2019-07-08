#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>
#include <vector>

#include "window.h"
#include "render/device.h"

class App
{
public:
    App();
    ~App();

    void    run(); // app starting point

    void    setup();
    void    mainLoop();
    void    cleanup();
private:
    Window m_window;
    Device m_device;
};