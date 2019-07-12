#include "utils.h"
#include <fstream>

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface)
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = VK_FALSE;
        device.getSurfaceSupportKHR(i, surface.get(), &presentSupport);

        if (queueFamily.queueCount > 0 && static_cast<bool>(presentSupport)) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface)
{
    SwapChainSupportDetails details;

    details.capabilities = device.getSurfaceCapabilitiesKHR(surface.get());
    details.formats = device.getSurfaceFormatsKHR(surface.get());
    details.presentModes = device.getSurfacePresentModesKHR(surface.get());

    return details;
}

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}
