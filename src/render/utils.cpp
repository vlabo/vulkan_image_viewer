#include "utils.h"

QueueFamilyIndices findQueueFamilies( vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface )
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) 
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags &  vk::QueueFlagBits::eGraphics ) 
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        device.getSurfaceSupportKHR( i, surface.get(), &presentSupport );

        if (queueFamily.queueCount > 0 && presentSupport)
            indices.presentFamily = i;

        if (indices.isComplete()) 
            break;

        i++;
    }

    return indices;
}

SwapChainSupportDetails querySwapChainSupport( vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface ) {
    SwapChainSupportDetails details;

    details.capabilities    = device.getSurfaceCapabilitiesKHR  ( surface.get() );
    details.formats         = device.getSurfaceFormatsKHR       ( surface.get() );
    details.presentModes    = device.getSurfacePresentModesKHR  ( surface.get() );

    return details;
}
