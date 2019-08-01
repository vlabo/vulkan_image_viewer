#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

class Device {
public:
    Device() = default;
    ~Device() = default;

    void initLibrary(std::vector<const char*> extensions);
    void initDevice(VkSurfaceKHR surface);
    void createGraphicsPipeline();
    VkInstance getVkInstance() const { return m_instance.get(); }

private:
    void initPhysicalDevice(vk::UniqueInstance& instance);
    bool isPhysicalDeviceSuitable(vk::PhysicalDevice& device);
    bool checkDeviceExtensionSupport(vk::PhysicalDevice& device);
    void createDevice();
    void createSwapChain();
	void createImageViews();
	void createRenderPass();

    vk::ShaderModule createShaderModule(const std::vector<char>& code);

	void printAvaliableExtensions();
	void printAvaliableLayers();

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

private:
    vk::UniqueInstance m_instance;
    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice m_device;
    vk::UniqueSurfaceKHR m_surface;

    vk::UniqueSwapchainKHR m_swapChain;
    std::vector<vk::Image> m_swapChainImages;
	vk::Format m_swapChainImageFormat;
	vk::Extent2D m_swapChainExtent;

	vk::UniquePipelineLayout m_pipelineLayout;
	vk::UniqueRenderPass m_renderPass;
	vk::UniquePipeline m_pipeline;

	std::vector<vk::UniqueImageView>	m_swapChainImageViews;
};