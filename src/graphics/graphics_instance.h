#pragma once
#include <stdint.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct EvGraphicsIntance_T* EvGraphicsIntance;

typedef struct EvGraphicsIntance_T{
    VkInstance instance;
    VkDebugUtilsMessengerEXT messenger;
} EvGraphicsIntance_T;

EvGraphicsIntance evCreateGraphicsInstance(const char* pAppName, uint32_t appVersion, bool enableValidation);
void evDestroyGraphicsInstance(EvGraphicsIntance instance);