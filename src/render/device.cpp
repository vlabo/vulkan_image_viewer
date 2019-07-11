#define NOMINMAX

#include "device.h"
#include "utils.h"

#include <algorithm>
#include <limits>

#include <set>

#ifndef NDEBUG

#include "vulkandebug.h"
constexpr std::array<const char*, 2> debugExtensions = {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};
constexpr std::array<const char*, 1> debugLayers = {
    "VK_LAYER_KHRONOS_validation"
};
#endif

constexpr std::array<const char*, 1> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void Device::initLibrary(std::vector<const char*> extensions)
{

    std::vector<const char*> layers;
#ifndef NDEBUG
    printAvaliableExtensions();
    printAvaliableLayers();
    layers.insert(layers.end(), debugLayers.begin(), debugLayers.end());
    extensions.insert(extensions.end(), debugExtensions.begin(), debugExtensions.end());
#endif

    vk::InstanceCreateInfo instanceInfo(vk::InstanceCreateFlags(), nullptr, layers.size(), layers.data(), extensions.size(), extensions.data());

    m_instance = vk::createInstanceUnique(instanceInfo);
#ifndef NDEBUG
    registerDebugLog(m_instance);
#endif
}

void Device::initDevice(VkSurfaceKHR surface)
{
    m_surface = vk::UniqueSurfaceKHR(surface, *m_instance);
    initPhysicalDevice(m_instance);
    createDevice();
    createSwapChain();
    createImageViews();
}

void Device::printAvaliableExtensions()
{
    std::cout << "Extensions:\n";
    std::vector<vk::ExtensionProperties> allEx = vk::enumerateInstanceExtensionProperties();
    for (const auto& ext : allEx) {
        std::cout << ext.extensionName << '\n';
    }
}

void Device::printAvaliableLayers()
{
    std::cout << "Layers:\n";
    std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
    for (const auto& ley : layerProperties) {
        std::cout << ley.layerName << '\n';
    }
}

vk::SurfaceFormatKHR Device::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    if (availableFormats.empty()) {
        throw std::runtime_error("chooseSwapSurfaceFormat: format list is empty.");
    }

    for (const auto& surfaceFormat : availableFormats) {
        if (surfaceFormat.format == vk::Format::eB8G8R8A8Unorm && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return surfaceFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR Device::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Device::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    vk::Extent2D actualExtent = { 800, 600 }; // TODO: replace with actual size

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

void Device::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice, m_surface);

    vk::SurfaceFormatKHR imageFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    m_swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities);
    m_swapChainImageFormat = imageFormat.format;

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    bool hasImageCountLimit = swapChainSupport.capabilities.maxImageCount > 0;
    if (hasImageCountLimit && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.setSurface(m_surface.get())
        .setMinImageCount(imageCount)
        .setImageFormat(imageFormat.format)
        .setImageColorSpace(imageFormat.colorSpace)
        .setImageExtent(m_swapChainExtent)
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface);
    std::array<uint32_t, 2> queueFamilyIndices = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
            .setQueueFamilyIndexCount(2)
            .setPQueueFamilyIndices(queueFamilyIndices.data());
    } else {
        createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
    }

    createInfo.setPreTransform(swapChainSupport.capabilities.currentTransform)
        .setPresentMode(presentMode)
        .setClipped(VK_TRUE);

    m_swapChain = m_device->createSwapchainKHRUnique(createInfo);
    m_swapChainImages = m_device->getSwapchainImagesKHR(m_swapChain.get());
}

void Device::createImageViews()
{
    m_swapChainImageViews.reserve(m_swapChainImages.size());
    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo;
        createInfo.setImage(m_swapChainImages[i])
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(m_swapChainImageFormat);
        // createInfo.setComponents(vk::ComponentMapping()); this is set by default
        vk::ImageSubresourceRange subresourceRange;
        subresourceRange.setBaseMipLevel(0)
            .setLevelCount(1)
            .setBaseArrayLayer(0)
            .setLayerCount(1);
        createInfo.setSubresourceRange(subresourceRange);

        m_swapChainImageViews.push_back(m_device->createImageViewUnique(createInfo));
    }
}

void Device::initRenderer() {}

void Device::initPhysicalDevice(vk::UniqueInstance& instance)
{
    std::vector<vk::PhysicalDevice> devices = instance->enumeratePhysicalDevices();

    bool isDeviceSelected = false;
    for (vk::PhysicalDevice& currentDevice : devices) {
        if (isPhysicalDeviceSuitable(currentDevice) && checkDeviceExtensionSupport(currentDevice)) {
            m_physicalDevice = currentDevice;
            isDeviceSelected = true;
        }
    }
    if (!isDeviceSelected) {
        throw std::runtime_error("initPhysicalDevice: Unable to select physical device.");
    }
}

bool Device::isPhysicalDeviceSuitable(vk::PhysicalDevice& device)
{
    QueueFamilyIndices indices = findQueueFamilies(device, m_surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, m_surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool Device::checkDeviceExtensionSupport(vk::PhysicalDevice& device)
{
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();

    for (const vk::ExtensionProperties& property : extensions) {
        requiredExtensions.erase(property.extensionName);
    }

    return requiredExtensions.empty();
}

void Device::createDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0F;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    vk::DeviceCreateInfo createInfo;

    createInfo.setQueueCreateInfoCount(queueCreateInfos.size());
    createInfo.setPQueueCreateInfos(queueCreateInfos.data());

    createInfo.setEnabledExtensionCount(deviceExtensions.size());
    createInfo.setPpEnabledExtensionNames(deviceExtensions.data());

#ifndef NDEBUG
    createInfo.setEnabledLayerCount(debugLayers.size());
    createInfo.setPpEnabledLayerNames(debugLayers.data());
#endif

    m_device = m_physicalDevice.createDeviceUnique(createInfo);
}
