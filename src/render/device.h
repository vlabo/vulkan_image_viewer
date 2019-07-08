#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>

class Device
{
public:
    Device();
    ~Device();

    void                    initLibrary( std::vector<const char*> extensions );
    void                    initDevice( VkSurfaceKHR surface );
    void                    initRenderer();
    VkInstance              getVkInstance() const   {   return m_instance.get();  }

private:
    void                    initPhysicalDevice( vk::UniqueInstance& instance );
    bool                    isPhysicalDeviceSuitable( vk::PhysicalDevice& device );
    bool                    checkDeviceExtensionSupport( vk::PhysicalDevice& device );
    void                    createDevice();
    void                    createSwapChain();

    vk::SurfaceFormatKHR    chooseSwapSurfaceFormat( const std::vector<vk::SurfaceFormatKHR>& availableFormats );
    vk::PresentModeKHR      chooseSwapPresentMode( const std::vector<vk::PresentModeKHR>& availablePresentModes );
    vk::Extent2D            chooseSwapExtent( const vk::SurfaceCapabilitiesKHR& capabilities );

private:
    vk::UniqueInstance      m_instance;
    vk::PhysicalDevice      m_physicalDevice;
    vk::UniqueDevice        m_device;
    vk::UniqueSurfaceKHR    m_surface;
    vk::UniqueSwapchainKHR  m_swapchain;
    std::vector<vk::Image>  m_swapchainImages;
};