#pragma once
#include "graphics/graphics_instance.h"
#include <vulkan/vulkan.h>

typedef struct EvGraphicsDevice_T* EvGraphicsDevice;

typedef struct EvGraphicsDevice_T{
    VkPhysicalDevice physical;
    uint32_t graphicsFamily, transferFamily, computeFamily;
    VkDevice logical;
    VkQueue graphicsQueue, transferQueue, computeQueue;
} EvGraphicsDevice_T;

EvGraphicsDevice evCreateGraphicsDevice(EvGraphicsIntance instance);
void evDestroyGraphicsDevice(EvGraphicsDevice device);