#pragma once

#include <iostream>
#include <sstream>
#include <vulkan/vulkan.hpp>

PFN_vkCreateDebugReportCallbackEXT  pfnVkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
  return pfnVkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
  pfnVkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL dbgFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objType*/, uint64_t /*srcObject*/, size_t /*location*/, int32_t msgCode, const char *pLayerPrefix, const char *pMsg, void * /*pUserData*/)
{
  std::ostringstream message;

  switch (flags)
  {
    case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
      message << "INFORMATION: ";
      break;
    case VK_DEBUG_REPORT_WARNING_BIT_EXT:
      message << "WARNING: ";
      break;
    case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
      message << "PERFORMANCE WARNING: ";
      break;
    case VK_DEBUG_REPORT_ERROR_BIT_EXT:
      message << "ERROR: ";
      break;
    case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
      message << "DEBUG: ";
      break;
    default:
      message << "unknown flag (" << flags << "): ";
      break;
  }
  message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

#ifdef _WIN32
  MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
#else
  std::cout << message.str() << std::endl;
#endif

  return false;
}

void    registerDebugLog( vk::UniqueInstance& instance )
{
    pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance->getProcAddr("vkCreateDebugReportCallbackEXT"));
    if (!pfnVkCreateDebugReportCallbackEXT)
    {
        throw std::runtime_error( "GetInstanceProcAddr: Unable to find vkCreateDebugReportCallbackEXT function." );
    }

    pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance->getProcAddr("vkDestroyDebugReportCallbackEXT"));
    if (!pfnVkDestroyDebugReportCallbackEXT)
    {
        throw std::runtime_error( "GetInstanceProcAddr: Unable to find vkDestroyDebugReportCallbackEXT function." );
    }

    vk::UniqueDebugReportCallbackEXT debugReportCallback = instance->createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning, dbgFunc));
}