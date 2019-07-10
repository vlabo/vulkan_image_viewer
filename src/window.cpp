#include "window.h"
#include <stdexcept>

Window::Window()
    : m_window(nullptr)
{
}

Window::~Window()
{
    destroy();
}

void Window::init(const char* name, int width, int height)
{
    destroy();

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

void Window::destroy()
{
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        m_window = nullptr;
    }
}

std::vector<const char*>
Window::getRequiredExtensions()
{
    if (m_window == nullptr) {
        return std::vector<const char*>();
    }

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	if (glfwExtensions == nullptr || glfwExtensionCount == 0) {
		return {};
	}
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    return extensions;
}

void Window::processEvents()
{
    if (m_window == nullptr) {
        return;
    }
    glfwPollEvents();
}

bool Window::isOpen()
{
    if (m_window == nullptr) {
        return false;
    }
    return !static_cast<bool>(glfwWindowShouldClose(m_window));
}

VkSurfaceKHR Window::createSurface(VkInstance instance)
{
    if (m_window == nullptr) {
        throw std::runtime_error("Failed to create VkSurfaceKHR: m_window == NULL.");
    }
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult result = glfwCreateWindowSurface(instance, m_window, nullptr, &surface);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create VkSurfaceKHR: result != VK_SUCCESS.");
    }

    return surface;
}