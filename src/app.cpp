#include "app.h"

#include <vector>
#include <vulkan/vulkan.h>

const int WIDTH = 800;
const int HEIGHT = 600;

App::App() 
{
}

App::~App()
{
}

void App::run()
{
    setup();
    mainLoop();
    cleanup();
}

void App::setup()
{
    m_window.init( "Vulkan", WIDTH, HEIGHT );
    m_device.initLibrary(m_window.getRequiredExtensions() );
    VkSurfaceKHR surface = m_window.createSurface( m_device.getVkInstance() );
    m_device.initDevice( surface );
    m_device.initRenderer();
}

void    App::mainLoop()
{
    while( m_window.isOpen() )
    {
        m_window.processEvents();
    }
}

void    App::cleanup()
{

}
